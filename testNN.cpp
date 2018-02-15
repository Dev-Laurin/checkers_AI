//testNN.cpp
#include <iostream>
#include "neuralNetwork.h"
#include <vector>
using std::cout;
using std::endl;
#include "board.h"
#include <ctime> 
#include <random> 

// #include <ctime>
// #include <chrono>

int main(){

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
		bigN.getBoardInput(b, 0); 
		
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

		int boardEvalReturn = bigN.calculateBoard(b, 0); 

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
			else{
				cout << "Failed. " << bigN.nodes[1][0] << endl;
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

	{
		cout << "Testing a 2 hidden layer network: " << endl;
		std::vector<int>nodes{32, 2, 1}; 
		NN twoLayer(nodes); 
		stdBoard b; 
		int output = twoLayer.calculateBoard(b, 0); 

		//Check if first hidden layer 2 nodes are correct
		double firstNode = 0; 
		for(int j=0; j<twoLayer.nodes[0].size(); ++j){
			firstNode += twoLayer.nodes[0][j] * twoLayer.network[0][j];
		}
		firstNode = twoLayer.sigmoid(firstNode); 
		if(firstNode>0.55){
			//it fires, output a 1 ?
			firstNode = 1; 
		} 
		else if(firstNode<0.45){
			firstNode = -1; 
		}
		else{
			//it doesn't fire, output 0 
			firstNode = 0; 
		}
		if(firstNode==twoLayer.nodes[1][0]){
			cout << "First hidden layer node successful." << endl;
		}
		else{
			cout << "Failed first hidden node." << endl; 
			cout << firstNode << endl; 
			cout << "vs" << endl; 
			cout << twoLayer.nodes[1][0] << endl; 
		}
		//Second node 
		double secondNode = 0; 
		int networkIndex = 32; 
		for(int j=0; j<twoLayer.nodes[0].size(); ++j){
			secondNode += twoLayer.nodes[0][j] * twoLayer.network[0][networkIndex];
			++networkIndex; 
		}		
		secondNode = twoLayer.sigmoid(secondNode); 
		if(secondNode>0.55){
			//it fires, output a 1 ?
			secondNode = 1; 
		} 
		else if(secondNode<0.45){
			secondNode = -1; 
		}
		else{
			//it doesn't fire, output 0 
			secondNode = 0; 
		}
		if(secondNode==twoLayer.nodes[1][1]){
			cout << "Successful Second hidden node." << endl;
		}
		else{
			cout << "Failed 2nd hidden node." << endl; 
			cout << secondNode << endl; 
			cout << "Vs" << endl; 
			cout << twoLayer.nodes[1][1] << endl; 
		}

		//Test output as well
		double finalNode = 0; 
		for(int i=0; i<twoLayer.nodes[1].size(); ++i){
			finalNode += twoLayer.nodes[1][i] * twoLayer.network[1][i]; 
		}
		finalNode = twoLayer.sigmoid(finalNode); 
		if(finalNode>0.55){
			//it fires, output a 1 ?
			finalNode = 1; 
		} 
		else if(finalNode<0.45){
			finalNode = -1; 
		}
		else{
			//it doesn't fire, output 0 
			finalNode = 0; 
		}
		if(finalNode==twoLayer.nodes[2][0] and 
			finalNode==output){
			cout << "Successful Final node." << endl;
		}
		else{
			cout << "Failed Final node." << endl;
			cout << finalNode << endl; 
			cout << "vs" << endl; 
			cout << twoLayer.nodes[2][0] << endl; 
			cout << "vs" << endl; 
			cout << output << endl; 
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
		auto output = nTimed1.calculateBoard(b, 0);
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
		auto output = nTimed1.calculateBoard(b, 0);
		cout << "Output: " << output << endl; 

		duration = (clock() - start) / (double) CLOCKS_PER_SEC;
		cout << "Time Difference: " << duration << endl; 
	}

	{
		cout << "Timed Test 3. Board: 'rrrrrrrrrr r  r  b  bb bbbbbbbbb'";
		cout << endl; 
		clock_t start; 
		double duration; 
		start = clock();

		std::vector<int> nodes{32, 50, 50, 50, 100, 50, 1};
		NN nTimed1(nodes); 
		stdBoard b; 
		b.updateBoard("rrrrrrrrrr r  r  b  bb bbbbbbbbb");  
		auto output = nTimed1.calculateBoard(b, 0);
		cout << "Output: " << output << endl; 

		duration = (clock() - start) / (double) CLOCKS_PER_SEC;
		cout << "Time Difference: " << duration << endl;
	}

	{
		cout << "Timed Test 4. Board: 'rrrrr    r       r    b bbbbbbbb'";
		cout << endl; 
		clock_t start; 
		double duration; 
		start = clock();

		std::vector<int> nodes{32, 50, 50, 50, 100, 50, 1};
		NN nTimed1(nodes); 
		stdBoard b; 
		b.updateBoard("rrrrr    r       r    b bbbbbbbb");  
		auto output = nTimed1.calculateBoard(b, 0);
		cout << "Output: " << output << endl; 

		duration = (clock() - start) / (double) CLOCKS_PER_SEC;
		cout << "Time Difference: " << duration << endl; 


		//Test whether the NN is consistent
		vector<int> nod{32, 40, 10, 1}; 
		NN consistent(nod); 
		stdBoard bb; 

		cout << "Testing for NN consistancy." << endl; 
		vector<int>outputFromNN(4); 
		for(int i=0; i<4; i++){
			outputFromNN[i] = consistent.calculateBoard(bb, 0); 
		}
		if(outputFromNN[0]==outputFromNN[1]==outputFromNN[2]==outputFromNN[3]){
			cout << "NN is consistent using same board. Successful." << endl;
		}
		else{
			cout << "Failed consistancy."<< endl; 
			for(int i=0; i<outputFromNN.size(); i++){
				cout << outputFromNN[i] << endl;
			}
		}

	}

		{
		//Blondie24
		std::vector<int> nodes{ 32, 40, 10, 1}; 
		NN n(nodes); 

		cout << "Imitating Blondie24 Neural Network Size.--------" << endl; 
		cout << "Network layers:  " << n.network.size() << endl; 
		for(int i=0; i<n.network.size(); ++i){
			std::cout << "Dendrite sizes: " << n.network[i].size() << std::endl; 
			
			// for(int j=0; j<n.network[i].size(); ++j){
			// 	cout << n.network[i][j] << endl; 
			// }
		}

		//Timing
		stdBoard b; //Default
		stdBoard b1; 
		b1.updateBoard("           r       b            ");
		stdBoard b2; 
		b2.updateBoard("rrrrrrrrrr r  r  b  bb bbbbbbbbb"); 
		stdBoard b3; 
		b3.updateBoard("rrrrr    r       r    b bbbbbbbb"); 
		std::vector<stdBoard> boards{b, b1, b2, b3}; 

		std::mt19937 gen(time(0));
		std::uniform_int_distribution<int> dis(0,boards.size()-1);

		//Inputs for Blondie {4, ...}
		//b = 1.0
		//B = 1.4
		//r = -1.0
		//R = -1.4

		// std::vector<std::vector<double > > nodeInput{
		// 	{1.0, 
		// 	1.4, 
		// 	-1.0,
		// 	-1.4}, }; 

		cout << "Timing for Blondie24 NN. " << endl; 
		clock_t start; 
		double duration; 
		start = clock();
		NN blondie(nodes); 
		//blondie.giveInputs(nodeInput); 

		int runs = 1000; 
		for(int i=0; i<runs; ++i){
			cout << blondie.calculateBoard(boards[dis(gen)], 0) << endl; 
		}

		duration = (clock() - start) / (double) CLOCKS_PER_SEC;
		cout << "Time Difference: " << duration << endl; 

		double timePerBoard = duration/runs; 
		cout << "Time per board: " << timePerBoard << endl; 
		cout << "Board Evals per second: " << runs/duration << endl;
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

	stdBoard b; //Default
	stdBoard b1; 
	b1.updateBoard("           r       b            ");
	stdBoard b2; 
	b2.updateBoard("rrrrrrrrrr r  r  b  bb bbbbbbbbb"); 
	stdBoard b3; 
	b3.updateBoard("rrrrr    r       r    b bbbbbbbb"); 
	std::vector<stdBoard> boards{b, b1, b2, b3}; 

	std::mt19937 gen(time(0));
	std::uniform_int_distribution<int> dis(0,boards.size()-1);

//Start the timer

	cout << "Timing for Big NN {32, 50, 50, 50, 100, 50, 1}." << endl;
	clock_t start; 
	double duration; 
	start = clock();

	std::vector<int> nodes{32, 50, 50, 50, 100, 50, 1};
	NN loopTimedNN(nodes);

	int randIndex = 0;  
	//For loop for timing purposes, testing 4 boards chosen randomly
	int runs = 1000; 
	for(int i=0; i<runs; ++i){
		randIndex = dis(gen);
		cout << "Answer: " << loopTimedNN.calculateBoard(boards[randIndex], 0) << endl; 
	}


	duration = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Time Difference: " << duration << endl; 

	double timePerBoard = duration/runs; 
	cout << "Time per board: " << timePerBoard << endl; 
	cout << "Board Evals per second: " << runs/duration << endl;


// 	//To create NN & evaluate: Max Time (not loop) = 0.006 to 0.007 seconds = 6 miliseconds = 6 mil nanoseconds
// 	//If O2 flag = 11955.3 boards per sec (around 12,000)
// 		//179,329.5 boards in 15 sec 

	return 0; 
}

