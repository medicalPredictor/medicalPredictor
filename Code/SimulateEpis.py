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

alpha = 1


def infected(sick: int):
    if alpha == 1:
        return True
    else:
        beta = 1 - exp(sick *  log(1 - alpha))
        if random.random() < beta:
            return True
    return False


def fitness_bare(adj_lists: List[List[int]], nodes: int, p0):
    temp_list = copy.deepcopy(adj_lists)
    n_state = [0 for _ in range(nodes)]  # susceptible
    n_state[p0] = 1
    epi_log = [[p0]]
    num_infected = 1
    ttl_infected = 0
    time_step = 0
    length = 0
    while num_infected > 0 and time_step < nodes:
        inf_neighbours = [0 for _ in range(nodes)]
        for n in range(nodes):
            if n_state[n] == 1:
                for nei in temp_list[n]:
                    inf_neighbours[nei] += 1
        for n in range(nodes):
            if n_state[n] == 0 and inf_neighbours[n] > 0:
                if infected(inf_neighbours[n]):
                    n_state[n] = 3
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
        avg_prof[idx] = avg_prof[idx]/1000
        pass

    return avg_prof


def main():
    file = "../bs_test.txt"
    adj = [[0 for _ in range(256)] for _ in range(256)]
    edges = 0
    # tot_weight = 0
    # weight_cnt = [0 for _ in range(5)]
    with open(file, 'r') as f:
        lines = f.readlines()
        line = lines[0].rstrip('\n').split('\t')
        cnt = 0
        for row in range(256):
            for col in range(row+1, 256):
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