/*
NeuralNetTest.cpp
Jason Bright
Neural Net Test program
20160215

*/

#include "neuralnet.h"
#define RUNS_PER 1000000

using std::cout;
using std::endl;
//Mode of arguments: 
//Number of layers [layersize] [number of networks] [directory]
int main(int argc, char * argv[]) {
	std::stringstream Input(argv[1]);
	int NLayers;  //Number of layers to expect.
	if (!(Input >> NLayers)) {
		std::cout << "Invalid Neural Depth!" << std::endl;
		return 1; //
	}
	std::cout << "Neural depth: " << NLayers << std::endl;
	int NLayerSize[NLayers]; //Stores the #layers expected

	for(int x = 0;x < NLayers; ++x) {
		NLayerSize[x] = atoi (argv[x+2]);
		if (NLayerSize[x]<1) {
			std::cout << "Invalid input!" << std::endl;
			return 1;
		}
	}
	
	int BrainCount = atoi(argv[NLayers+2]);
	cout << "Number of Brains to generate: " << BrainCount << endl;
	NeuralNet BrainX (NLayers,NLayerSize);
	for (int i = 0;i<BrainCount;++i) {
		std::stringstream fileName;
		fileName << argv[NLayers+3] << "/brain" << i << ".NN";
		BrainX.RandomizeDendrites();
		BrainX.Save(fileName.str());
	}
	return 0;
}
	/*
	//testing entry method:
	for(int x = 0;x < NLayers; ++x)
		std::cout << "Layer " << x+1 << " Size " << NLayerSize[x] << std::endl;
	
	//Make the Neural net
	NeuralNet BrainX (NLayers,NLayerSize);
	BrainX.RandomizeDendrites(); //put random stuff in there!
	BrainX.setInputs("rrrrrrrrrrrr________bbbbbbbbbbbb");  //Initial board
	BrainX.Print("NeuralNet1.txt"); //Print the network to file in a format ready for skynet.
	BrainX.Save(); //Testing the save function
	BrainX.Load();
	NeuralNet BrainX2 = BrainX;
	BrainX2.Evolve();
	BrainX2.Print("NeuralNet2.txt");
	BrainX.Print("NeuralNet1b.txt");
	BrainX2.Save();
	srand(time(0)); //randomize seed
	clock_t time_start;
	clock_t time_end;
	time_start = clock();
	for (int x = 0; x < RUNS_PER;++x) {
		BrainX.NeuralLayer[0][rand()%32] = (rand() % 5 - 2); //Mixes up the board.
//		std::cout << "Impulse Results: " << BrainX.Impulse() << std::endl;
		BrainX.Impulse();
		}
	time_end = clock();
	std::cout << "It took " << (((double)(time_end-time_start))/CLOCKS_PER_SEC) << " seconds to run " << RUNS_PER << " times" << std::endl;
	return 0;
}
*/