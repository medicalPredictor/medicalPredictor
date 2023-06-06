import copy
# import operator
# import math
import random
import sys
import numpy as np
import csv
import os
# import pandas as pd
from math import exp, log
# import matplotlib.pyplot
# from PIL import Image, ImageTk
# from graphviz import Graph
from statistics import mean

from typing import List

alpha = 0.63
immunity_length = 7


def infected(sick: int, potent_ones: int, total_ones: int):
    if alpha == 1:
        return True
    else:
        beta = 1 - exp(sick * log(1 - alpha))
        ratio = potent_ones / total_ones  # ratio in [0, 1]
        if random.random() < (beta * ratio):
            return True
    return False


def get_variants():
    variants = []
    with open("./variants.txt") as f:
        lines = f.readlines()
        for line in lines:
            variant = []
            line = line.rstrip("\n")
            line = line.split("\t")
            for val in line:
                variant.append(int(val))
                pass
            variants.append(variant)
            pass
        pass
    return variants


def count_mismatches(immunity: [], variant: []):
    potent_cnt = 0
    total_cnt = 0
    for val in variant:
        if val == 1:
            total_cnt += 1
            pass
        pass
    for idx in range(len(immunity)):
        if variant[idx] == 1 and immunity[idx] == 0:
            potent_cnt += 1
            pass
        pass
    if total_cnt == 0:
        print("The following list has zero ones: ")
        print(variant)
        pass
    return potent_cnt, total_cnt


def gen_new_variant(idx: int):
    variants = get_variants()
    print("Variant " + str(idx) + ": ", end="")
    print(variants[idx])
    return variants[idx]


def StoI(variant_str: [], immunity_str: []):
    for idx in range(len(variant_str)):
        if variant_str[idx] == 1:
            immunity_str[idx] += 1
            pass
        pass
    return immunity_str


def decay(variant_str: [], immunity_str: []):
    for idx in range(len(variant_str)):
        if variant_str[idx] == 1:
            immunity_str[idx] -= 1
            pass
        pass
    return immunity_str


def run_epi(adj: List[List[int]], p0, var_prob: float, death_prob: float, decay_prob: float, rec_prob: float,
            var_timesteps: [] or None):
    num_nodes = len(adj)
    var_count = 0
    n_state = [0 for _ in range(num_nodes)]
    v_state = [-1 for _ in range(num_nodes)]
    inf_logs = [[] for _ in range(num_nodes)]
    IDR_logs = [[1], [0], [0]]
    n_state[p0] = 1
    v_state[p0] = var_count
    epi_log = [[p0]]
    inf_logs[p0].append(0)
    var_logs = []
    variants = []

    num_infected = 1
    new_infected = 1
    ttl_infected = 0
    time_step = 0
    length = 0

    immunity = [[0 for _ in range(immunity_length)] for _ in range(num_nodes)]
    variants.append(gen_new_variant(var_count))
    immunity[0] = []
    immunity[0].extend(variants[var_count])  # immunity for current variant
    var_count += 1
    var_logs.append([0, [1]])

    while num_infected > 0 and time_step < num_nodes:
        if (var_timesteps is None and random.random() < var_prob) or time_step in var_timesteps:  # new variant
            variants.append(gen_new_variant(var_count))
            var_logs.append([time_step, [1]])
            var_count += 1
            success = False
            tries = 0
            while not success and tries < num_nodes:
                node = random.randint(0, num_nodes - 1)
                if n_state[node] == 0:  # susceptible
                    potent_cnt, total_cnt = count_mismatches(immunity[node], variants[-1])
                    if potent_cnt > 0:  # not entirely immune
                        n_state[node] = 1
                        v_state[node] = var_count - 1
                        success = True
                        pass
                    pass
                tries += 1
                pass
            pass

        inf_neighbours = [[] for _ in range(num_nodes)]
        for n in range(num_nodes):
            if n_state[n] == 1:  # infected
                for nei in adj[n]:
                    inf_neighbours[nei].append(n)
                    pass
                pass
            pass
        for n in range(num_nodes):
            if n_state[n] == 0 and len(inf_neighbours[n]) > 0:
                for nei in inf_neighbours[n]:
                    if n_state[n] == 0:
                        potent_cnt, total_cnt = count_mismatches(immunity[n], variants[v_state[nei]])
                        if infected(1, potent_cnt, total_cnt):
                            n_state[n] = 3  # new infected
                            v_state[n] = v_state[nei]  # with variant
                            pass
                        pass
                    pass
                pass
            pass
        ttl_infected += new_infected
        new_infected = 0
        new_inf = []
        for log in var_logs:
            log[1].append(0)
            pass

        for node in range(num_nodes):
            if len(inf_logs[node]) > 0 and random.random() < decay_prob:
                to_rmv = inf_logs[node][0]
                inf_logs[node] = inf_logs[node][1:]
                immunity[node] = decay(variants[to_rmv], immunity[node])
                pass
            pass

        new_deaths = 0
        new_recoveries = 0
        for n in range(num_nodes):
            if n_state[n] == 1:  # infected -> susceptible
                if random.random() < rec_prob:
                    n_state[n] = 0
                    new_recoveries += 1
                    num_infected -= 1
                    pass
                elif random.random() < death_prob:
                    n_state[n] = 2
                    new_deaths += 1
                    num_infected -= 1
                    pass
                pass
            elif n_state[n] == 3:  # new infected -> infected
                n_state[n] = 1
                num_infected += 1
                new_infected += 1
                new_inf.append(n)
                immunity[n] = StoI(variants[v_state[n]], immunity[n])
                var_logs[v_state[n]][1][-1] += 1
                pass
            elif n_state[n] == 0:  # susceptible
                pass
            elif n_state[n] == 2:  # removed
                pass
        epi_log.append(new_inf)
        IDR_logs[0].append(new_infected)
        IDR_logs[1].append(new_deaths)
        IDR_logs[2].append(new_recoveries)
        length += 1
        time_step += 1
        pass
    return epi_log, IDR_logs, var_logs


def fitness_bare(adj_lists: List[List[int]], nodes: int, p0: int, varProb: float, deathProb: float, decayProb: float):
    temp_list = copy.deepcopy(adj_lists)
    var_count = 0
    n_state = [0 for _ in range(nodes)]  # susceptible
    v_state = [-1 for _ in range(nodes)]
    n_state[p0] = 1
    v_state[p0] = var_count
    epi_log = [[p0]]
    num_infected = 1
    ttl_infected = 0
    time_step = 0
    length = 0
    immunity = [[0 for _ in range(immunity_length)] for _ in range(nodes)]
    variants = get_variants()
    immunity[0] = variants[var_count]
    var_count += 1
    while num_infected > 0 and time_step < nodes:
        inf_neighbours = [0 for _ in range(nodes)]
        for n in range(nodes):
            if n_state[n] == 1:
                for nei in temp_list[n]:
                    inf_neighbours[nei] += 1
                    pass
                pass
            pass
        for n in range(nodes):
            if n_state[n] == 0 and inf_neighbours[n] > 0:
                if infected(inf_neighbours[n], count_mismatches(immunity[n], v_state)):
                    n_state[n] = 3
                    pass
                pass
            pass
        ttl_infected += num_infected
        num_infected = 0
        new_inf = []
        for n in range(nodes):
            if n_state[n] == 1:  # infected -> removed
                n_state[n] = 2
            elif n_state[n] == 3:
                n_state[n] = 1
                num_infected += 1
                new_inf.append(n)
        epi_log.append(new_inf)
        length += 1
        time_step += 1
    return epi_log, ttl_infected, length


def make_prof(adj_lists: List[List[int]], nodes: int, p0):
    logs = []
    most = -1
    for _ in range(1000):
        prof = fitness_bare(adj_lists, nodes, p0)[0]
        logs.append(prof)
        if len(prof) > most:
            most = len(prof)
            pass
        pass

    avg_prof = [0 for _ in range(most)]
    for idx in range(most):
        for pr in logs:
            if idx < len(pr):
                avg_prof[idx] += len(pr[idx])
                pass
            pass
        pass

    for idx in range(most):
        avg_prof[idx] = avg_prof[idx] / 1000
        pass

    return avg_prof


def make_file(epi_log, IDR_log, var_log):
    with open("./DATA2.DAT", "w") as f:
        f.write("Experiment 1\n___\n")
        for epi in range(len(IDR_log)):
            for val in IDR_log[epi][0]:
                f.write(str(val) + " ")
                pass
            f.write("\n")
            for val in IDR_log[epi][1]:
                f.write(str(val) + " ")
                pass
            f.write("\n")
            for val in IDR_log[epi][2]:
                f.write(str(val) + " ")
                pass
            f.write("\n___\n")
            for idx, log in enumerate(var_log[epi]):
                for bit in gen_new_variant(idx):
                    f.write(str(bit) + " ")
                    pass
                f.write("\nStart Date: ")
                f.write(str(log[0]) + "\n")
                for val in log[1]:
                    f.write(str(val) + " ")
                    pass
                f.write("\n")
                pass
            f.write("___\n")
            pass
        pass
    pass


def main():
    num_nodes = 256
    var_prob = 0.01
    death_prob = 0.00168214
    decay_prob = 0.006666
    rec_prob = 0.126

    file = "./bs_test.txt"
    adj = [[0 for _ in range(num_nodes)] for _ in range(num_nodes)]
    with open(file, 'r') as f:
        lines = f.readlines()
        line = lines[0].rstrip('\n').split('\t')
        cnt = 0
        for row in range(num_nodes):
            for col in range(row + 1, num_nodes):
                adj[row][col] = int(line[cnt])
                adj[col][row] = int(line[cnt])
                cnt += 1
                pass
            pass
        pass

    lists = []
    for row in range(num_nodes):
        li = []
        for col in range(num_nodes):
            if adj[row][col] == 1:
                li.append(col)
                pass
            pass
        lists.append(li)
        pass

    multi_IDR, multi_var_logs = [], []
    var_timesteps = [5, 10, 15]
    for _ in range(10):
        epi_log, IDR_logs, var_logs = run_epi(lists, 0, var_prob, death_prob, decay_prob, rec_prob, var_timesteps)
        print(epi_log)
        print(IDR_logs[0])
        print(IDR_logs[1])
        print(IDR_logs[2])
        for log in var_logs:
            print(log)
            pass
        multi_IDR.append(IDR_logs)
        multi_var_logs.append(var_logs)
        pass

    make_file(epi_log, multi_IDR, multi_var_logs)
    return 0

    pass


main()
