//testNN.cpp
#include <iostream>
#include "neuralNetwork.h"
#include <vector>
using std::cout; 
using std::endl; 

int main(){
	//Blondie24
	std::vector<int> nodes{4, 32, 40, 10, 1}; 
	NN n(nodes); 

	cout << "Imitating Blondie24 Neural Network Size." << endl; 
	cout << "Network layers:  " << n.network.size() << endl; 
	for(int i=0; i<n.network.size(); ++i){
		std::cout << "Dendrite sizes: " << n.network[i].size() << std::endl; 
	}

	

}