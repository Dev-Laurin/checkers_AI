//neuralNetwork.h
//Neural Network
#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <random>
#include <vector>
using std::cout;
using std::endl;
#include "board.h"

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
	//Given a board, calculate the output of the NN 
	int calculateBoard(stdBoard & board){
		//Get board input and place into node inputs
		getBoardInput(board); 

		//iterate through network 
		for(int net=0; net<nodes.size()-1; ++net){
			int netIndex = 0;
			if(net!=0)
				netIndex = net - 1;
			int networkIndex = 0; 
			//for each node, sum up the weights & firing value
			for(int node=0; node<nodes[net+1].size(); ++node){
				double sumWeights = 0;
				for(int weight=0; weight<nodes[net].size(); ++weight){
					sumWeights+= network[netIndex][networkIndex] * nodes[net][weight];
					++networkIndex; 
				}		

				//Apply the sigmoid function to the weights & fires
				double sigmoidOutput = sigmoid(sumWeights);
				if(sigmoidOutput>0.55){
					//it fires, output a 1 ?
					nodes[net+1][node] = 1; 
				} 
				else if(sigmoidOutput<0.45){
					nodes[net+1][node] = -1; 
				}
				else{
					//it doesn't fire, output 0 
					nodes[net+1][node] = 0; 
				}
		
			}  
		}

		//the output
		return nodes[nodes.size()-1][0]; 
	}
	//Puts board inputs into nodes for NN calculating
	void getBoardInput(stdBoard & board){
		for(int i=0; i<board.str().size(); ++i){
			//for each position on board
			char pos = board.str()[i]; 

			if(pos=='b'){
				nodes[0][i] = 1; 
			}
			else if(pos=='r'){
				nodes[0][i] = -1; 
			}
			else if(pos==' '){
				//is a blank space
				nodes[0][i] = 0; 
			}			
			else if(pos=='B'){
				nodes[0][i] = 1.4; 
			}
			else{ //Red king 'R'
				nodes[0][i] = -1.4; 
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
