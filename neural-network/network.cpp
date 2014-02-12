#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>

#include "neuron.hpp"

using namespace std;

int main(int argc, char** argv)
{
	//open the parameter file
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

	
	//parse the parameter file to configure the network
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

	
	//build the input/output matrices from the data set files
	ifstream trainingFile(trainingName.c_str()), 
			 testingFile(testingName.c_str()), 
			 validationFile(validationName.c_str());
	if (trainingFile.fail() || testingFile.fail() || validationFile.fail()) {
		cerr << "One or more of the data set files are missing.\n";
		return -1;
	}
	
	vector<vector<double> > trainingSet, testingSet, validationSet;
	string line;
	stringstream ss;
	double val;
	while (getline(trainingFile, line)) {
		ss.clear(); ss.str(line);
		trainingSet.emplace_back();
		while (ss >> val)
			trainingSet.back().push_back(val);
	}
	
	while (getline(testingFile, line)) {
		ss.clear(); ss.str(line);
		testingSet.emplace_back();
		while (ss >> val)
			testingSet.back().push_back(val);
	}
	
	while (getline(validationFile, line)) {	
		ss.clear(); ss.str(line);
		validationSet.emplace_back();
		while (ss >> val)
			validationSet.back().push_back(val);
	}
	
	
	//print header information before starting the network simulation
	cout << "Number of layers: " << network.size() << endl;
	
	cout << "Neurons per layer: ";
	for (int i = 0; i < network.size(); ++i) { 
		cout << network[i].size() << " ";
	}
	cout << endl;

	cout << "Number of epochs: " << numEpochs << endl;
	cout << "Learning rate: " << learnRate << endl;
	

	//initialize the network and assign random weights to each neuron's inputs
	mt19937 rng(random_device{}());
	uniform_real_distribution<double> dist(-0.1,0.1);

	for (int i = 1; i < network.size(); ++i) {
		for (int j = 0; j < network[i].size(); ++j) {
			for (int k = 0; k < network[i-1].size(); ++k) {
				network[i][j].inputs.push_back(&network[i-1][k]);
				network[i-1][k].outputs.push_back(&network[i][j]);
				network[i][j].weights.push_back(dist(rng));
			}
			network[i][j].index = j;
			network[i][j].bias = dist(rng);
		}
	}
	
	double sum, rmse, mse;
	
	//loop over the training and testing data for each epoch
	for (int t = 0; t < numEpochs; ++t) 
	{
		//loop over the training set
		for (int i = 0; i < trainingSet.size(); ++i) 
		{
			for (int j = 0; j < trainingSet[i].size() - 1; ++j) 
				network[0][j].output = trainingSet[i][j];

			//calculate an output
			for (int p = 1; p < network.size(); ++p)
				for (int q = 0; q < network[p].size(); ++q)
					network[p][q].update();
			
			//calculate deltas
			for (int j = 0; j < network.back().size(); ++j)
				network.back().at(j).calcoutputdelta(trainingSet[i].back());
			
			for (int p = network.size() - 2; p >= 1; --p)
				for (int q = 0; q < network[p].size(); ++q)
					network[p][q].calcdelta();

			//update the weights
			for (int p = 1; p < network.size(); ++p)
				for (int q = 0; q < network[p].size(); ++q)
					network[p][q].apply(learnRate);
		}

		//loop over the testing set
		sum = 0.0f;
		for (int i = 0; i < testingSet.size(); ++i) 
		{
			for (int j = 0; j < testingSet[i].size() - 1; ++j) 
				network[0][j].output = testingSet[i][j];

			for (int p = 1; p < network.size(); ++p)
				for (int q = 0; q < network[p].size(); ++q)
					network[p][q].update();
			
			sum += pow((testingSet[i].back() - network.back().back().output), 2);
		}

		mse = sum / testingSet.size();
		rmse = sqrt(mse);
		cout << setw(10) << t << ", " << setprecision(10) 
			 << mse << ", " << rmse << endl;
	}


	//verify the accuracy of the network over the validation set
	sum = 0.0f;
	for (int i = 0; i < validationSet.size(); ++i)
	{
		for (int j = 0; j < validationSet[i].size() - 1; ++j)
			network[0][j].output = validationSet[i][j];

		for (int p = 1; p < network.size(); ++p)
			for (int q = 0; q < network[p].size(); ++q)
				network[p][q].update();

		sum += pow((validationSet[i].back() - network.back().back().output), 2);
	}
	mse = sum / validationSet.size();
	rmse = sqrt(mse);
	cout << setprecision(10) << mse << ", " << rmse << endl;

	return 0;
}
