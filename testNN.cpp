//testNN.cpp
#include <iostream>
#include "neuralNetwork.h"
#include <vector>
using std::cout; 
using std::endl; 
#include "board.h"
#include <ctime> 

int main(){

	{
		//Blondie24
		std::vector<int> nodes{4, 32, 40, 10, 1}; 
		NN n(nodes); 

		cout << "Imitating Blondie24 Neural Network Size.--------" << endl; 
		cout << "Network layers:  " << n.network.size() << endl; 
		for(int i=0; i<n.network.size(); ++i){
			std::cout << "Dendrite sizes: " << n.network[i].size() << std::endl; 
			
			// for(int j=0; j<n.network[i].size(); ++j){
			// 	cout << n.network[i][j] << endl; 
			// }
		}
	}

	{
		cout << endl; 
		cout << "Bigger Neural Net.--------" << endl; 
		//Test a bigger network
		std::vector<int> nodes{32, 50, 50, 50, 100, 50, 1}; 
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
	}


	{
		//Test a network
		std::vector<int> nodes{32, 1}; //50, 50, 50, 100, 50, 1}; 
		NN bigN(nodes); 

		cout << endl; 
		cout << "Small Neural Network - {32, 1}.--------" << endl;
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
		cout << endl; 
		cout << "Testing Board input-output" << endl;  

		stdBoard b; 
		//b.updateBoard("           r       b            "); 
		bigN.getBoardInput(b); 
		
		cout << "	Checking filling up nodes with input from board." << endl; 
		string nodeInputs = ""; 
		for(int i=0; i<bigN.nodes.size(); ++i){
			for(int j=0; j<bigN.nodes[i].size(); ++j){
				nodeInputs += std::to_string((int)bigN.nodes[i][j]);  
			}
		}
		if(nodeInputs=="-1-1-1-1-1-1-1-1-1-1-1-1000000001111111111110"){
			cout << "	Node inputs from board successful." << endl;
		}	
		else{
			cout << nodeInputs << endl; 
			cout << "vs" << endl; 
			cout << "-1-1-1-1-1-1-1-1-1-1-1-1000000001111111111110" << endl;
		}

		int boardEvalReturn = bigN.calculateBoard(b); 
		cout << "	First board evaluation: " << boardEvalReturn;
		cout << endl; 

		string nodeInts = "";   
		for(int i=0; i<bigN.nodes.size(); ++i){
			for(int j=0; j<bigN.nodes[i].size(); ++j){
				nodeInts+= std::to_string((int)bigN.nodes[i][j]);  
			}
		}	
		nodeInputs[nodeInputs.size()-1] = (std::to_string((int)boardEvalReturn))[0]; 
		nodeInputs+=(std::to_string((int)boardEvalReturn))[1]; 
		if(nodeInputs==nodeInts){
			cout << "	After board eval node vals same. Successful" << endl;
		}
		else{
			cout << nodeInts << endl; 
			cout << "vs" << endl; 
			cout << nodeInputs << endl; 
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
		auto sigOutput = bigN.sigmoid(sum); 
		cout << "Sigmoid of : " << sigOutput << endl;

		if(sigOutput>0.55){
			//it fires, output a 1 ?
			if(bigN.nodes[1][0] == 1){
				cout << "	Test successful. Output = 1." << endl;
			}
		} 
		else if(sigOutput<0.45){
			if(bigN.nodes[1][0] == -1)
				cout << "	Test successful. Output = -1." << endl; 
		}
		else{
			//it doesn't fire, output 0 
			if(bigN.nodes[1][0] == 0)
				cout << "	Test successful. Output = 0." << endl; 
		}
	}

	//Time the NN 
	cout << endl; 
	cout << "Timing code. Timing how fast the";
	cout << " Neural Network Evaluates a Board " << endl; 

	{
		cout << "Timed Test 1. Board: '           r       b            '";
		cout << endl; 
		clock_t start; 
		double duration; 
		start = clock();

		std::vector<int> nodes{32, 50, 50, 50, 100, 50, 1};
		NN nTimed1(nodes); 
		stdBoard b; 
		b.updateBoard("           r       b            "); 
		auto output = nTimed1.calculateBoard(b);
		cout << "Output: " << output << endl; 

		duration = (clock() - start) / (double) CLOCKS_PER_SEC;
		cout << "Time Difference: " << duration << endl; 
	}

	{
		cout << "Timed Test 2. Board: Default";
		cout << endl; 
		clock_t start; 
		double duration; 
		start = clock();

		std::vector<int> nodes{32, 50, 50, 50, 100, 50, 1};
		NN nTimed1(nodes); 
		stdBoard b; 
		auto output = nTimed1.calculateBoard(b);
		cout << "Output: " << output << endl; 

		duration = (clock() - start) / (double) CLOCKS_PER_SEC;
		cout << "Time Difference: " << duration << endl; 
	}

}