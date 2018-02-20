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

	NN copy = blondie24; 
	copy.generation+=1; 
	copy.saveToFile(); 

	ifstream file("blondie_NN_0.txt"); 
	ifstream file1("blondie_NN_1.txt"); 

	return 0; 
}