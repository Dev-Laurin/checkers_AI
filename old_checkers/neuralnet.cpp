/*
neuralnet.cpp
Jason Bright
Neural Net program
20160215 

*/

#include "neuralnet.h"

using std::cout;
using std::endl;

double sigmoid(double x, double b) {
	static const double a = 2.0;  //Gives a range of -1 to 1 after sigmoid is applied.
//	static const double b = 4.0;
	static const double c = -1.0;
	return ((a/(1+std::exp((-x)/b)))+c);
}

//Ctor is blank; not sure why I had to write it.
NeuralNet::NeuralNet() {
}

// Allocate memory for the net
NeuralNet::NeuralNet(int layers, int neurons[]) {
	Nlayers = layers;
	neuronCount.resize(layers);
	NeuralLayer.resize(layers);
	for(int x = 0; x < layers; ++x) { //Allocate space for the neural layers.
		neuronCount[x] = neurons[x];
		NeuralLayer[x].resize(neurons[x]); //Set the size.
	}
	Dendrites.resize(layers-1);
	DendriteStability.resize(layers-1);
	for(int x = 0; x < layers -1; ++x) { //Create the links
		NumFact += neurons[x]*neurons[x+1];
		Dendrites[x].resize(neurons[x]*neurons[x+1]); //Here's where the space gets used.
		DendriteStability[x].resize(neurons[x]*neurons[x+1]); //Here's where the space gets used.
	}
}

//Fill the Dendrites with random values
void NeuralNet::RandomizeDendrites() {
	std::random_device rd;  //Note:  Change this out and one can have full repeatability.
	std::mt19937_64 gen(rd()); //use a decent random number
	std::uniform_real_distribution<double> d(-1,1); //flat distribution - changed from standard(bell curve)
	for(int x = 0;x < Dendrites.size();++x) {
		for(int y = 0; y < Dendrites[x].size(); ++y) {
			Dendrites[x][y] = d(gen);
			DendriteStability[x][y] = INITMUTATE; //Stores the initial mutate factor.
		}
	}
	KingValue = INITKINGVAL;
	KingMutate = INITMUTATE;
	PieceCount = INITPIECEVAL;
	PieceMutate = INITMUTATE;
}

//Sets the inputs of the neural net
//Note:  Consider making the king values variable.
void NeuralNet::setInputs(char sBoard[]) {
	PieceCount = 0;
	for(int x = 0; x < BOARDSIZE; ++x) {
		switch (sBoard[x]) {
			case REDPAWN:
				NeuralLayer[0][x] = -1;
				PieceCount += -1;
				break;
			case BLACKPAWN:
				NeuralLayer[0][x] = 1;
				PieceCount += 1;
				break;
			case REDKING:
				NeuralLayer[0][x] = -2;
				PieceCount -= KingValue;
				break;
			case BLACKKING:
				NeuralLayer[0][x] = 2;
				PieceCount += KingValue;
				break;
			default:
				NeuralLayer[0][x] = 0;
				break;			
		}
	}
}
//Impulse is where you actually figure out the value.
//Note:  Actually faster than the 'fast' example!
//2.3 seconds for 1M calcs vs 3.4

NETPRECISION NeuralNet::Impulse() {
	for(size_t x = 0; x < Dendrites.size();++x) { //Work through each layer of dendrites
		for(size_t y = 0; y < neuronCount[x+1];++y) {
			NeuralLayer[x+1][y] = 0; //Reset it to zero
		}
		for(size_t y = 0; y < NeuralLayer[x].size();++y) { //Work through the start layer of neurons.
			for(size_t z = 0; z < NeuralLayer[x+1].size();++z) { //Work through the next layer of neurons.
					NeuralLayer[x+1][z] += (NeuralLayer[x][y]*Dendrites[x][y*neuronCount[x+1]+z]);				
			}
		}
		//I think that by applying the sigmoid AFTER calculating everything else, I'm reducing cache misses.
		for(size_t y = 0; y < neuronCount[x+1];++y) {
			NeuralLayer[x+1][y] = sigmoid(NeuralLayer[x+1][y], NeuralLayer[x].size());
			if (NeuralLayer[x+1][y] != NeuralLayer[x+1][y]) { //NaN check
				NeuralLayer[x+1][y] = 0;
			}
		}
	}
/*	for(size_t x = 0; x < Dendrites.size();++x) { //Work through each layer of dendrites
		for(size_t y = 0; y < neuronCount[x+1];++y) {
			cout << NeuralLayer[x][y] << " ";
		}
		cout << endl;
	}
*/

	return NeuralLayer[Nlayers-1][0] + PieceCount;
}


//Not sure this is calculating it correctly, but is also slower.
/*
NETPRECISION NeuralNet::Impulse() {
	for(size_t x = 0; x < Dendrites.size(); ++x) {
		for(size_t y = 0; y < neuronCount[x+1];++y) {
			double temp = 0.0;
			for(size_t z = 0;z<neuronCount[x];++z) {
				temp+=NeuralLayer[x][z] * Dendrites[x][y*neuronCount[x+1]+z];
			}
				NeuralLayer[x+1][y] = sigmoid(temp); // triples my run time!
//				NeuralLayer[x+1][y] = temp;
		}
	}
}
*/

void NeuralNet::Print(std::string fileName) {
	std::ofstream OutFile (fileName.c_str(),std::ofstream::out);
//Print the Neural Layers
	OutFile << "[";
	for(int x = 0;x < Nlayers-1;++x) {
		OutFile << neuronCount[x] << ',' ;
	}
	OutFile << neuronCount[Nlayers-1] << ']' << std::endl;
	//Print the Dendrites
	OutFile << '[';
	for(int x = 0;x < Dendrites.size();++x) {
		for(int y = 0;y < Dendrites[x].size();++y) {
			OutFile << Dendrites[x][y] << ',';
		}
	}
	size_t filepos = OutFile.tellp();
	OutFile.seekp(filepos-1); //Move back 1 character to fix input.
	OutFile << ']' << std::endl;
	//Current board
	OutFile << '[';
	for(int x = 0; x < NeuralLayer[0].size();++x) {
		OutFile << NeuralLayer[0][x] << ',';
	}
	filepos = OutFile.tellp();
	OutFile.seekp(filepos-1); //Move back 1 character to fix input.
	OutFile << ']' << std::endl;
	//Result for current board.
	OutFile << Impulse() << std::endl; //Punch in the result for the current board
	//Print the Mutations
	OutFile << '[';
	for(int x = 0;x < DendriteStability.size();++x) {
		for(int y = 0;y < DendriteStability[x].size();++y) {
			OutFile << DendriteStability[x][y] << ',';
		}
	}
	filepos = OutFile.tellp();
	OutFile.seekp(filepos-1); //Move back 1 character to fix input.
	OutFile << ']' << std::endl;
	OutFile << "King Value: " << KingValue << " Mutate: " << KingMutate << std::endl;
	OutFile << "PieceValue: " << PieceValue << " Mutate: " << PieceMutate << std::endl;
	OutFile.close();
}

//Save the neural network to file
bool NeuralNet::Save (std::string fileName) {
	if (fileName == "") { //build default string
		std::stringstream ss;
		ss << "Brain_" << Generation << "_" << Place <<".NN";
		fileName = ss.str();
	}
	try {
		std::ofstream OutFile (fileName.c_str(),std::ofstream::out | std::ofstream::binary);
		OutFile.write((char *) & Generation, sizeof(Generation));
		OutFile.write((char *) & Nlayers, sizeof(Nlayers));
		OutFile.write((char *) & neuronCount[0], sizeof(neuronCount[0])*neuronCount.size());
		for(int i = 0;i<Dendrites.size();++i) {
			OutFile.write((char *) &Dendrites[i][0], sizeof(Dendrites[0][0])*Dendrites[i].size());
			OutFile.write((char *) &DendriteStability[i][0], sizeof(DendriteStability[0][0])*DendriteStability[i].size());
		}
		OutFile.write((char*) & KingValue, sizeof(KingValue));
		OutFile.write((char*) & KingMutate, sizeof(KingMutate));
		OutFile.write((char*) & PieceValue, sizeof(PieceValue));
		OutFile.write((char*) & PieceMutate, sizeof(PieceMutate));
		
		OutFile.close();
	return true;
	}
	catch(...) {
		return false;
	}
}
bool NeuralNet::Load (std::string fileName) {
	if (fileName == "") { //build default string
		std::stringstream ss;
		ss << "Brain_" << Generation << "_" << Place <<".NN";
		fileName = ss.str();
	}
	try {
		std::ifstream InFile (fileName.c_str(),std::ifstream::in | std::ifstream::binary);
		InFile.read((char *) & Generation, sizeof(Generation));
		InFile.read((char *) & Nlayers, sizeof(Nlayers));
		neuronCount.resize(Nlayers);
		InFile.read((char *) & neuronCount[0], sizeof(neuronCount[0])*neuronCount.size());
		NeuralLayer.resize(Nlayers);
		for(int i = 0;i < Nlayers;++i) {
			NeuralLayer[i].resize(neuronCount[i]);
		}
		Dendrites.resize(Nlayers-1);
		DendriteStability.resize(Nlayers-1);
		for(int i = 0;i<Dendrites.size();++i) {
			NumFact += neuronCount[i]*neuronCount[i+1];
			Dendrites[i].resize(neuronCount[i]*neuronCount[i+1]);
			DendriteStability[i].resize(neuronCount[i]*neuronCount[i+1]);
			InFile.read((char *) &Dendrites[i][0], sizeof(Dendrites[0][0])*Dendrites[i].size());
			InFile.read((char *) &DendriteStability[i][0], sizeof(DendriteStability[0][0])*DendriteStability[i].size());
		}
		InFile.read((char*) & KingValue, sizeof(KingValue));
		InFile.read((char*) & KingMutate, sizeof(KingMutate));
		InFile.read((char*) & PieceValue, sizeof(PieceValue));
		InFile.read((char*) & PieceMutate, sizeof(PieceMutate));
		InFile.close();
		Score = 0;
	return true;
	}
	catch(...) {
		return false;
	}
}
	
void NeuralNet::Evolve(NETPRECISION mutateFactor) { //mutateFactor allows overriding of the mutation factor
	std::random_device rd;
	std::mt19937_64 gen(rd()); //use a decent random number
	NETPRECISION tau = 1/std::sqrt(2*std::sqrt(NumFact));
	std::normal_distribution<> d(0,mutateFactor); //distribution
	for(int x = 0;x < Dendrites.size();++x) {
		for(int y = 0; y < Dendrites[x].size(); ++y) {
			DendriteStability[x][y] *= std::exp(tau * d(gen));
//			cout << DendriteStability[x][y] << endl;
			Dendrites[x][y] += DendriteStability[x][y] * d(gen);
		}
	}
// Ah, the fund.
	KingMutate *= std::exp(tau * d(gen));
	KingValue += KingMutate * d(gen);
	PieceMutate *= std::exp(tau * d(gen));
	PieceValue += PieceMutate * d(gen);
	cout << "KM: " << KingMutate << " KV: " << KingValue << " PM: " << PieceMutate << " PV: " << PieceValue << endl;
	++Generation; //It's now the next generation.
}