//blondieExperiment1SpringBreak
//To run the first blondie24 experiment over spring break
	//to get errors early rather than later with NN. 
//3-12-18

#include <iostream>
using std::cout; 
using std::endl; 
#include <vector>
using std::vector; 
#include "neuralNetwork.h"
#include <boost/filesystem.hpp> 
#include "playgame.h"

int main(){
	vector<int> nodes{32, 40, 10, 1}; 
	NN blondie24(nodes, "blondie");  

	//Population = 30 
	int population = 30; 
	vector<NN> NeuralNets(population, blondie24);
	for(int i=0; i<population; ++i){
		NeuralNets[i] = NN(nodes, "blondie_" + to_string(i));

		//First generation 
		NeuralNets[i].saveToFile()
	}

	while(true){

		//For choosing random opponent within NNs 
		std::mt19937_64 gen(time(0));
		std::uniform_int_distribution<double> dis(0,NeuralNets.size());

		//150 game tournament = 30 NNs * 5 games each 
		int gamesPerNetwork = 5; 
		for(int i=0; i<NeuralNets.size(); ++i){
			//Each NN playes 5 games as Red selecting 
				//random NN opponent

			for(int games=0; games<gamesPerNetwork; ++games){
				//Choose random opponent
				int randIndex = dis(gen); 
				//Play a game 
				playGame(NeuralNets[i], NeuralNets[randIndex]); 
			}

		}

		//Tournament ended, choose the top 15 Neural Networks that
			//will generate children 
		//Sort the vector by wins 

		//First 15 will generate children overtop the last 15

	}

	return 0; 
}