#include <iostream>
#include <vector>
#include <random>

using namespace std;

vector<vector<int> > patterns; //random patterns for testing the network
vector<int> neurons; //the network, pretty simple, ain't it? 
vector<vector<double> > weights; //weight matrix for the network
vector<int> stable; //number of stable inputs for a given number of patterns
vector<double> avgPercentUnstable; //final output for graph 1
vector<double> avgNumberStable; //final output for graph 2

void imprintPatterns(int p)
{
	double sum = 0.0f;
	for (int i = 0; i < neurons.size(); ++i)
		for (int j = 0; j < neurons.size(); ++j)
		{
			if (i != j)	
				for (int k = 0; k < p && k < patterns.size(); ++k)
					sum += patterns[k][i] * patterns[k][j];
			weights[i][j] = sum / neurons.size();
		}
}

void testPatterns(int p)
{
	double sum;
	for (int k = 0; k < p && k < patterns.size(); ++k)
	{
		neurons = patterns[k];
		for (int i = 0; i < neurons.size(); ++i)
		{
			sum = 0.0f;
			for (int j = 0; j < patterns.size(); ++j)
				sum += weights[i][j] * patterns[j];

			neurons[i] = (sum < 0.0f) ? -1.0f : 1.0f;
		}
		if (neurons == patterns[k])
			stable[p - 1] += 1;
	}
}

void resetWeights()
{
	for (int i = 0; i < weights.size(); ++i)
		for (int j = 0; j < weights[i].size(); ++j)
			weights[i][j] = 0.0f;
}

int main(int argc, char** argv)
{
	//C++ is wizardry. All shall bow before its mighty random number generation
	mt19937 rng(random_device{}()); //constructs the random number generator
	discrete_distribution<int> dist {1, 0, 1}; //generates 0's and 2's
	weights.resize(50, vector<double>(50, 0.0f));
	
	int numruns = 1; //take as argument later
	for (int i = 0; i < numruns; i++)
	{
		//initialize the pattern set
		patterns = vector<vector<int> >(50, vector<int>(100, dist(rng) - 1));
		for (int p = 1; p <= patterns.size(); ++p)
		{
			imprintPatterns(p);
		}
	}
}
