import jax
import jax.numpy as jnp
from jax import random as jrandom
from jax import nn as jnn
from jax import jit
import numpy as np
import random
import time
import matplotlib.pyplot as plt

#global variable? Really? RB
verbose = False

class Graph:

  # Takes:
  # # bitspray1; list, the upper triangle of the adj matrix
  # # nn; int, represents population size
  # # immunityStringSize; int, defines bitstring size for varient infection. 
  # # bitSpray2; list, the initial infection varient
  # # alpha; float, intrinsic infection percentage
  # # bitSpray2; list, not to sure?

  def __init__(self, nn, bitSpray1, immunityStringSize, bitSpray2, alpha, recProb, decProb):
    # generates adj matrix from bitSpray
    self.num_nodes = nn
    self.adj = np.zeros((nn,nn))
    counter = 0
    #might be worth fidning a way to remove this loop RB
    for v in range(1,nn): # this for loop cuts up the bit spray smartly
      self.adj[v-1][v:nn] = bitSpray1[counter:(nn-v+counter)]  
      counter+=nn-v 
    self.adj+= self.adj.T # finishes making adj matrix

    self.state = np.array([0 for _ in range(self.num_nodes)]) #sets all nodes to state uninfected
    # use self.state = np.zeros(self.num_nodes) ^ 
    #self.stateLog = []
    self.infected = np.array([0 for _ in range(self.num_nodes)]) #sets all nodes to not infected? 
    # use self.infected = np.zeros(self.num_nodes) ^ 

    self.alpha = alpha 
    self.recoveryProb = recProb 
    self.decayProb = decProb 
    self.deathProb = 0.00168214 #use literature values

    self.bitstringSize = immunityStringSize
    self.VariantDic = [] # contains all variants seen by the simulation
    self.VariantDic.append(np.array(bitSpray2[0:self.bitstringSize]).astype("float32")) # so bitSpray2 is the first varient?
    self.immunityDic = np.zeros((nn, self.bitstringSize)) 
    self.variantHistory = [[] for _ in range(self.num_nodes)] # contains the varients each node has had
    #find a way to remove this loop RB
    # for x in range(0,nn):
    #   self.variantHistory.append([])
    #print(self.variantHistory)
    self.deathcount = 0
  
  # add_edge: individually adds edges if desired. 
  # Takes: 
  # # node1, node2; ints representing the two nodes we make an edge between
  def add_edge(self, node1, node2): 
    if (node1 < 0) or (node1 >= self.num_nodes):
      print("Error. Node1 outside Bounds.")
      return()
    if (node2 < 0) or (node2 >= self.num_nodes):
      print("Error. Node2 outside Bounds.")
      return()
    self.adj[node1][node2] = 1
    self.adj[node2][node1] = 1

  # infect: infects node with variant. 
  # Takes: 
  # # node; int representing the node to be infected
  # # variant; bitstring, represents variant to infect with
  # Rachel Note. Reverified function
  def infect(self, node, variant):
    if (node < 0) or (node >= self.num_nodes): # checks if valid node
      print("Error. Node outside Bounds.")
      return(2) # why return 2? It's an error code. I needed to know if a new variant was successful at infecting a node. So it returns 0 if successful, 1 if it tried with no errors but the node could not be infected, and 2 if invalid input was given. RB
    if(isinstance(variant, int) == False): #sometimes the variant passed was not an int. I forced it to be an int. RB
        variant = int(variant[0]) 
    if (len(self.VariantDic) < variant):
      print("Error. Variant not found in Dictionary.")
      return(3)
    if((self.state[node] == 0) or (self.state[node] == -1)): # lets go through this (Matt)
      dif = self.VariantDic[int(variant)].astype("float32") - self.immunityDic[int(node)] #subtract the immunity string from the variant string RB
      if(not(all(i <= 0 for i in dif))): #if any 1s are left standing, infect the string. This is mostly a backup for calculating the probabilty. I found it useful for trying to infect with new variants. RB
        self.state[int(node)] = int(variant) + 1 
        self.infected[int(node)] = 1
        self.immunityDic[int(node)] += self.VariantDic[int(variant)].astype("float32")
        self.variantHistory[int(node)].append(variant+1)
        return(0)
    return(1)

  # unfect: makes node immune. 
  # Takes: 
  # # node; int, node to become immune
  def unInfect(self, node):
    if (node < 0) or (node >= self.num_nodes): 
      print("Error. Node outside Bounds.")
      return(2)
    if(self.state[node] >= 1):
      #print("uninfecting:")
      #print(node)
      self.state[node] = -1
      self.infected[node] = 0
    return(0)

  # removeImmunity: removes immunity of a node
  # Takes:
  # # node; int, node to lose immunity. 
  # to incorporate varients we will have to bring in the varient record for this node?
  #Rachel Note. Functionality Verified.
  def removeImmunity(self, node): 
    if(node < 0) or (node >= self.num_nodes):
      print("Error. Node outside Bounds.")
      return(2)
    if(self.state[node] == -1): #if the node is in the recovered state
      self.state[node] = 0
      if(len(self.variantHistory[node])>0): #if the node has been infected before
        variant = self.variantHistory[node][0] - 1 #looks at oldest variant in its own infection history
        self.immunityDic[node] -= self.VariantDic[variant] # subtracts oldest variant from its immunity string
        self.variantHistory[node].pop(0) #removes oldest variant from its infection history
    return(0)


  def calcExtra(self, Ac, x, totalbits, variant):
    #print("start")
    #print(Ac[x])
    if(Ac[x] == 0):
      return(0.0) #if the node has no infected neighbours, set the probabilty of infection for that node to 0.
    if(Ac[x] >= 1):
      #print(Ac[x])
      mismatches = 0;
      #if verbose: print(self.immunityDic[x])
      #if verbose: print(self.VariantDic[variant])
      vec = self.VariantDic[variant] - self.immunityDic[x] #subtract the immunity string from the variant string
      #print(self.immunityDic[x])
      #print(self.VariantDic[variant])
      #if verbose: print("vec:")
      #print(vec)
      mismatches = sum(np.where(vec >= 1, 1, 0))
      # print(mismatches2)
      # for y in vec: #go through the vector created from the difference, and record the number of values greater than 0. This is because the immunity string can be greater than one.
      #   # if verbose: print(y)
      #   if(y > 0):
      #     mismatches = mismatches + 1
      #print(mismatches)
      #print(mismatches/totalbits)
      #print("Done")
      #extra.append(mismatches/totalbits) #create a vector that hold the modifier for the the probabilty calc.
    return(mismatches/totalbits)

  # removeImmunity: produces the chance every node has to become infected. 
  # Takes:
  # # node; int, node to lose immunity. 
  # NEED TO INCORPORATE VARIENTS
  #Rachel Note, the variants were incorporated, you commented them out.
  #Rachel Note. Functionality Verified.
  def infectionProb(self, Ac, variant): # this 
    #put the probabililty curve here
    extra = []
    #if verbose: print("In infectionProb")
    #if verbose: print(Ac)
    #if verbose: print("sum of self.VariantDic[variant]:")
    totalbits = sum(self.VariantDic[variant]) #total bits in the infecting bitstring
    #if verbose: print(totalbits)
    #No I actually needed all this. It adds the bitstring immunity to the probabilty calculation. RB
    #attepting to convert this loop to a single function
    #for each node, calculate the modifer from the immunity strings and add it to the vector "extra"
    extra = np.array([self.calcExtra(Ac, x, totalbits, variant) for x in range(0,len(Ac))])
    #print(extra)
    #calc probabilty as normal
    exps = Ac * np.log(1 - self.alpha)
    actualExp = np.exp(exps)
    probs = 1 - actualExp
    probs2 = np.multiply(probs, extra) #add in the immunity string modifier
    if verbose: print("probs2:")
    if verbose: print(probs2)
    if verbose: print("End of infectionProb")
    return(probs2)

  # runInfections (matt got here)
  def runInfections(self, new, variant):
    #variant = 0 #i need to know the state of the neighbour infecting the node in order to infect the node with the correct varriant. Rachel
    vec = np.where((self.state == 0) & (new == 1))
    #print(vec[0])
    #vec = np.where(self.state == 0, newInfections, self.state)
    err = np.array([self.infect(x,variant) for x in vec[0]])
    #for x in vec[0]:   #there is a loop here. Not sure how to remove it, updating states by replacing the vec completely did not work. Rachel
    #  if(new[x] == 1):
    #    self.infect(x,variant)
    return(0)

  #this works so far. Rachel
  def recoveries(self):
    vec = np.where(self.state >= 1)
    #print("In recoveries")
    #print(vec[0])
    rand = np.random.rand(len(vec[0]))
    #print(rand)
    err = np.where(rand < self.recoveryProb)
    #print(err)
    err2 = np.array([self.unInfect(vec[0][x]) for x in err[0]])
    # for x in range(0,len(rand)):
    #   if(rand[x] < self.recoveryProb):
    #     print(x)
    #     print(vec[0][x])
    #     self.unInfect(vec[0][x])
    #self.printState()
    #self.printInfected()
    #print("Done Recoveries")
    return(0)

  def death(self, node):
    if (node < 0) or (node >= self.num_nodes): 
      print("Error. Node outside Bounds.")
      return()
    if(self.state[node] >= 1):
      self.state[node] = -2
      self.infected[node] = 0
      self.deathcount = self.deathcount + 1

  #this also seems to work fine. Rachel
  def immunityDecay(self):
    vec2 = np.array([len(x) for x in self.variantHistory])
    vec3 = np.where((vec2 >= 1) & (self.state < 1) & (self.state != -2))
    rand = np.random.rand(len(vec3[0]))
    err = np.where(rand < self.decayProb)
    err2 = np.array([self.removeImmunity(vec3[0][x]) for x in err[0]])
    # for x in range(0,len(rand)):
    #   if(rand[x] < self.decayProb):
    #     self.removeImmunity(vec3[0][x])
    return(0)

  def currentState(self):
    return(self.state)

  def currentInfected(self):
    return(self.infected)

  def currentVariantHistory(self):
    return(self.variantHistory)

  def currentDeathCount(self):
    return(self.deathcount)

  def currentLifeCount(self):
    return(self.num_nodes - self.deathcount)

  def runVariant(self, variant):
    Ac = self.infectedNeighbours(variant)
    randVec = np.random.rand(self.num_nodes)
    #if randvec is less than infectionsProb (values between 0 and 1), then put randvec values, otherwise, use AC
    newInfections = np.where(randVec < self.infectionProb(Ac, variant), Ac, randVec)
    newInfections = np.where(newInfections <= 1, newInfections, 1) #if any node has more than one infected neighbour, this converts it to 1. It's important. Don't remove it. RB
    newInfections = np.floor(newInfections) # spits out 0 for fail, 1 for success
    return(newInfections)

  #Rachel Note. Now does more than one variant by implementing masking of the state vector.
  def updateInfected(self): # this updates the infections after 1 round
    #if verbose: print("In update infected")
    #for x in self.VariantDic:
      #if verbose: print(x)
    #if verbose: print("End of variant list")
    #if verbose: print(len(self.VariantDic))
    if(len(self.VariantDic) == 0): #this is the basic one that uses only one variant. i modified it to be 0 instead of one to see if the mask works with the basecase.
      Ac = jnp.dot(self.adj, self.infected)
      randVec = np.random.rand(self.num_nodes)
      newInfections = np.where(randVec < self.infectionProb(Ac, 0), Ac, randVec)
      newInfections = np.where(newInfections <= 1, newInfections, 1)
      newInfections = np.floor(newInfections)
      self.immunityDecay()
      self.recoveries()
      self.runInfections(newInfections, 0)
    else: #this one is modified for more than one variant.
      variant = 0
      infectionList = []
      infectionList1 = []
      infectionList2 = []
      #Mask created in infectedNeighbours. Takes the variant index, and creates Ac. RB
      infectionList2 = np.array([self.runVariant(x) for x in range(0,len(self.VariantDic))])
      # for x in range(0,len(self.VariantDic)):
      #   infectionList1.append(self.runVariant(x))
      #   Ac = self.infectedNeighbours(x)
      #   randVec = np.random.rand(self.num_nodes)
      #   #if randvec is less than infectionsProb (values between 0 and 1), then put randvec values, otherwise, use AC
      #   newInfections = np.where(randVec < self.infectionProb(Ac, x), Ac, randVec)
      #   newInfections = np.where(newInfections <= 1, newInfections, 1) #if any node has more than one infected neighbour, this converts it to 1. It's important. Don't remove it. RB
      #   newInfections = np.floor(newInfections) # spits out 0 for fail, 1 for success
      #   infectionList.append(newInfections) # makes a list 
      #print(infectionList)
      #print(infectionList1)
      #print(infectionList2)
      #put in here calc new immunity decay
      self.immunityDecay()
      #put in here calc new recoveries
      self.recoveries()
      self.kill()
      infectionList2 = np.array([self.runInfections(infectionList2[y], y) for y in range(0, len(infectionList2))])
      #for y in range(0, len(infectionList2)):
        #self.runInfections(infectionList2[y], y)
      return(self.state)

  def printAdj(self):
    for x in self.adj:
      print(x)

  def printState(self):
      print(self.state)
  
  def printInfected(self):
      print(self.infected)

  def infectedNeighbours(self, variant):
    mask = np.where(self.state == (variant+1), 1, 0)
    Ac = jnp.dot(self.adj, mask)
    return(Ac)

  def newVariant(self, bits):
    self.VariantDic.append(np.array(bits[0:self.bitstringSize]).astype("float32"))
    worked = 1
    node = np.random.randint(0,self.num_nodes)
    #print(node)
    #print(len(self.VariantDic))
    worked = self.infect(node, self.VariantDic[len(self.VariantDic)-1])
    if verbose: print("")
    if verbose: print("Worked:")
    if verbose: print(worked)
    counter = 0
    while(worked > 0):
      node = np.random.randint(0,self.num_nodes)
      worked = self.infect(node, self.VariantDic[len(self.VariantDic)-1])
      if verbose: print("")
      if verbose: print("Worked:")
      if verbose: print(worked)
      counter += 1
      if(counter > self.num_nodes):
        if verbose: print("No one to infect. New Variant failed to infect.")
        return

  def kill(self):
    #print("Starting to kill off people")
    vec = np.where(self.state >= 1)
    rand = np.random.rand(len(vec[0]))
    #print(rand)
    err = np.where(rand < self.deathProb)
    #print(err)
    err2 = np.array([self.death(vec[0][x]) for x in err[0]])
    # for x in range(0,len(rand)):
    #   if(rand[x] < self.deathProb):
    #     self.death(vec[0][x])
    #self.printState()
    #self.printInfected()
    #print("Done Killing")

#this is the function for the simulation. It is rough so I can get an idea of what steps are involved Rachel

# simulation: takes a graph and generates an epidemic simulation
# AGraph; a graph object, represents the contact network of a city
# Output:
# scatter plot containing infections over time.
def simulation(AGraph):
  infectedLog = []
  deathsLog = []
  lifeLog = []
  AGraph.infect(0,0)
  max_timesteps = 1000
  #AGraph.updateInfected()
  #infects = np.sum(AGraph.infected)
  #infectedLog.append(infects)
  count = 0
  # while infects > 0 and count < 1000:
  #   AGraph.updateInfected()
  #   infects = np.sum(AGraph.currentInfected())
  #   deaths = AGraph.currentDeathCount()
  #   life = AGraph.currentLifeCount()
  #   infectedLog.append(infects)
  #   deathsLog.append(deaths)
  #   lifeLog.append(life)
  #   count+=1
  thing = np.array([nextTimestep(AGraph) for _ in range(int(max_timesteps/5))])
  #print(thing)
  #print(thing.transpose())
  thing2 = thing.transpose()
  infectedLog = thing2[0]
  deathsLog = thing2[1]
  lifeLog = thing2[2]

  #print(infectedLog[len(infectedLog)-1])
  if(infectedLog[len(infectedLog)-1] > 0):
    thing = np.array([nextTimestep(AGraph) for _ in range(int(max_timesteps/5))])
    thing2 = thing.transpose()
    infectedLog = np.append(infectedLog,thing2[0])
    deathsLog = np.append(deathsLog,thing2[1])
    lifeLog = np.append(lifeLog,thing2[2])

  if(infectedLog[len(infectedLog)-1] > 0):
    thing = np.array([nextTimestep(AGraph) for _ in range(int(max_timesteps/5))])
    thing2 = thing.transpose()
    infectedLog = np.append(infectedLog,thing2[0])
    deathsLog = np.append(deathsLog,thing2[1])
    lifeLog = np.append(lifeLog,thing2[2])

  if(infectedLog[len(infectedLog)-1] > 0):
    thing = np.array([nextTimestep(AGraph) for _ in range(int(max_timesteps/5))])
    thing2 = thing.transpose()
    infectedLog = np.append(infectedLog,thing2[0])
    deathsLog = np.append(deathsLog,thing2[1])
    lifeLog = np.append(lifeLog,thing2[2])
  
  if(infectedLog[len(infectedLog)-1] > 0):
    thing = np.array([nextTimestep(AGraph) for _ in range(int(max_timesteps/5))])
    thing2 = thing.transpose()
    infectedLog = np.append(infectedLog,thing2[0])
    deathsLog = np.append(deathsLog,thing2[1])
    lifeLog = np.append(lifeLog,thing2[2])
  
  fig, ax = plt.subplots()
  ax.scatter(range(len(infectedLog)), infectedLog)
  ax.scatter(range(len(deathsLog)), deathsLog)
  ax.scatter(range(len(lifeLog)), lifeLog)
  plt.savefig("Results.png")
  print("Blue is Cases, Orange is Deaths, Green is Currently alive nodes")

def nextTimestep(AGraph):
    AGraph.updateInfected()
    infects = np.sum(AGraph.currentInfected())
    deaths = AGraph.currentDeathCount()
    life = AGraph.currentLifeCount()
    return([infects, deaths, life])


#bsInit = open("bs_test.txt", "r")
#bits1 = bsInit.readlines()
#bits = bits1[0].split("\t")
#print(bits)
#given that immunity disappears after approximately 6 months ~ 180 days. P(losing immunity given that you havent lost immunty after 179 days) = 0.7
#probabilty that you still have immunity after 180 days in the row = x^(180) = 0.3
# x = 180 root(0.3)
#x = 0.99333358101
# therefore the probabilty of losing your immunity on any particular day is 1-x
#which is 0.006666417986643
#for recovery
#probaility of not recovering after 12 days = 0.2 which is 1- 0.8, the probabilty of recovering after 11 days
#12 root of 0.2 = 0.87448527222
#1- 0.87448527222 = 0.125514
#probabilty of infection from paper Agrawal and Bhardwaj 2021, probablity of infection, given that you are hit by a cough cloud and the infected person is not wearing a mask, is about 63%
# - probability of death over entire infection is ~ 2%
# - p(death | not dead after 11 days (average length of infection)) = 0.02
# - p(not dead after 12 days) = 0.98
# - x^(12) = 0.98
# - x = 0.998317858
# - 1-x = 0.00168214
#graph2 = Graph(256, bits, 7, bits, 0.63, 0.126, 0.006666) #build graph
#simulation(graph2)

#so far Graph, printAdj and add_edge work. Rachel
#next infect looks good. Rachel
#next uninfect looks good. Rachel
#contining testing. Rachel

def edgeTest():
  bits = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
  graph1 = Graph(7, bits, 7, [0,1,1,1,0,0,1,0,1,0,0,1,0,1,0,1], 0.5, 0.8, 0.3) #build graph
  graph1.printAdj()
  print("")
  graph1.add_edge(0,1) #add edge 0,1
  graph1.printAdj()
  print("")
  graph1.add_edge(-1,1) #try to add edge outside of bounds
  graph1.printAdj()
  print("")
  graph1.add_edge(20,1) #try to add edge outside of bounds
  graph1.printAdj()
  print("")
  graph1.add_edge(0,-1) #try to add edge outside of bounds
  graph1.printAdj()
  print("")
  graph1.add_edge(0,20) #try to add edge outside of bounds
  graph1.printAdj()
  print("")
  graph1.add_edge(0,10) #try to add edge outside of bounds
  graph1.printAdj()
  print("")
  graph1.add_edge(10,0) #try to add edge outside of bounds
  graph1.printAdj()
  print("")
  graph1.add_edge(2,4) #add edge at 2,4
  graph1.printAdj()
  print("")
  graph1.add_edge(2,1) #add edge at 2,1
  graph1.printAdj()
  print("")
  graph1.add_edge(5,6) #add edge at 5,6
  graph1.printAdj()
  print("")
  graph1.add_edge(5,2) #add edge at 5,2
  graph1.printAdj()
  print("")
  

  print("Infecting node 1")
  graph1.infect(1,0)
  print("state:")
  graph1.printState()
  print("infected:")
  graph1.printInfected()

  print("")
  print("Infecting node 3")
  graph1.infect(3,0)
  print("state:")
  graph1.printState()
  print("infected:")
  graph1.printInfected()

  print("")
  print("Uninfecting node 3")
  graph1.unInfect(3)
  print("state:")
  graph1.printState()
  print("infected:")
  graph1.printInfected()

  print("")
  print("Infecting node 3")
  graph1.infect(3,0)
  print("state:")
  graph1.printState()
  print("infected:")
  graph1.printInfected()

  print("")
  print("Infecting node 4")
  graph1.infect(4,0)
  print("state:")
  graph1.printState()
  print("infected:")
  graph1.printInfected()
  print("")

  print("Infecting node 6")
  graph1.infect(6,0)
  print("state:")
  graph1.printState()
  print("infected:")
  graph1.printInfected()

  print("Killing node 6")
  graph1.death(6)
  print("state:")
  graph1.printState()
  print("infected:")
  graph1.printInfected()

  print("")
  #vec1 = np.array([0,1,0,0,0,0,0,1,0,0]) #testing multiplying the currently infected vector vec1 with the associativity matrix
  #print(graph1.nextInfected(vec1))

  # print("state:")
  # graph1.printState()
  # print("infected:")
  # graph1.printInfected()

  print("")
  print("Updating to next infection round")
  graph1.updateInfected()
  print("state:")
  graph1.printState()
  print("infected:")
  graph1.printInfected()

  # print("")
  # print("Updating to next infection round")
  # graph1.updateInfected()
  # print("state:")
  # graph1.printState()
  # print("infected:")
  # graph1.printInfected()

  graph1.newVariant([1,0,0,0,1,1,1,1,0,1])
  print(graph1.currentState())

  print("")
  print("Updating to next infection round")
  graph1.updateInfected()
  print("state:")
  graph1.printState()
  print("infected:")
  graph1.printInfected()

  #print(graph1.currentVariantHistory())

  #print("")
  #print("Updating to next infection round")
  #graph1.updateInfected()
  #print("state:")
  #graph1.printState()
  #print("infected:")
  #graph1.printInfected()

  #print(graph1.currentVariantHistory())

def nextTests():
  bits = [0,1,0,0,0,0,1,0,1,0,1,1,1,0,0,0,1,0,1,0,1,0,1,1,0]
  graph1 = Graph(7, bits, 7, [0,1,1,1,0,0,1,0,1,0,0,1,0,1,0,1]) #build graph
  graph1.printAdj()
  bsInit = open("bs_test.txt", "r")
  bits1 = bsInit.readlines()
  bits = bits1[0].split("\t")
  print(bits)
  graph2 = Graph(256, bits, 7, [0,1,1,1,0,0,1,0,1,0,0,1,0,1,0,1]) #build graph
  #graph2.printAdj()
  print("")
  print("Infecting node 3")
  graph2.infect(3,0)
  print("state:")
  graph2.printState()
  print("infected:")
  graph2.printInfected()

  graph2.updateInfected()
  print("state:")
  graph2.printState()
  print("infected:")
  graph2.printInfected()



#edgeTest()
#nextTests()