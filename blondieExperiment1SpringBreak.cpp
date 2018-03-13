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
#include <algorithm> 

int main(){
	vector<int> nodes{32, 40, 10, 1}; 
	NN blondie24(nodes, "blondie");  

	//file variables
	string parentDirectory = "NeuralNetworkFiles/";
	boost::filesystem::path current_path(boost::filesystem::current_path()); 


	//Population = 30 
	int population = 30; 
	vector<NN> NeuralNets(population, blondie24);
	for(int i=0; i<population; ++i){
		NeuralNets[i] = NN(nodes, "blondie");

		//First generation 
		string initialPath = parentDirectory + NeuralNets[i].familyName + 
			"/" + "GEN" + to_string(NeuralNets[i].generation) + 
			"/"; 
		boost::filesystem::path path = current_path; 
		string slash  = "/" + initialPath; 
		path += slash.c_str(); 
		boost::filesystem::path dir(path);
		boost::filesystem::create_directories(dir);
		if(boost::filesystem::is_directory(dir)){
			NeuralNets[i].saveToFile(initialPath + "NN" + to_string(i));
		}
		else{
			cout << "Error, directory not created in "; 
			cout << "before-tournament." << endl;
			cout << path << endl;
		}
		 
	}

	while(true){

		//For choosing random opponent within NNs 
		std::mt19937_64 gen(time(0));
		std::uniform_int_distribution<int> dis(0,NeuralNets.size());

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
		std::sort(NeuralNets.begin(), NeuralNets.end(), 
			[](const NN & a, const NN & b) -> bool{
				return a.wins < b.wins; 
			}); 

		//Last 15 will generate children (higher wins)
			//overtop the first 15 
		for(int i=NeuralNets.size()-1; i>NeuralNets.size()/2; --i){
			//save parent to file
			boost::filesystem::path totalPath = current_path; 
			string path = parentDirectory + NeuralNets[i].familyName + 
			"/" + "GEN" + to_string(NeuralNets[i].generation) + 
			"/"; 
			string slash = "/" + path; 
			totalPath += slash.c_str(); 
			boost::filesystem::path dir(totalPath);
			boost::filesystem::create_directories(dir);

			if(boost::filesystem::is_directory(dir)){
				path += "NN" + i; 
				NeuralNets[i].saveToFile(path); 	
			}
			else{
				cout << "Path making failed." << endl;
				cout << path << endl;
			}
			
			//make loser network a child of this parent
			NeuralNets[i - NeuralNets.size()/2] = NeuralNets[i]; 
			NeuralNets[i - NeuralNets.size()/2].becomeOffspring(); 

		}

	}

	return 0; 
}