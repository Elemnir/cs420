#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <random>

using namespace std;

vector<vector<int> > patterns; //random patterns for testing the network
vector<int> neurons; //the network, pretty simple, ain't it? 
vector<vector<double> > weights; //weight matrix for the network
map<int, double> avgPercentUnstable; //final output for graph 1
map<int, double> avgNumberStable; //final output for graph 2

void imprintPatterns(int p)
{
	double sum;
	for (int i = 0; i < neurons.size(); ++i)
	{
		for (int j = 0; j < neurons.size(); ++j)
		{
			sum = 0.0f;
			if (i != j)	
			{
				for (int k = 0; k < p; ++k)
					sum += patterns[k][i] * patterns[k][j];
			}
			weights[i][j] = sum / neurons.size();
		}
	}
}

void testPatterns(int p)
{
	double sum;
	int unstableCount = 0, sprime;
	for (int k = 0; k < p; ++k)
	{
		neurons = patterns[k];
		for (int i = 0; i < neurons.size(); ++i)
		{
			sum = 0.0f;
			for (int j = 0; j < neurons.size(); ++j)
				sum += weights[i][j] * neurons[j];

			sprime = (sum < 0.0f) ? -1.0f : 1.0f;
			if (neurons[i] != sprime)
			{
				unstableCount++;
				break;
			}
		}
	}
	cout << "Unstable: " << unstableCount << endl;
	avgPercentUnstable[p] += (double(unstableCount) / p);
	avgNumberStable[p] += p - unstableCount;
}

int main(int argc, char** argv)
{
	mt19937 rng(random_device{}()); //constructs the random number generator
	discrete_distribution<int> dist {1, 0, 1}; //generates 0's and 2's
	weights.resize(100, vector<double>(100, 0.0f));
	
	int numruns = 1; //take as argument later
	for (int i = 0; i < numruns; i++)
	{
		//initialize the pattern set
		//patterns = vector<vector<int> >(50, vector<int>(100, dist(rng) - 1));
		patterns.clear();
		for (int x = 0; x < 50; ++x)
		{
			patterns.emplace_back(100);
			for (int y = 0; y < patterns.back().size(); ++y)
				patterns.back().at(y) = dist(rng) - 1;
		}
		
		for (int p = 1; p <= patterns.size(); ++p)
		{
			imprintPatterns(p);
			testPatterns(p);
		}
	}


	for (int i = 1; i <= 50; ++i)
	{
		avgPercentUnstable[i] /= numruns;
		avgNumberStable[i] /= numruns;
	}

	ofstream graph1("graph1.csv");
	for (auto im = avgPercentUnstable.begin(); im != avgPercentUnstable.end(); ++im)
		graph1 << im->first << ", " << im->second << endl;

	ofstream graph2("graph2.csv");
	for (auto im = avgNumberStable.begin(); im != avgNumberStable.end(); ++im)
		graph2 << im->first << ", " << im->second << endl;
}	
