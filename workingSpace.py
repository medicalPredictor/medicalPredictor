from Graphcode import *
from EvolAlgor import *

import time

start = time.time()

bsInit = open("bs_test.txt", "r")
bits1 = bsInit.readlines()
bits = bits1[0].split("\t")
graph2 = Graph(256, bits, 7, bits, 0.63, 0.126, 0.006666) #build graph
#simulation(graph2)

#edgeTest()
evolve()

print("Time:")
end = time.time()
print(end - start)