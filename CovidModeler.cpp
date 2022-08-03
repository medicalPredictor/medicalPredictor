#include <cmath>
#include <list> 
#include <sstream>
#include <vector>
#include <iostream>
#include <time.h>

using namespace std;

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

int Graph::initialize(int nn1, vector <int> bitspray, int immunityStringSize, vector <int> bitspray2, double alpha1, double recProb1, double decProb1)
{
	nn = nn1;
	//cout << "numNodes: " << nn << "\n";
	int counter = 0;
	vector <int> holder;
	for(int x = 0; x < nn; x++)
	{
		holder.push_back(0);
		//cout << holder[holder.size()-1] << " ";
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
			//cout << adj[x][y] << " ";
			adj[x][y] = bitspray[counter];
			adj[y][x] = bitspray[counter];
			counter++;
		}
		//cout << "\n";
	}
	//cout << "Counter: " << counter << "\n";
	
	// for(int x = 0; x < adj.size(); x++)
// 	{
// 		for(int y = 0; y < adj[x].size(); y++)
// 		{
// 			cout << adj[x][y] << " ";
// 			//adj[x][y] = bitspray[counter];
// 			//counter++;
// 		}
// 		cout << "\n";
// 	}
// 	cout << "State:\n";
// 	for(int x = 0; x < nn; x++)
// 	{
// 		cout << state[x] << " ";
// 	}
// 	cout << "\n";
// 	cout << "Infected:\n";
// 	for(int x = 0; x < nn; x++)
// 	{
// 		cout << infected[x] << " ";
// 	}
// 	cout << "\n";
	alpha = alpha1;
	recProb = recProb1;
	decProb = decProb1;
	deathProb = 0.00168214;
	
	//cout << alpha << " " << recProb << " " << decProb << " " << deathProb << "\n";
	
	immunitySize = immunityStringSize;
	addVariant(bitspray2, immunitySize);
	//printVariantList();
	
	vector <int> holder2;
	for(int y = 0; y < immunitySize; y++)
	{
		holder2.push_back(0);
	}
	
	for(int x = 0; x < nn; x++)
	{
		immunityDic.push_back(holder2);
	}
	
	//printImmunityList();
	
	for(int x = holder.size(); x > 0; x--)
	{
		holder.pop_back();
	}
	for(int x = 0; x < nn; x++)
	{
		variantHistory.push_back(holder);
	}
	//cout << variantHistory.size() << "\n";
	
	deathcount = 0;
	
	//printVariantHistory();
	
	return(0);
}

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

int printVector(vector <int> vec)
{
	for(int y = 0; y < vec.size(); y++)
	{
		cout << vec[y] << " ";
	}
	cout << "\n";
	return(0);
}

int printVector(vector <double> vec)
{
	for(int y = 0; y < vec.size(); y++)
	{
		cout << vec[y] << " ";
	}
	cout << "\n";
	return(0);
}

vector <int> Graph::getState()
{
	return(state);
}

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
			//printVariantList();
			//printImmunityList();
			return(0);
		}
	}
	return(1);
}

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

int Graph::removeImmunity(int node)
{
	int variant = 0;
	if(node < 0 | node >= nn)
	{
		cout << "Error. Node outside Bounds.\n";
		return(2);
	}
	if(state[node] == -1) //(maybe remove this?)
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

double Graph::calcExtra(vector <int> Ac, int x, int totalBits, int variant)
{
	if(Ac[x] == 0)
	{
		return(0.0);
	}
	//vector <int> vec;
	int mismatches = 0;
	if(Ac[x] > 1)
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
	//cout << "mismatches: ";
	//cout << mismatches << "\n";
	return(mismatches/totalBits);
}

vector <double> Graph::infectionProb(vector <int> Ac, int variant)
{
	vector <double> probs;
	vector <double> extra;
	vector <double> extra2;
	int totalBits = 0;
	for(int x = 0; x < VariantDic[variant].size(); x++)
	{
		totalBits = totalBits + VariantDic[variant][x];
		//cout << VariantDic[variant][x] << " ";
	}
	//cout << "\n";
	//cout << totalBits << "\n";
	double a = 0.0;
	for (int x = 0; x < Ac.size(); x++)
	{
		a = calcExtra(Ac, x, totalBits, variant);
		//cout << a << "\n";
		extra.push_back(a);
	}
	
	double b = log(1- alpha);
	//cout << "\n" << b << "\n";
	for(int x = 0; x < Ac.size(); x++)
	{
		extra2.push_back(Ac[x]*b);
	}
	
	//printVector(extra2);
	
	for(int x = 0; x < Ac.size(); x++)
	{
		extra2[x] = 1 - exp(extra2[x]);
	}
	//cout << "extra2 second time\n";
	//printVector(extra2);
	
	for(int x = 0; x < Ac.size(); x++)
	{
		probs.push_back(extra2[x]*extra[x]);
	}
	//printVector(probs);
	
	return(probs);
}

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

int Graph::recoveries()
{
	double randVar = 0;
	for(int x = 0; x < state.size(); x++)
	{
		if(state[x] >= 1)
		{
			randVar = (rand()%100)/100.0;
			//cout << randVar << "\n";
			if(randVar < recProb)
			{
				unInfect(x);
			}
		}
	}
	return(0);
}

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

int Graph::ImmunityDecay()
{
	
	return(0);
}


int main()
{
	srand((int)time(NULL));
	Graph a;
	int bits1[] = {0,1,1,1,0,1,0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,0,1,1,1,0,1,0,1,1,0,0,1,0,1,0};
	vector <int> bits;
	std::copy(std::begin(bits1), std::end(bits1), std::back_inserter(bits));
	cout << "bitstring length: " << bits.size() << "\n";
	a.initialize(5, bits, 4, bits, 0.63, 0.125514, 0.006666417986643);
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
	
	a.death(4);
	cState = a.getState();
	printVector(cState);
	
	a.ImmunityDecay();
	cState = a.getState();
	printVector(cState);
	
	return(0);
}

