//blondieExperiment1SpringBreak
//To run the first blondie24 experiment over spring break
	//to get errors early rather than later with NN. 
//3-12-18

#include <iostream>
using std::cout; 
using std::endl; 
#include <vector>
using std::vector; 
#include "neuralNetwork.h"
#include <boost/filesystem.hpp> 

int main(){
	vector<int> nodes{32, 40, 10, 1}; 
	NN blondie24(nodes, "blondie");  
	blondie24.saveToFile(); 

	return 0; 
}