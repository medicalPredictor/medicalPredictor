from Graphcode import *
#from Graphcode import Graph

bsInit = open("bs_test.txt", "r")
bits1 = bsInit.readlines()
bits = bits1[0].split("\t")
graph2 = Graph(256, bits, 7, bits, 0.63, 0.126, 0.006666) #build graph
simulation(graph2)

#edgeTest()
#nextTests()

print("here")