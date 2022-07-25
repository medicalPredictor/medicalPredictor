import random
from Graphcode import *

##Rachel has a question: what does pass do? Is it like break/continue?

seed = 568946
numNodes = 128
popsize = 50
chrSize = int(numNodes * (numNodes - 1) / 2)
varImmStringSize = 7
initVar = [0, 0, 0, 0, 0, 0, 1]
pop = []
popFits = []
alpha, recProb, decProb = 0.5, 0.8, 0.3
tSize = 5
runs = 30
gens = 10000
rptIvl = 1000
maxMuts = 3
random.seed(seed)

def init():
  global pop, popFits
  pop = []
  popFits = []
  for _ in range(popsize):
    mem = randomGraph(numNodes)
    pop.append(mem)
    fit = fitness(mem)
    popFits.append(fit)
    pass
  pass


def randomGraph(nodes: int):
  assert chrSize == nodes * (nodes - 1) / 2
  lst = [random.randint(0, 1) for _ in range(chrSize)]
  return lst


def tourn(size):
  fitTups = []
  for i in range(size):
    idx = random.randint(0, popsize-1)
    fitTups.append((idx, popFits[idx]))
    pass
  fitTups.sort(key=lambda y: y[1])
  return fitTups

#RB: is the fitness the number of bits in the bitstring that is used to make the graph? So maximum connections?
def fitness(lst):
  graph = Graph(numNodes, lst, varImmStringSize, initVar, alpha, recProb, decProb)
  graph.infect(0, 0)
  # Run Sim
  #infectedLog = simulation(graph)
  #return(infectedLog.max())
  graph.updateInfected()
  infects1 = np.sum(graph.currentInfected())
  graph.updateInfected()
  infects2 = np.sum(graph.currentInfected())
  graph.updateInfected()
  infects3 = np.sum(graph.currentInfected())
  #print("\nFirst 3 timesteps:")
  #print(infects1)
  #print(infects2)
  #print(infects3)
  return(max(infects1, infects2, infects3))
  #return lst.count(1)


def matingEvent():
  tMems = tourn(tSize)
  parent1 = pop[tMems[tSize - 1][0]]
  parent2 = pop[tMems[tSize - 2][0]]
  cP1, cP2 = random.randint(0, chrSize-1), random.randint(0, chrSize-1)
  if cP1 > cP2:
    cP1, cP2 = cP2, cP1
    pass
  child1, child2 = [None for _ in range(chrSize)], [None for _ in range(chrSize)]
  child1[:cP1] = parent1[:cP1]
  child2[:cP1] = parent2[:cP1]
  child1[cP1:cP2] = parent2[cP1:cP2]
  child2[cP1:cP2] = parent1[cP1:cP2]
  child1[cP2:] = parent1[cP2:]
  child2[cP2:] = parent2[cP2:]
  muts = random.randint(0, maxMuts) 
  #print(muts)
  for _ in range(muts):
    child1[random.randint(0, len(child1)-1)] = random.randint(0, 1)
    child2[random.randint(0, len(child2)-1)] = random.randint(0, 1)
    pass
  pop[tMems[0][0]] = child1
  pop[tMems[1][0]] = child2
  popFits[tMems[0][0]] = fitness(child1)
  popFits[tMems[1][0]] = fitness(child2)
  pass


def report(run, intv):
  print("Run", run, ":", intv, "Mean: ", np.average(popFits), "Best: ", max(popFits))
  pass


def finalReport(run):
  print("Run", run, "FINAL", "Mean: ", np.average(popFits), "Best: ", max(popFits))
  print()
  pass


def evolve():
  print("MAX MAX:", chrSize)
  for run in range(runs):
    init()
    report(run, 0)
    for gen in range(1, gens + 1):
      if gen % rptIvl == 0:
        report(run, int(gen / rptIvl))
        pass
      matingEvent()
      pass
    finalReport(run)
    pass
  print("Execution Complete!")
  pass
  


#evolve()