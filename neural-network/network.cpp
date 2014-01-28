#include <iostream>
#include <fstream>
#include <vector>
#include <random>

#include "neuron.hpp"

using namespace std;

int main(int argc, char** argv)
{
	string paramFileName;
	if (argc == 2)
		paramFileName = argv[1];
	else
		paramFileName = "config.param";
	
	ifstream paramfile(paramFileName.c_str());

	if (paramfile.fail()) {
		cerr << "Configuration file '" << paramFileName << "' is missing,\n" 
			 << "use '$> make param' to generate the default 'config.param'\n";
		return -1;
	}

	int numLayers, numEpochs;
	double learnRate;
	string arg, trainingName, testingName, validationName;
	vector<vector<Neuron<double> > > network;

	while (paramfile >> arg)
	{
		if (arg == "//") {
			getline(paramfile, arg);
		}
		else if (arg == "num_layers") {
			paramfile >> numLayers;
			network.resize(numLayers);
		}
		else if (arg == "neurons_per_layer") {
			int neurons_per_layer;
			for (int i = 0; i < network.size(); ++i) {
				paramfile >> neurons_per_layer;
				network[i].resize(neurons_per_layer);
			}
			getline(paramfile, arg);
		}
		else if (arg == "learning_rate") {
			paramfile >> learnRate;
		}
		else if (arg == "num_epochs") {
			paramfile >> numEpochs;
		}
		else if (arg == "training_file_name") {
			paramfile >> trainingName;
		}
		else if (arg == "testing_file_name") {
			paramfile >> testingName;
		}
		else if (arg == "validation_file_name") {
			paramfile >> validationName;
		}
	}

	cout << "Number of layers: " << network.size() << endl;
	
	cout << "Neurons per layer: ";
	for (int i = 0; i < network.size(); ++i) { 
		cout << network[i].size() << " ";
	}
	cout << endl;

	cout << "Number of epochs: " << numEpochs << endl;
	cout << "Learning rate: " << learnRate << endl;
	
	mt19937 rng(random_device{}());
	uniform_real_distribution<double> dist(-1.0,1.0);

	for (int i = 1; i < network.size(); ++i) {
		for (int j = 0; j < network[i].size(); ++j) {
			for (int k = 0; k < network[i-1].size(); ++k) {
				network[i][j].inputs.push_back(&network[i-1][k]);
				network[i][j].weights.push_back(dist(rng));
			}
		}
	}
	
	return 0;
}
