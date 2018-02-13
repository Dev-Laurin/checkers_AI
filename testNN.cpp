//testNN.cpp
#include <iostream>
#include "neuralNetwork.h"
#include <vector>
using std::cout; 
using std::endl; 
#include "board.h"

int main(){

	{
		//Blondie24
		std::vector<int> nodes{4, 32, 40, 10, 1}; 
		NN n(nodes); 

		cout << "Imitating Blondie24 Neural Network Size." << endl; 
		cout << "Network layers:  " << n.network.size() << endl; 
		for(int i=0; i<n.network.size(); ++i){
			std::cout << "Dendrite sizes: " << n.network[i].size() << std::endl; 
			
			// for(int j=0; j<n.network[i].size(); ++j){
			// 	cout << n.network[i][j] << endl; 
			// }
		}
	}

	//Test a bigger network
	std::vector<int> nodes{32, 1}; //50, 50, 50, 100, 50, 1}; 
	NN bigN(nodes); 

	cout << "Bigger Neural Network > 10,000 weights." << endl;
	cout << "Network layers: " << bigN.network.size() << endl;
	double weights = 0; 
	for(int i=0; i<bigN.network.size(); ++i){
		weights += bigN.network[i].size(); 
		cout << "Dendrite sizes: " << bigN.network[i].size() << endl;
		// for(int j=0; j<bigN.network[i].size(); ++j){
		// 	cout << bigN.network[i][j] << endl; 
		// }
	}
	cout << "Counted network weights: " << weights << endl; 

	cout << "Testing Board input-output" << endl; 
	cout << "Nodes "<< endl; 
	cout << "Nodes size [i]: "<< bigN.nodes.size() << endl; 
	for(int i=0; i<bigN.nodes.size(); ++i){
		for(int j=0; j<bigN.nodes[i].size(); ++j){
			cout << bigN.nodes[i][j] << " ";  
		}
		cout << endl; 
	}
	stdBoard b; 
	//b.updateBoard("           r       b            "); 
	bigN.getBoardInput(b); 
	cout << "Nodes size [i]: "<< bigN.nodes.size() << endl; 
	for(int i=0; i<bigN.nodes.size(); ++i){
		for(int j=0; j<bigN.nodes[i].size(); ++j){
			cout << bigN.nodes[i][j] << " ";  
		}
		cout << endl; 
	}	
	cout << "First board evaluation: " << bigN.calculateBoard(b);
	cout << endl; 

	cout << "Node vector after eval: " << endl; 
		cout << "Nodes size [i]: "<< bigN.nodes.size() << endl; 
	for(int i=0; i<bigN.nodes.size(); ++i){
		for(int j=0; j<bigN.nodes[i].size(); ++j){
			cout << bigN.nodes[i][j] << " ";  
		}
		cout << endl; 
	}	

	cout << "Weights: " << endl; 
	double sum = 0; 
	for(int i=0; i<bigN.network.size(); ++i){
		for(int j=0; j<bigN.network[i].size(); ++j){
			cout << bigN.network[i][j] << " "; 
			sum += (bigN.network[i][j] * bigN.nodes[i][j]); 
		}
		cout << endl; 
	}
	cout << "Sum of weights: " << sum << endl; 
	cout << "Sigmoid of : " << bigN.sigmoid(sum) << endl; 

}