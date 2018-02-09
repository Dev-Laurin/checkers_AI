//testNN.cpp
#include <iostream>
#include "neuralNetwork.h"
#include <vector>
using std::cout; 
using std::endl; 

int main(){

	{
		//Blondie24
		std::vector<int> nodes{4, 32, 40, 10, 1}; 
		NN n(nodes); 

		cout << "Imitating Blondie24 Neural Network Size." << endl; 
		cout << "Network layers:  " << n.network.size() << endl; 
		for(int i=0; i<n.network.size(); ++i){
			std::cout << "Dendrite sizes: " << n.network[i].size() << std::endl; 
		}
	}

	//Test a bigger network
	std::vector<int> nodes{10, 50, 50, 50, 100, 50, 1}; 
	NN bigN(nodes); 

	cout << "Bigger Neural Network > 10,000 weights." << endl;
	cout << "Network layers: " << bigN.network.size() << endl;
	double weights = 0; 
	for(int i=0; i<bigN.network.size(); ++i){
		weights += bigN.network[i].size(); 
		cout << "Dendrite sizes: " << bigN.network[i].size() << endl;
	}
	cout << "Counted network weights: " << weights << endl; 

}