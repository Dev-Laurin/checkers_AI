//neuralNetwork.h
//Neural Network
#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <random> 
#include <vector>
using std::cout; 
using std::endl; 

class NN{
public:
	//Set weights to random values
	NN(std::vector<int>& nodeSizes){
		//create how many layers there will be (4,32,40,10,1)
		for(int j=0; j<nodeSizes.size(); ++j){

			if(j+1 < nodeSizes.size()){
				std::vector<double> v(nodeSizes[j]*nodeSizes[j+1]);
				for(int k=0; k<v.size(); ++k){
					//put random weights in 
					std::mt19937 gen(time(0)); 
					std::uniform_real_distribution<double> dis(-1.0,1.0); 
					v[k] = dis(gen); 
				}
				network.push_back(v); 					
			}			
		}
	}



	std::vector<std::vector<double > > network; 	
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
