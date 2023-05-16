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

alpha = 0.01
immunity_length = 7


def infected(sick: int, potent_ones: int, total_ones: int):
    if alpha == 1:
        return True
    else:
        beta = 1 - exp(sick * log(1 - alpha))
        ratio = potent_ones/total_ones  # ratio in [0, 1]
        if random.random() < (beta * ratio):
            return True
    return False


def get_variants():
    variants = []
    with open("variant.txt") as f:
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
    total_cnt = variant.count(1)
    for idx in range(len(immunity)):
        if variant[idx] == 1 and immunity[idx] == 0:
            potent_cnt += 1
            pass
        pass
    return potent_cnt, total_cnt


def gen_new_variant(idx: int):
    variants = get_variants()
    return variants[idx]


def StoI(variant_str: [], immunity_str: []):
    for idx in range(len(variant_str)):
        if variant_str[idx] == 1:
            immunity_str[idx] = 1
            pass
        pass
    return immunity_str


def run_epi(adj: List[List[int]], p0, var_prob: float, death_prob: float, decay_prob: float, var_timesteps: [] or None):
    num_nodes = len(adj)
    var_count = 0
    n_state = [0 for _ in range(num_nodes)]
    v_state = [-1 for _ in range(num_nodes)]
    n_state[p0] = 1
    v_state[p0] = var_count
    epi_log = [[p0]]
    var_logs = []
    variants = []

    num_infected = 1
    ttl_infected = 0
    time_step = 0
    length = 0

    immunity = [[0 for _ in range(immunity_length)] for _ in range(nodes)]
    variants.append(gen_new_variant(var_count))
    var_count += 1
    var_logs.append([0, [1]])
    immunity[0] = variants[var_count]  # immunity for current variant

    while num_infected > 0 and time_step < nodes:
        if var_timesteps is None:
            if random.random() < var_prob:  # new variant
                variants.append(gen_new_variant(var_count))
                var_count += 1
                success = False
                tries = 0
                while not success and tries < num_nodes:
                    node = randint(0, num_nodes - 1)
                    if n_state[node] == 0:  # susceptible
                        potent_cnt, total_cnt = count_mismatches(variants[-1], immunity[node])
                        if potent_cnt > 0:  # not entirely immune
                            n_state[node] = 1
                            v_state[node] = var_count - 1
                            success = True
                            pass
                        pass
                    tries += 1
                    pass
                pass
            pass
        elif time_step in var_timesteps:  # make new variant this timestep
            # TODO: Add functionality.
            pass
        
        inf_neighbours = [[] for _ in range(nodes)]
        for n in range(nodes):
            if n_state[n] == 1:  # infected
                for nei in adj[n]:
                    inf_neighbours[nei].append(n)
                    pass
                pass
            pass
        for n in range(nodes):
            if n_state[n] == 0 and len(inf_neighbours[n]) > 0:
                for nei in inf_neighbours[n]:
                    if n_state[n] == 0:
                        potent_cnt, total_cnt = count_mismatches(immunity[n], v_state[nei])
                        if infected(1, potent_cnt, total_cnt):
                            n_state[n] = 3  # new infected
                            v_state[n] = v_state[nei]  # with variant
                            pass
                        pass
                    pass
                pass
            pass
        ttl_infected += num_infected
        num_infected = 0
        new_inf = []
        for n in range(nodes):
            if n_state[n] == 1:  # infected -> susceptible
                n_state[n] = 0
                pass
            elif n_state[n] == 3:  # new infected -> infected
                n_state[n] = 1
                num_infected += 1
                new_inf.append(n)
                immunity[n] = StoI(variants[v_state[n]], immunity[n])
                pass
            elif n_state[n] == 0:  # susceptible
                pass
            elif n_state[n] == 2:  # removed
                pass
        epi_log.append(new_inf)
        length += 1
        time_step += 1
        pass
    return epi_log, var_logs


def fitness_bare(adj_lists: List[List[int]], nodes: int, p0, varProb: float, deathProb: float, decayProb: float):
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


def main():
    hist = [0 for _ in range(100)]
    for _ in range(10):
        sum = 0
        for _ in range(10000):
            val = (random.randint(0, 50000) % 100) / 100
            sum += val
            hist[int(val * 100)] += 1
            pass
        print(sum / 10000)
    print(hist)
    print(min(hist))
    print(max(hist))
    print(hist.index(max(hist)) / 100)
    file = "./bs_test.txt"
    adj = [[0 for _ in range(256)] for _ in range(256)]
    edges = 0
    # tot_weight = 0
    # weight_cnt = [0 for _ in range(5)]
    with open(file, 'r') as f:
        lines = f.readlines()
        line = lines[0].rstrip('\n').split('\t')
        cnt = 0
        for row in range(256):
            for col in range(row + 1, 256):
                adj[row][col] = int(line[cnt])
                adj[col][row] = int(line[cnt])
                cnt += 1
                pass
            pass
        pass

    lists = []
    for row in range(256):
        li = []
        for col in range(256):
            if adj[row][col] == 1:
                li.append(col)
                pass
            pass
        lists.append(li)
        pass

    avg_prof = make_prof(lists, 256, 0)
    with open("BaseSIRResults.txt", "w") as f:
        print(avg_prof)
        for val in avg_prof:
            if val > 0:
                print(val)
                f.write(str(val) + "\n")
                pass
            pass
        pass

    # lists = []
    # with open("dublin_graph.dat", "w") as f:
    #     f.write(str(200) + " " + str(edges) + " " + str(tot_weight) + "\n")
    #     for val in weight_cnt:
    #         f.write(str(val) + " ")
    #         pass
    #     f.write("\n")
    #     for rowIdx in range(200):
    #         li = []
    #         for colIdx in range(200):
    #             for _ in range(int(adj[rowIdx][colIdx])):
    #                 li.append(int(colIdx))
    #                 f.write(str(colIdx) + " ")
    #                 pass
    #             pass
    #         lists.append(li)
    #         f.write("\n")
    #         pass
    #     pass

    # avgProf = make_prof(lists, 200, 0)

    # with open("Profiles/Profile0.dat", "w") as f:
    #     for av in avgProf:
    #         f.write(str(av) + "\n")
    #         pass
    #     pass

    pass


main()
