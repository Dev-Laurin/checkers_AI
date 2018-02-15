//neuralNetwork.h
//Neural Network
#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <random>
#include <vector>
using std::cout;
using std::endl;
#include "board.h"
using std::vector; 


class NN{
public:
	//Set weights to random values
	NN(std::vector<int>& nodeSizes){
		//Save the node configuration
		nodes.resize(nodeSizes.size());
		for(int i=0; i<nodeSizes.size(); ++i){
			nodes[i].resize(nodeSizes[i], 0); 
		}
		//Random number generator
		gen.seed(time(0)); 
    	std::uniform_real_distribution<double> dis(-1.0,1.0);

		//create how many layers there will be (4,32,40,10,1)
		network.resize(nodeSizes.size()-1);
		for(int j=0; j<network.size(); ++j){
            network[j].resize(nodeSizes[j]*nodeSizes[j+1]);
            for(int k=0; k<network[j].size(); ++k){
                //put random weights in
                network[j][k] = dis(gen);
			}
		}
	}
	//user defined node inputs
	int giveInputs(vector<vector<double > > & inNodes){
		for(int i=0; i<inNodes.size(); ++i){
			for(int j=0; j<inNodes[i].size(); ++j){
				nodes[i][j] = inNodes[i][j]; 
			}
		}
	}

	//Given a board, calculate the output of the NN 
	int calculateBoard(stdBoard & board, int nodeLayerIndex){

		getBoardInput(board, nodeLayerIndex); 
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
	}

	//Puts board input into nodes vector in user specified index
	void getBoardInput(stdBoard & board, int nodeLayerIndex){
		
		for(int i=0; i<board.str().size(); ++i){

			//for each position on board
			char pos = board.str()[i]; 

			if(pos=='b'){
				nodes[nodeLayerIndex][i] = 1.0; 
			}
			else if(pos=='r'){
				nodes[nodeLayerIndex][i] = -1.0; 
			}
			else if(pos==' '){
				//is a blank space
				nodes[nodeLayerIndex][i] = 0; 
			}			
			else if(pos=='B'){
				nodes[nodeLayerIndex][i] = 1.4; 
			}
			else{ //Red king 'R'
				nodes[nodeLayerIndex][i] = -1.4; 
			}

		} 
	}

	//Given a number, calculate a sigmoid function output 
	double sigmoid(double num){
		double e = 2.71828182845; 
		return 1/(1 + pow(e, -num)); 
	}
	std::vector<std::vector<double > > network;
    std::mt19937 gen; //(time(0));
    std::vector<std::vector<double >> nodes; 

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
