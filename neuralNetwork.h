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
using std::vector;

class AIPlayer{
public:
    double kingVal = 0.4;  //Value of kings over pawns.
    double pieceWeight = 1.0; //Value of piece counter input
    double montyWeight = 1.0;
    int montyK = 1; // (w-l)/(w+l+k)

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
    	std::uniform_real_distribution<double> dis(-1.0,1.0);

		//create how many layers there will be (4,32,40,10,1)
		network.resize(nodeSizes.size()-1);
		for(unsigned int j=0; j<network.size(); ++j){
            network[j].resize(nodeSizes[j]*nodeSizes[j+1]);
            for(unsigned int k=0; k<network[j].size(); ++k){
                //put random weights in
                network[j][k] = dis(gen);
			}
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
	double calculateBoard(stdBoard & board, int nodeLayerIndex){
		getBoardInput(board, nodeLayerIndex);

		for (unsigned int i = 0; i < network.size(); ++ i) { //Step through network layers
/*                cout << "i: " << i << endl;
                cout << "nodeSizes.size(): " << nodeSizes.size() << endl;
                cout << "nodeSize[i] " << nodeSizes[i] << " " << nodeSizes[i+1] << endl;
                cout << "nodes size: " << nodes[i].size() << " " << nodeSizes[i+1] << endl;
                cout << "network size: " << network[i].size() << endl;
*/            for (int j = 0; j < nodeSizes[i]; ++j) { //Step through first layer nodes
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
		/*
		//Go through entire node vector
		for(int outerNodeVector=0; outerNodeVector<nodes.size()-1; outerNodeVector++){

			int networkIndex = 0;
			//For every right-hand node
			for(int i=0; i<nodes[outerNodeVector+1].size(); ++i){
				double sumWeights = 0;
				//The left-hand nodes
				for(int j=0; j<nodes[outerNodeVector].size(); ++j){
					sumWeights+= network[outerNodeVector][networkIndex] * nodes[outerNodeVector][j];
					++networkIndex;
				}

				//Apply the sigmoid function to the weights & fires
				//cout << "Sum of weights: " << sumWeights << endl;
				double sigmoidOutput = sigmoid(sumWeights);
				//cout << "Sigmoid: " << sigmoidOutput << endl;
				//cout << "outerNodeVector: " << outerNodeVector << endl;
				if(sigmoidOutput>0.55){
					//it fires, output a 1 ?
					nodes[outerNodeVector+1][i] = 1;
				}
				else if(sigmoidOutput<0.45){
					nodes[outerNodeVector+1][i] = -1;
				}
				else{
					//it doesn't fire, output 0
					nodes[outerNodeVector+1][i] = 0;
				}
			}
		}
		//the output
		return nodes[nodes.size()-1][0];
		*/
	}

	//Puts board input into nodes vector in user specified index
	void getBoardInput(stdBoard & board, int nodeLayerIndex){

		for(int i=0; i<32; ++i){
            nodes[nodeLayerIndex][i] =
                (int)(board.pieces[0][i] - board.pieces[1][i]) +
                (int)(board.pieces[2][i] - board.pieces[3][i]) * kingVal;
            //cout << "node " << i << " val: " << nodes[nodeLayerIndex][i] << endl;
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
