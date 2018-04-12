//training.cpp
//Trains the NN
//2-19-18

#include <iostream>
using std::cout; 
using std::endl; 
#include <vector>
using std::vector; 
#include "neuralNetwork.h"

int main(){
	vector<int> nodes{32, 40, 10, 1}; 
	NN blondie24(nodes, "blondie");  
	blondie24.saveToFile(); 
	blondie24.loadFromFile("blondie_NN_0.txt"); 

	NN child = blondie24; 
	child.becomeOffspring(); 
	child.saveToFile(); 
	return 0; 
}