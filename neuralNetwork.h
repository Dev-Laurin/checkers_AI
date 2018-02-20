//neuralNetwork.h
//Neural Network
#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <random>
#include <vector>
#include <chrono>
using std::time;

using std::cout;
using std::endl;
#include "board.h"
#include "alphabeta.h"
#include <fstream> 
using std::ofstream; 
#include <string>
using std::string; 
using std::to_string; 

using std::vector;

class AIPlayer{
public:
    double kingVal = 0.4;  //Value of kings over pawns.
    double sigma = 0.05; //How much we vary from the parent weights
    double pieceWeight = 1.0; //Value of piece counter input
    double montyWeight = 1.0;
    int montyK = 1; // (w-l)/(w+l+k)
    int generation = 0; //Starts out as parent 

    stdBoard getMove(stdBoard board, bool side=false) {
          stdBoard possibleBoards[32];
          if(side) {
            board = board.flip();
          }
			    int moves = board.genMoves(possibleBoards,0);
			    int selectMove = 0;
			    sNN moveVal = beta(possibleBoards[0],7);
			    sNN tempMove;
			    for (int i = 1;i<moves;++i) {
                    tempMove = beta(possibleBoards[i],7);
                    //cout << "moveVal: " << moveVal << " tempMove: " << tempMove << endl;
                    //cout << "Highest: " << HIGHEST << " Lowest: " << LOWEST << endl;
                    if (tempMove > moveVal) {
                        //better move found
                        moveVal = tempMove;
                        selectMove = i;
                    }
			    }
			    board = possibleBoards[selectMove];
			    if (side) {
              board = board.flip();
			    }
			    return board;
    }

};

class NN: public AIPlayer
{
public:
	//Set weights to random values
	NN(std::vector<int>& nS){
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
	double calculateBoard(stdBoard & board, int nodeLayerIndex){
		getBoardInput(board, nodeLayerIndex);

		for (unsigned int i = 0; i < network.size(); ++ i) { //Step through network layers
           for (int j = 0; j < nodeSizes[i]; ++j) { //Step through first layer nodes
                for (int k = 0; k < nodeSizes[i+1]; ++k) {//Step through second layer nodes
                    nodes[i+1][k] += nodes[i][j] * network[i][j*nodeSizes[i+1] + k];
                }
            }
            //layer done, apply sigmoid
            for (unsigned int j = 0; j < nodes[i+1].size(); ++j) {
                nodes[i+1][j] = sigmoid(nodes[i+1][j]);
            }
		}
		//Add other modifiers
		return nodes[nodes.size()-1][0] + boardCount(board)* pieceWeight;
	}

	//Puts board input into nodes vector in user specified index
	void getBoardInput(stdBoard & board, int nodeLayerIndex){

		for(int i=0; i<32; ++i){
            nodes[nodeLayerIndex][i] =
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
	double sigmoid(double num){
		const static double e = 2.71828182845;
		return 1/(1 + pow(e, -num));
	}

	//Save this NN to a file
	int saveToFile(string filename){

		ofstream file(filename + "_NN_" + to_string(generation) + ".txt"); 

		if(!file){
			cout << "Unable to open file." << endl;
			return -1; 
		}

		//write NN to file 
			//save the generation number 
		file << generation << endl; 
			//save the king value
		file << kingVal << " " << sigma << endl; 
			//Save the weights
		//Save the size of the weight network
		file << network.size() << " "; 
		for(int i=0; i<network.size(); ++i){
			file << network[i].size() << " "; 
		}
		file << endl; 
		//Save the actual weights
		for(int i=0; i<network.size(); ++i){
			for(int j=0; j<network[i].size(); ++j){
				file << network[i][j] << " "; 
			}
			file << endl; //newline after each vector
		}


		file.close(); 

	}


	//Data Members
	std::vector<std::vector<double>> network;
    std::mt19937_64 gen; //(time(0));
    std::vector<std::vector<double>> nodes; //Count of nodes per layer
    std::vector<int> nodeSizes;

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
