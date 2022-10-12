#include <cmath>
#include <list> 
#include <sstream>
#include <vector>
#include <iostream>
#include <time.h>
#include <fstream>

using namespace std;

void ReadData(string input1, std::vector <int>  * listOfPoints, int linesize);
void tokenize(std::string const str, const char delim, std::vector<int> * out);
void writeToFile(vector <int> data, string filename);

class Graph
{
	public:
		// functions go here
		int initialize(int nn1, vector <int> bitspray, int immunityStringSize, vector <int> bitspray2, double alpha1, double recProb1, double decProb1);
		int addVariant(vector <int> bits, int len);
		int printVariantList();
		int printImmunityList();
		int printVariantHistory();
		int infect(int node, int variant);
		vector <int> getState();
		int unInfect(int node);
		int removeImmunity(int node);
		vector <double> infectionProb(vector <int> Ac, int variant);
		vector <int> calcAc(int variant);
		int runInfections(vector <int> new1, int variant);
		int recoveries();
		int death(int node);
		int ImmunityDecay();
		vector <int> getInfected();
		vector < vector <int> > getCurrentVariantHistory();
		int deathCount();
		int lifeCount();
		vector <int> runVariant(int variant);
		int updateInfected();
		void printAdj();
		int newVariant(vector <int> bits);
		void kill();
		vector <int> nextTimeStep();
		int numOfVariants();
	private:
		//Variables go here
		double alpha;
		double recProb;
		double decProb;
		double deathProb;
		vector < vector <int> > adj;
		int nn;
		int immunitySize;
		vector <int> state;
		vector <int> infected;
		vector < vector <int> > VariantDic;
		vector < vector <int> > immunityDic;
		vector < vector <int> > variantHistory;
		int deathcount;
		double calcExtra(vector <int> Ac, int x, int totalBits, int variant);
};

/**
 * Initializes a graph object with the following settings.
 *
 * @param nn1  Number of nodes. (int)
 * @param bitspray  Vector of integers representing the adjacency matrix of the graph. 
 * @param immunityStringSize  Length of the immunity string. (int)
 * @param bitspray2  Vector of integers representing the first variants immunity string.
 * @param alpha1  Base probability of infection.
 * @param recProb1  Probability of recovering on any particular day.
 * @param decProb1 Probability of immunity decay on any particular day.
 * @return          If the program completes successfully, returns 0.
 */
int Graph::initialize(int nn1, vector <int> bitspray, int immunityStringSize, vector <int> bitspray2, double alpha1, double recProb1, double decProb1)
{
	nn = nn1;
	int counter = 0;
	vector <int> holder;
	for(int x = 0; x < nn; x++)
	{
		holder.push_back(0);
		state.push_back(0);
		infected.push_back(0);
	}
	for( int y = 0; y < nn; y++)
	{
		adj.push_back(holder);
	}
	
	for(int x = 0; x < adj.size(); x++)
	{
		for(int y = x+1; y < adj[x].size(); y++)
		{
			adj[x][y] = bitspray[counter];
			adj[y][x] = bitspray[counter];
			counter++;
		}
	}

	alpha = alpha1;
	recProb = recProb1;
	decProb = decProb1;
	deathProb = 0.00168214;
	
	immunitySize = immunityStringSize;
	addVariant(bitspray2, immunitySize);
	
	vector <int> holder2;
	for(int y = 0; y < immunitySize; y++)
	{
		holder2.push_back(0);
	}
	
	for(int x = 0; x < nn; x++)
	{
		immunityDic.push_back(holder2);
	}
	
	for(int x = holder.size(); x > 0; x--)
	{
		holder.pop_back();
	}
	for(int x = 0; x < nn; x++)
	{
		variantHistory.push_back(holder);
	}
	
	deathcount = 0;
	
	return(0);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
int Graph::addVariant(vector <int> bits, int len)
{
	vector <int> variant;
	for(int x = 0; x < len; x++)
	{
		variant.push_back(bits[x]);
	}
	VariantDic.push_back(variant);
	return(0);
}

/**
 * Prints the immunity string for each node to the commandline.
 *
 * @return          If the program completes successfully, returns 0.
 */
int Graph::printImmunityList()
{
	cout << "Immunity List:\n";
	for(int x = 0; x < immunityDic.size(); x++)
	{
		for(int y = 0; y < immunityDic[x].size(); y++)
		{
			cout << immunityDic[x][y] << " ";
		}
		cout << "\n";
	}
	return(0);
}

/**
 * Prints the Lists of Variants to the commandline.
 *
 * @return          If the program completes successfully, returns 0.
 */
int Graph::printVariantList()
{
	cout << "Variant List:\n";
	for(int x = 0; x < VariantDic.size(); x++)
	{
		for(int y = 0; y < VariantDic[x].size(); y++)
		{
			cout << VariantDic[x][y] << " ";
		}
		cout << "\n";
	}
	return(0);
}

/**
 * Prints the infection variant history for each node. This vector helps keep track of immunity decay, as the first member of this vector is removed during immunity decay. New infections are added at the end.
 *
 * @return          If the program completes successfully, returns 0.
 */
int Graph::printVariantHistory()
{
	cout << "Variant History List:\n";
	for(int x = 0; x < variantHistory.size(); x++)
	{
		for(int y = 0; y < variantHistory[x].size(); y++)
		{
			cout << variantHistory[x][y] << " ";
		}
		cout << "\n";
	}
	return(0);
}

/**
 * Prints an integer vector. Mostly for debugging purposes. 
 *
 * @param vec  A vector of intergers of any length.
 * @return          If the program completes successfully, returns 0.
 */
int printVector(vector <int> vec)
{
	for(int y = 0; y < vec.size(); y++)
	{
		cout << vec[y] << " ";
	}
	cout << "\n";
	return(0);
}

/**
 * Prints a vector of doubles. Mostly for debugging purposes. 
 *
 * @param vec  A vector of doubles of any length.
 * @return          If the program completes successfully, returns 0.
 */
int printVector(vector <double> vec)
{
	for(int y = 0; y < vec.size(); y++)
	{
		cout << vec[y] << " ";
	}
	cout << "\n";
	return(0);
}

/**
 * Returns a copy of the state vector. This is to ensure encapsulation of the information, and guarantee internal consistancy of the object.
 *
 * @return          A copy of the state vector. 
 */
vector <int> Graph::getState()
{
	return(state);
}

/**
 * Returns a copy of the vector of currently infected nodes. This is to ensure encapsulation of the information, and guarantee internal consistancy of the object.
 *
 * @return          A copy of the vector of currently infected nodes.
 */
vector <int> Graph::getInfected()
{
	return(infected);
}

/**
 * Returns a copy of variant history for each node, contained in a vector of vector of ints. This is to ensure encapsulation of the information, and guarantee internal consistancy of the object.
 *
 * @return          A copy of variant history for each node, contained in a vector of vector of ints.
 */
vector < vector <int> > Graph::getCurrentVariantHistory()
{
	return(variantHistory);
}

/**
 * Returns the current death total of the simulation. This is to ensure encapsulation of the information, and guarantee internal consistancy of the object.
 *
 * @return          Current death count of the simulation. 
 */
int Graph::deathCount()
{
	return(deathcount);
}

/**
 * Returns the current number of nodes that are still alive in the simulation. This is to ensure encapsulation of the information, and guarantee internal consistancy of the object.
 *
 * @return          The current number of nodes that are still alive in the simulation.
 */
int Graph::lifeCount()
{
	return(nn - deathcount);
}

/**
 * Infects the passed node with the passed variant. Updates all internal datastructures to reflect the new information. Checks to ensure the node index is between 0 and total number of nodes. Checks to ensure variant exists. Only infects if the immunity string does not cover all ones in the variant string.
 *
 * @param node      integer representing the index of the node to be infected.
 * @param variant   index of the variant from the variant dictionary that is to infect the node.
 * @return          If the program completes successfully, returns 0. If the node is out of bounds, returns 2. If the variant does not exist in the dictionary, returns 3. Otherwise, returns 1.
 */
int Graph::infect(int node, int variant)
{
	if(node < 0 | node >= nn)
	{
		cout << "Error. Node outside of Bounds.\n";
		return(2);
	}
	if(variant > VariantDic.size())
	{
		cout << "Error. Variant Not Found.\n";
		return(3);
	}
	int stuff = 0;
	int counter = 0;
	if(state[node] == 0 | state[node] == -1)
	{
		for(int x = 0; x < immunitySize; x++)
		{
			stuff = VariantDic[variant][x] - immunityDic[node][x];
			if(stuff > 0)
			{
				counter++;
			}
		}
		if(counter > 0)
		{
			state[node] = variant + 1;
			infected[node] = 1;
			for(int x = 0; x < immunityDic[node].size(); x++)
			{
				immunityDic[node][x] = immunityDic[node][x] + VariantDic[variant][x];
			}
			variantHistory[node].push_back(variant+1);
			return(0);
		}
	}
	return(1);
}

/**
 * Remove infection from the passed node. Updates internal data structures to reflect the new state. Checks to ensure the node index is between 0 and total number of nodes.
 *
 * @param node  Integer representing the index of the node to be infected.
 * @return          If the program completes successfully, returns 0. If the node is out of bounds, returns 2.
 */
int Graph::unInfect(int node)
{
	if(node < 0 | node >= nn)
	{
		cout << "Error. Node outside Bounds.\n";
		return(2);
	}
	if(state[node] >= 1)
	{
		state[node] = -1;
		infected[node] = 0;
	}
	return(0);
}

/**
 * Removes immunity from passed node. Checks if the node dead, or if the node has immunity to lose before updating. Variant history, immunity string and state updated to reflect new information.
 *
 * @param node  Index representing node to be updated. 
 * @return          If the program completes successfully, returns 0.
 */
int Graph::removeImmunity(int node)
{
	int variant = 0;
	if(node < 0 | node >= nn)
	{
		cout << "Error. Node outside Bounds.\n";
		return(2);
	}
	if((state[node] < 1) && (state[node] != -2)) //(maybe remove this?)
	{
		state[node] = 0;
		if(variantHistory[node].size() > 0)
		{
			variant = variantHistory[node][0] -1;
			for(int x = 0; x < immunityDic[node].size(); x++)
			{
				immunityDic[node][x] = immunityDic[node][x] - VariantDic[variant][x];
			}
			variantHistory[node].erase(variantHistory[node].begin());
		}
		
	}
	return(0);
}

/**
 * Additional calculation to include immunity string into probability of infection. 
 *
 * @param Ac  Integer vector representing the number of neighbours each node has that is infected with the variant passed.
 * @param x  index of node involved in the calculation.
 * @param totalBits  length of 
 * @param variant  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
double Graph::calcExtra(vector <int> Ac, int x, int totalBits, int variant)
{
	if(Ac[x] == 0)
	{
		return(0.0);
	}
	int mismatches = 0;
	if(Ac[x] > 0)
	{
		int dif = 0;
		for (int y = 0; y < VariantDic[variant].size(); y++)
		{
			dif = VariantDic[variant][y] - immunityDic[x][y];
			if(dif >= 1)
			{
				mismatches++;
			}
		}
	}
	return(mismatches/totalBits);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
vector <double> Graph::infectionProb(vector <int> Ac, int variant)
{
	vector <double> probs;
	vector <double> extra;
	vector <double> extra2;
	int totalBits = 0;
	for(int x = 0; x < VariantDic[variant].size(); x++)
	{
		totalBits = totalBits + VariantDic[variant][x];
	}

	double a = 0.0;
	for (int x = 0; x < Ac.size(); x++)
	{
		a = calcExtra(Ac, x, totalBits, variant);
		extra.push_back(a);
	}
	
	double b = log(1- alpha);
	for(int x = 0; x < Ac.size(); x++)
	{
		extra2.push_back(Ac[x]*b);
	}
	
	for(int x = 0; x < Ac.size(); x++)
	{
		extra2[x] = 1 - exp(extra2[x]);
	}

	for(int x = 0; x < Ac.size(); x++)
	{
		probs.push_back(extra2[x]*extra[x]);
	}
	
	return(probs);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
vector <int> Graph::calcAc(int variant)
{
	vector <int> Ac;
	vector <int> mask;
	for(int x = 0; x < state.size(); x++)
	{
		if(state[x] == variant+1)
		{
			mask.push_back(1);
		}
		else
		{
			mask.push_back(0);
		}
	}
	int a;
	for(int x = 0; x < nn; x++)
	{
		a = 0;
		for(int y = 0; y < nn; y++)
		{
			a = a + (adj[x][y]*mask[y]);
		}
		Ac.push_back(a);
	}
	return(Ac);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
int Graph::runInfections(vector <int> new1, int variant)
{
	for(int x = 0; x < new1.size(); x++)
	{
		if(state[x] == 0 && new1[x] == 1)
		{
			infect(x, variant);
		}
	}
	return(0);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
int Graph::recoveries()
{
	double randVar = 0;
	for(int x = 0; x < state.size(); x++)
	{
		if(state[x] >= 1)
		{
			randVar = (rand()%100)/100.0;
			if(randVar < recProb)
			{
				unInfect(x);
			}
		}
	}
	return(0);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
int Graph::death(int node)
{
    if ((node < 0) || (node >= nn))
	{
        cout << "Error. Node outside Bounds.\n";
        return(2);
	}
	if(state[node] >= 1)
	{
		state[node] = -2;
		infected[node] = 0;
		deathcount++;
	}
	return(0);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
int Graph::ImmunityDecay()
{
	int size = 0;
	double rand1 = 0.0;
	for(int x = 0; x < nn; x++)
	{
		if((variantHistory[x].size() > 0) && (state[x] < 1) && (state[x] != -2)) //maybe remove (state < 1) unless variantHistory.size*( > 2)
		{
			rand1 = (rand()%100)/100.0;
			if(rand1 < decProb)
			{
				removeImmunity(x);
			}
		}
	}
	return(0);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
vector <int> Graph::runVariant(int variant)
{
	vector <int> newInfections;
	double rands = 0.0;
	vector <int> Ac = calcAc(variant);
	vector <double> infectProb = infectionProb(Ac, variant);
	for(int x = 0; x < nn; x++)
	{
		rands = (rand()%100)/100.0;
		if(rands < infectProb[x])
		{
			newInfections.push_back(1);
		}
		else
		{
			newInfections.push_back(0);
		}
	}
	return(newInfections);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
int Graph::updateInfected()
{
	vector < vector <int> > infectionListList;
	vector <int> holder;
	for(int x = 0; x < VariantDic.size(); x++)
	{
		holder = runVariant(x);
		ImmunityDecay();
		recoveries();
		kill();
		runInfections(holder, x);
	}
	return(0);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
void Graph::printAdj()
{
	for(int x = 0; x < adj.size(); x++)
	{
		for(int y = 0; y < adj[x].size(); y++)
		{
			cout << adj[x][y] << " ";
		}
		cout << "\n";
	}
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
int Graph::newVariant(vector <int> bits)
{
	int counter = 0;
	vector <int> holder;
	int worked = 1;
	for(int x = 0; x < immunitySize; x++)
	{
		holder.push_back(bits[x]);
	}
	VariantDic.push_back(holder);
	int node = rand()%nn;
	worked = infect(node, VariantDic.size()-1);
	while(worked > 0)
	{
		node = rand()%nn;
		worked = infect(node, VariantDic.size()-1);
		counter++;
		if(counter > nn)
		{
			cout << "New Variant failed to infect.\n";
			return(1);
		}
	}
	return(0);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
void Graph::kill()
{
	double rands = 0.0;
	for(int x = 0; x < nn; x++)
	{
		if(state[x] > 0)
		{
			rands = (rand()%100)/100.0;
			if(rands < deathProb)
			{
				death(x);
			}
		}
	}
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
int tests()
{
	srand((int)time(NULL));
	Graph a;
	int bits1[] = {0,1,1,1,0,1,0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,0,1,1,1,0,1,0,1,1,0,0,1,0,1,0};
	vector <int> bits;
	std::copy(std::begin(bits1), std::end(bits1), std::back_inserter(bits));
	
	cout << "bitstring length: " << bits.size() << "\n";
	a.initialize(10, bits, 4, bits, 0.63, 0.125514, 0.006666417986643);
	//a.initialize(5, bits, 4, bits, 0.63, 0.125514, 0.5);
	a.infect(0,0);
	vector <int> cState = a.getState();
	printVector(cState);
	
	a.infect(1,0);
	cState = a.getState();
	printVector(cState);
	
	a.infect(4,0);
	cState = a.getState();
	printVector(cState);
	
	a.unInfect(0);
	cState = a.getState();
	printVector(cState);
	
	cout << "removing immnity\n";
	
	a.removeImmunity(0);
	cState = a.getState();
	printVector(cState);
	
	vector <int> Ac = a.calcAc(0);
	printVector(Ac);
	vector <double> probs = a.infectionProb(Ac, 0);
	printVector(probs);
	
	vector <int> newInfections;
	
	for(int x = 0; x < probs.size(); x++)
	{
		if(probs[x] > 0)
		{
			newInfections.push_back(1);
		}
		else
		{
			newInfections.push_back(0);
		}
	}
	cout << "NewInfections: ";
	printVector(newInfections);
	
	a.runInfections(newInfections, 0);
	cState = a.getState();
	printVector(cState);
	
	a.recoveries();
	cState = a.getState();
	printVector(cState);
	
	cout << "killing 4\n";
	a.death(4);
	cState = a.getState();
	printVector(cState);
	
	a.unInfect(1);
	cState = a.getState();
	printVector(cState);
	
	a.ImmunityDecay();
	cState = a.getState();
	printVector(cState);
	
	a.infect(2,0);
	cState = a.getState();
	printVector(cState);
	
	newInfections.clear();
	
	newInfections = a.runVariant(0);
	cState = a.getState();
	printVector(cState);
	
	a.runInfections(newInfections, 0);
	cState = a.getState();
	printVector(cState);
	
	a.unInfect(0);
	cState = a.getState();
	printVector(cState);
	
	a.removeImmunity(0);
	cState = a.getState();
	printVector(cState);
	
	a.updateInfected();
	cState = a.getState();
	printVector(cState);
	
	a.printAdj();
	
	int bits2[] = {1,0,0,1,1,1};
	std::copy(std::begin(bits2), std::end(bits2), std::back_inserter(bits));
	
	a.newVariant(bits);
	cState = a.getState();
	printVector(cState);
	
	return(0);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
vector <int> Graph::nextTimeStep()
{
	//cout << "Inside nextTimeStep\n";
	vector <int> stats;
	int deaths = 0;
	int life = 0;
	vector <int> infected;
	int sumInf = 0;
	updateInfected();
	//cout << "infected updated\n";
	infected = getInfected();
	//cout << "infected aquired\n";
	//printVector(infected);
	for(int x = 0; x < infected.size(); x++)
	{
		sumInf += infected[x];
	}
	
	deaths = deathCount();
	life = lifeCount();
	
	stats.push_back(sumInf);
	stats.push_back(deaths);
	stats.push_back(life);
	
	return(stats);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
vector <int> makeNewVariant(vector <int> bits, int length)
{
	int rands = rand()%(bits.size()-length);
	vector <int> output;
	for(int x = rands; x < rands + length; x++)
	{
		output.push_back(bits[x]);
	}
	return(output);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
int Graph::numOfVariants()
{
	return(VariantDic.size());
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
int main()
{
	//tests();
	int newVariantsFlag = 0;
	srand((int)time(NULL));
	std::vector <int>  listOfPoints;
	ReadData("bs_test.txt", &listOfPoints, 100000);
	//printVector(listOfPoints);
	
	vector <int> infectedLog;
	vector <int> deathLog;
	vector <int> lifeLog;
	
	int immunityLength = 7;
	
	Graph a;
	a.initialize(256, listOfPoints, immunityLength, listOfPoints, 0.63, 0.126, 0.006666);
	//a.printAdj();
	a.infect(0,0);
	vector <int> infected;
	vector <int> states;
	infected = a.getInfected();
	//printVector(infected);
	int max_timeSteps = 1000;
	int count = 0;
	double variantProb = 0.0001;
	
	vector <int> stats;
	stats = a.nextTimeStep();
	printVector(stats);
	infectedLog.push_back(stats[0]);
	deathLog.push_back(stats[1]);
	lifeLog.push_back(stats[2]);
	double variantTest = 0.0;
	
	std::vector <int> variant2;
	
	while((stats[0] > 0) && (count < max_timeSteps))
	{
		if(newVariantsFlag == 1)
		{
			variantTest = (rand()%100)/100.0;
			if(variantTest < variantProb)
			{
				cout << "Adding New Variant\n";
				variant2 = makeNewVariant(listOfPoints, immunityLength);
				a.newVariant(variant2);
			}
		}
		stats = a.nextTimeStep();
		//printVector(stats);
		states = a.getState();
		//cout << "\n";
		//printVector(states);
		stats = a.nextTimeStep();
		infectedLog.push_back(stats[0]);
		deathLog.push_back(stats[1]);
		lifeLog.push_back(stats[2]);
		count++;
	}
	int final1 = a.numOfVariants();
	cout << final1 << "\n";
	writeToFile(infectedLog, "InfectedLog.txt");
	writeToFile(deathLog, "DeathLog.txt");
	writeToFile(lifeLog, "LifeLog.txt");
	return(0);
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
void ReadData(string input1, std::vector <int> * listOfPoints, int linesize)
{//read in the data

	fstream input;
	char buf1[linesize];
	string buf2;
    string line;
	
	string inputFile = input1;
	
    std::vector<int> out;
	input.open(inputFile,ios::in);
	input.getline(buf1, linesize); //Get first line
    line = buf1;
    tokenize(line, '\t', &out);
    (*listOfPoints) = out;
	input.close();
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
void tokenize(std::string const str, const char delim, std::vector<int> * out)
{
	std::stringstream ss(str);
	std::string s;
	std::vector<string> temp;
	while(std::getline(ss, s, delim))
	{
		temp.push_back(s);
	}
	
	std::vector<string>:: iterator it;  
    for(it = temp.begin(); it != temp.end(); ++it) 
	{
		out->push_back(stod(*it));
	}
}

/**
 * Adds a new variant to the simulation.
 *
 * @param bits  Integer vector representing the immunity string for the new variant.
 * @param len  Length of immunity string. 
 * @return          If the program completes successfully, returns 0.
 */
void writeToFile(vector <int> a, string filename)
{
    fstream aus;  //output file
    aus.open(filename,ios::out);
    aus.precision(10);
       
    for(int i = 0; i < a.size(); i++)
    {
        aus << a[i] << "\n";
    }
    aus.close();
}