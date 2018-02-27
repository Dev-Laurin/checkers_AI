//neuralNetwork.h
//Neural Network
#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <cmath>
using std::exp;

#include <random>
#include <vector>
using std::vector;

#include <chrono>
using std::time;

#include <iostream>
using std::cout;
using std::endl;
using std::getline;

#include <fstream>
using std::ofstream;
#include <string>
using std::string;
using std::to_string;
#include <sstream>
using std::istringstream;
using std::ifstream;

#include <algorithm>
using std::max;
using std::min;

#include "board.h"


unsigned const static int MAXMOVES = 16;
typedef double sNN;
constexpr sNN LOWEST = std::numeric_limits<sNN>::lowest();
constexpr sNN HIGHEST = std::numeric_limits<sNN>::max();

class AIPlayer{
public:
    double kingVal = 0.4;  //Value of kings over pawns.
    double sigma = 0.05; //How much we vary from the parent weights
    double pieceWeight = 1.0; //Value of piece counter input
    double montyWeight = 1.0;
    int montyK = 1; // (w-l)/(w+l+k)
    int generation = 0; //Starts out as parent
    string familyName = "";
    // Returns the value of the board given.
    // A virtual function to be implimented by the specific AI.
    virtual sNN calculateBoard(stdBoard & board){return 1.0;}

    // Returns a move given a board and a side.
    stdBoard getMove(stdBoard & board, bool side=false);
    // Beta side of the board
    sNN beta(stdBoard & board, int depth);
    //  Alpha side of the board
    sNN alpha(stdBoard & board, int depth);
    // Handles starting up the alpha-beta search
    sNN alphabeta(stdBoard board, unsigned int side = 0);
};

class PieceCount: public AIPlayer
{
  PieceCount() {
    distro = std::uniform_real_distribution<double>(0.0,1.0);
  }
  std::default_random_engine gen;
  std::uniform_real_distribution<double> distro;

  sNN calculateBoard(stdBoard & board) {
    sNN count = (int)(
          board.pieces[0].count() +
          board.pieces[2].count() -
          board.pieces[1].count() -
          board.pieces[3].count()) +
          distro(gen);
    return count;
  }
};

class NN: public AIPlayer
{
public:
	//Set weights to random values
	NN(std::vector<int>& nS, string familyname){
		//Save the family name (to tell apart/organize)
		familyName = familyname;

	    nodeSizes = nS;
		//Save the node configuration
		nodes.resize(nodeSizes.size());
		for(unsigned int i=0; i<nodeSizes.size(); ++i){
			nodes[i].resize(nodeSizes[i], 0);
		}
		//Random number generator
		gen.seed(time(0));
		//Each weight is now between -0.2 and 0.2 (see proj3)
    	std::uniform_real_distribution<double> dis(-0.2,0.2);

		//create how many layers there will be (4,32,40,10,1)
		network.resize(nodeSizes.size()-1);
		for(unsigned int j=0; j<network.size(); ++j){
            network[j].resize(nodeSizes[j]*nodeSizes[j+1]);
            for(unsigned int k=0; k<network[j].size(); ++k){
                //put random weights in
                network[j][k] = dis(gen);
			}
		}
		//Generate a random king value U(1.0, 3.0)
		std::uniform_real_distribution<double> kingDis(1.0, 3.0);
		kingVal = kingDis(gen);

		//Save the beg sigmas of the weights
		sigmas.resize(network.size());
		for(size_t i=0; i<network.size(); ++i){
			sigmas[i].resize(network[i].size(), sigma);
		}
	}
	//user defined node inputs
	void giveInputs(vector<vector<double > > & inNodes){
		for(unsigned int i=0; i<inNodes.size(); ++i){
			for(unsigned int j=0; j<inNodes[i].size(); ++j){
				nodes[i][j] = inNodes[i][j];
			}
		}
	}

	//Given a board, calculate the output of the NN
	double calculateBoard(stdBoard & board){
		for(size_t i = 0;i < nodes.size();++i) {
            std::fill(nodes[i].begin(),nodes[i].end(),0.0);
		}
		getBoardInput(board);
		for (unsigned int i = 0; i < network.size(); ++ i) { //Step through network layers
           for (int j = 0; j < nodeSizes[i]; ++j) { //Step through first layer nodes
                for (int k = 0; k < nodeSizes[i+1]; ++k) {//Step through second layer nodes
                    nodes[i+1][k] += nodes[i][j] * network[i][j*nodeSizes[i+1] + k];
                }
            }
            //layer done, apply sigmoid

            for (unsigned int j = 0; j < nodes[i+1].size(); ++j) {
                sigmoid(nodes[i+1][j]);
            }
		}
		//Add other modifiers
		return nodes[nodes.size()-1][0];
		//nodes[nodes.size()-1][0] + boardCount(board)* pieceWeight;
	}

	//Puts board input into nodes vector in user specified index
	void getBoardInput(stdBoard & board){

		for(int i=0; i<32; ++i){
            nodes[0][i] =
                (int)(board.pieces[0][i] - board.pieces[1][i]) +
                (int)(board.pieces[2][i] - board.pieces[3][i]) * kingVal;
		}
	}

	double boardCount(stdBoard board) {
      double count =
        (int)(board.pieces[0].count() - board.pieces[1].count()) +
        (int)(board.pieces[2].count() - board.pieces[3].count()) *
        kingVal;
      return count;
    }

	//Given a number, calculate a sigmoid function output
	void sigmoid(double & num){
//		const static double e = 2.71828182845;
		//num = 1/(1 + exp(-num));
		num = num/(1+std::abs(num));

	}

	//Save this NN to a file
	int saveToFile(){

		ofstream file("NeuralNetworkFiles/" + familyName + "_NN_" + to_string(generation) + ".txt");

		if(!file){
			cout << "Unable to open file." << endl;
			return -1;
		}

		//write NN to file
			//save the generation number
		file << generation << endl;
			//save the king value
		file << kingVal << endl;
			//Save the weights
		//Save the size of the weight network
		file << network.size() << " ";
		for(size_t i=0; i<network.size(); ++i){
			file << network[i].size() << " ";
		}
		file << endl;
		//Save the actual weights
		for(size_t i=0; i<network.size(); ++i){
			for(size_t j=0; j<network[i].size(); ++j){
				file << network[i][j] << " ";
			}
			file << endl; //newline after each vector
		}
		file << "sigmas: " << endl;
		//Save the sigmas
		for(size_t i=0; i<sigmas.size(); ++i){
			for(size_t j=0; j<sigmas[i].size(); ++j){
				file << sigmas[i][j] << " ";
			}
			file << endl;
		}
		file.close();
		return 0; //successful
	}
	//Load NN from file
	int loadFromFile(string filename){
		ifstream file("NeuralNetworkFiles/" + filename);
		if(!file){
			cout << "Error opening file.";
			cout << " Filename: " << filename << endl;
			return -1;
		}
		//Read file line by line until failure
			//Read in the generation number
		string line;
		getline(file, line);

		istringstream num(line);
		num >> generation;

		//Read in the kingval
		getline(file, line);
		istringstream king(line);
		king >> kingVal;

		//Read in the network size
		getline(file, line);

		istringstream netSize(line);
		int networkSize;
		netSize >> networkSize;

		//Get the inner network sizes
		vector<int> networkWeightSize(networkSize);
		int i = 0;
		while(netSize){
			netSize >> networkWeightSize[i];
			++i;
		}

		//Change the overall network size
		network.resize(networkSize);

		//Read in the weights
		i = 0;
		while(getline(file, line) and line!="sigmas: "){
			istringstream weight(line);

			//Make the networks big enough
			network[i].resize(networkWeightSize[i]);

			//Load all the weights in
			for(size_t j=0; j<network[i].size(); ++j){
				weight >> network[i][j];
			}
			++i;
		}

		//Read in sigmas
		i = 0;
		sigmas.resize(networkSize);
		while(getline(file, line)){

			//Make the sigma vec the right size (from file)
			istringstream sig(line);
			sigmas[i].resize(networkWeightSize[i]);

			//Load the sigmas in
			for(size_t j=0; j<sigmas[i].size(); ++j){
				sig >> sigmas[i][j];
			}
			++i;
		}

		file.close();
		return 0;
	}

	//generate offspring, randomize this NN
		//(assuming this is a copy of the original)
	void becomeOffspring(){

		//change generation #
		++generation;

		//Randomize king value
		std::uniform_real_distribution<double> kingDis(-0.1,0.1);
		kingVal = kingVal + kingDis(gen);

		//randomize the sigmas
		//(mean, standard deviation)
		//Our numbers will generate bet -1 and 1 
		std::normal_distribution<double> nDis(0.0, 1.0);

		for(size_t i=0; i<sigmas.size(); ++i){
			for(size_t j=0; j<sigmas[i].size(); ++j){

				//The normal distribution returns numbers outside
					//our range, keep getting a rand number until
					//we get one we can use in our range 0-1
				double rand = nDis(gen);
				while(rand<-1.0 or rand>1.0){
					rand = nDis(gen);
				}

				//newSig = oldsig^(1/sqrt(2*sqrt(n)) * random)

				//Compute our new sigma
				sigmas[i][j] = pow(sigmas[i][j], (1.0/sqrt(2.0*sqrt(sigmas[i].size())))*rand);

				//randomize the weights using new sigmas
				rand = nDis(gen);
				while(rand<-1.0 or rand>1.0){
					rand = nDis(gen);
				}
				//newWeight = oldWeight + newSig * random normal num
				network[i][j] = network[i][j] + sigmas[i][j] * rand;
			}
		}
	}

	//Data Members
	std::vector<std::vector<double>> network;
    std::mt19937_64 gen; 
    std::vector<std::vector<double>> nodes; //Count of nodes per layer
    std::vector<int> nodeSizes;
    std::vector<vector<double>> sigmas; //the change in weights

};

#endif /* NEURAL_NETWORK_H */



//Possible Configurations ----------

//Neural Network Weights ----
//Monte Carlo
//Alpha Beta = adapt winning weights
//Breeding = mix NN's weights together if they win

//Search Tree ----
//Trim = trim search tree for faster searching
//Don't search every board, pick (like 3) a constant and search those
	//as far down as you can
