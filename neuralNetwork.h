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
	NN(std::vector<int>& nS){
		gen.seed(time(0));
   	std::uniform_real_distribution<double> dis(-1.0,1.0);
   	nodeSizes = nS;

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
	std::vector<std::vector<double > > network;
  std::mt19937 gen; //(time(0));
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
