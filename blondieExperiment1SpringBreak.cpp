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
			return -1; 
		}
		 
	}

	int tournamentNum = -1;  
	while(true){
		
		//For choosing random opponent within NNs 
		std::mt19937_64 gen(time(0));
		std::uniform_int_distribution<int> dis(0,NeuralNets.size());

		//150 game tournament = 30 NNs * 5 games each 
		++tournamentNum;
		int gamesPerNetwork = 5; 
		int gameNum = 0; 
		for(int i=0; i<NeuralNets.size(); ++i){
			//Each NN playes 5 games as Red selecting 
				//random NN opponent

			for(int games=0; games<gamesPerNetwork; ++games){
				//Choose random opponent
				int randIndex = dis(gen); 
				//Play a game 
				boost::filesystem::path allPath = current_path;
				string gameDirectory = parentDirectory + "TOUR" +
					to_string(tournamentNum) + "/GAME" + to_string(gameNum);
				string slash = "/" + gameDirectory; 
				allPath += slash.c_str(); 
				//Make the path 
				boost::filesystem::path dir(allPath);
				boost::filesystem::create_directories(dir);

				vector<stdBoard> gameBoards(200); 
				if(boost::filesystem::is_directory(dir)){
					++gameNum; 

					int results = playGame(NeuralNets[i], NeuralNets[randIndex], 
						gameBoards); 

					ofstream file(gameDirectory + ".txt");
					if(!file){
						cout << "Game File Error." << endl; 
						return -1; 
					}
					if(results>0){
						//NeuralNets[i] won 
						file << "winner: " << NeuralNets[i].familyName; 
						file << " " << NeuralNets[i].generation; 
						file << " " << i << endl; 

						file << NeuralNets[randIndex].familyName; 
						file << " " << NeuralNets[randIndex].generation; 
						file << " " << randIndex << endl;
					}
					else if(results<0){
						//NeuralNets[randIndex]
						file << "winner: " << NeuralNets[randIndex].familyName; 
						file << " " << NeuralNets[randIndex].generation; 
						file << " " << randIndex << endl;

						file << NeuralNets[i].familyName; 
						file << " " << NeuralNets[i].generation; 
						file << " " << i << endl;
					}
					else{
						//draw 
						file << NeuralNets[randIndex].familyName; 
						file << " " << NeuralNets[randIndex].generation; 
						file << " " << randIndex << endl;

						file << NeuralNets[i].familyName; 
						file << " " << NeuralNets[i].generation; 
						file << " " << i << endl;
					}

					//Save all the boards in the game into a file
					for(int k=0; k<gameBoards.size(); ++k){
						file << gameBoards[k].str() << endl; 
						file << endl; 
					}
					file.close(); 

				}
				else{
					cout << "Game DIR not correct." << endl;
					cout << gameDirectory << endl; 
					cout << allPath << endl;
					return -1; 
				}

				
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
				return -1; 
			}
			
			//make loser network a child of this parent
			NeuralNets[i - NeuralNets.size()/2] = NeuralNets[i]; 
			NeuralNets[i - NeuralNets.size()/2].becomeOffspring(); 

		}

	}

	return 0; 
}