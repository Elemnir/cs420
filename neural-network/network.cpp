#include <iostream>
#include <fstream>
#include <vector>

#include "neuron.inl"

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
		cerr << "File configuration file '" << paramFileName << "' is missing, " 
			 << "use '$> make param' to generate the default\n";
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
			for (auto i : network) {
				paramfile >> neurons_per_layer;
				i.resize(neurons_per_layer);
			}
		}
		else if (arg == "learing_rate") {
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

	cout << "Number of layers: " << numLayers << endl;
	cout << "Neurons per layer: ";
	for (auto i : network) cout << i.size() << " " << endl;
	cout << "Number of epochs: " << numEpochs << endl;
	cout << "Learning rate: " << learnRate << endl;
	cout << "File names:\n" 
		 << trainingName << endl
		 << testingName << endl
		 << validationName << endl;
	
	return 0;
}
