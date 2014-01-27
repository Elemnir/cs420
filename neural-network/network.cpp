#include <iostream>
#include <fstream>
#include <vector>

#include "neuron.h"

using namespace std;

int main(int argc, char** argv)
{
	string paramFileName;
	if (argc == 2)
		paramFileName = argv[1];
	else
		paramFileName = "config.param";
	
	istream paramfile(paramFileName.c_str());

	if (paramfile.fail()) {
		cerr << "File configuration file '" << paramFileName << "' is missing, " 
			 << "use '$> make param' to generate the default\n"
		return -1;
	}

	int numLayers, numEpochs;
	double learnRate;
	string trainingName, testingName, validationName;
	vector<vector<Neuron> > network;

	
	return 0;
}
