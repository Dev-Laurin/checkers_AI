//testNN.cpp
//2-10-18
//To test our neural network implementation.
#include <iostream>
#include "neuralNetwork.h"
#include <vector>
using std::cout;
using std::endl;
#include "board.h"
#include <ctime>
#include <random>

//Tells catch to provide a main (one file only)
#define CATCH_CONFIG_MAIN
#include "Catch2.hpp" //C++ Testing Framework

// #include <chrono> <- Doesn't work on Laurin's machine
	//Issue with new 5.4.1 g++ version on Ubuntu 16.06?


//Check's if two NN save files are equal
int checkIfFilesAreEqual(ifstream & file1, ifstream & file2){

	string line, line2;
	//skip generation line
	getline(file1, line);
	getline(file2, line2);

	while(getline(file1, line) and getline(file2, line2)){
		if(line!=line2){
			cout << line << endl;
			cout << "vs" << endl;
			cout << line2 << endl;
			return -1;
		}
	}
	return 0;
}

//Check if vector is full of same number
int checkIfVectorIsSame(vector<double> v){
	double num = v[0];
	for(size_t i=0; i<v.size(); ++i){
		auto diff = v[i] - num;
		if(diff>0.1 or diff< -0.1)
			return -1;
	}
	return 0;
}

//Beginning Testing of NN using Catch2

//Test a single network
TEST_CASE("Testing creation of small Neural Network.", "{32, 1}"){
	//Test a network
	std::vector<int> nodes{32, 1};
	NN smallN(nodes, "smallnn");

	//Test it is the right overall size
	REQUIRE( smallN.network.size() == 1);
	REQUIRE( smallN.nodes.size() == 2);
	//Test if the weights are the right size
	REQUIRE( smallN.network[0].size() == 32 );
	//Test how many weights there are
	double weights = 0;
	for(unsigned int i=0; i<smallN.network.size(); ++i){
		weights += smallN.network[i].size();
	}
	REQUIRE(weights == 32);
}

TEST_CASE("Testing small NN input-output.", "{32, 1}"){
	std::vector<int> nodes{32, 1};
	NN smallN(nodes, "smallnn");

	stdBoard b;
	smallN.getBoardInput(b);

	//Check if nodes are input correctly
		//default 0
		//Board is loaded in
	string nodeInputs = "";
	for(unsigned int i=0; i<smallN.nodes.size(); ++i){
		for(unsigned int j=0; j<smallN.nodes[i].size(); ++j){
			nodeInputs += std::to_string((int)smallN.nodes[i][j]);
		}
	}

	REQUIRE( nodeInputs == "-1-1-1-1-1-1-1-1-1-1-1-1000000001111111111110");

	//Test board evaluation
	int boardEvalReturn = smallN.calculateBoard(b);


	string nodeInts = "";
	for(unsigned int i=0; i<smallN.nodes.size(); ++i){
		for(unsigned int j=0; j<smallN.nodes[i].size(); ++j){
			nodeInts+= std::to_string((int)smallN.nodes[i][j]);
		}
	}
	nodeInputs[nodeInputs.size()-1] = (std::to_string((int)boardEvalReturn))[0];

	REQUIRE (nodeInputs==nodeInts);


	//Test the weights calculation
	double sum = 0;
	for(unsigned int i=0; i<smallN.network.size(); ++i){
		for(unsigned int j=0; j<smallN.network[i].size(); ++j){
			sum += (smallN.network[i][j] * smallN.nodes[i][j]);
		}
	}

	double sigOutput = sum;
	smallN.sigmoid(sigOutput);
	REQUIRE(sigOutput==smallN.nodes[1][0]);

}

TEST_CASE("Testing a 2 hidden layer network. ", "{32, 2, 1}"){
	std::vector<int>nodes{32, 2, 1};
	NN twoLayer(nodes, "twolayer");
	stdBoard b;
	int output = twoLayer.calculateBoard(b);

	//Check if first hidden layer 2 nodes are correct
	double firstNode = 0;
	for(unsigned int j=0; j<twoLayer.nodes[0].size(); ++j){
		firstNode += twoLayer.nodes[0][j] * twoLayer.network[0][j*twoLayer.nodes[1].size()];
	}
	twoLayer.sigmoid(firstNode);
	REQUIRE(firstNode == twoLayer.nodes[1][0]);

	//Second node
	double secondNode = 0;
	int networkIndex = 32;
	for(unsigned int j=0; j<twoLayer.nodes[0].size(); ++j){
		secondNode += twoLayer.nodes[0][j] * twoLayer.network[0][j*twoLayer.nodes[1].size()+1 ];
		++networkIndex;
	}
	twoLayer.sigmoid(secondNode);
	REQUIRE(secondNode == twoLayer.nodes[1][1]);

	//Test output as well
	double finalNode = 0;
	for(unsigned int i=0; i<twoLayer.nodes[1].size(); ++i){
		finalNode += twoLayer.nodes[1][i] * twoLayer.network[1][i];
	}
	twoLayer.sigmoid(finalNode);
	REQUIRE(finalNode == twoLayer.nodes[2][0]);
}

TEST_CASE("Testing NN for consistancy. If the outputs are the same for the same board.",
	"{32, 40, 10, 1}"){

	//Test whether the NN is consistent
	vector<int> nod{32, 40, 10, 1};
	NN consistent(nod, "consistentNNTest");
	stdBoard bb;

	vector<double>outputFromNN(4);
	for(int i=0; i<4; i++){
		outputFromNN[i] = consistent.calculateBoard(bb);
	}
	cout << outputFromNN[0] << " " << outputFromNN[1];
	cout << " " << outputFromNN[2] << " " << outputFromNN[3];
	cout << endl;
	REQUIRE(0==checkIfVectorIsSame(outputFromNN));
}

TEST_CASE("Blondie24", "{32, 40, 10, 1}"){
	//Blondie24
	std::vector<int> nodes{ 32, 40, 10, 1};
	NN blondie(nodes, "blondie24");

	REQUIRE(blondie.network.size() == 3);
	REQUIRE(blondie.network[0].size() == 32*40);
	REQUIRE(blondie.network[1].size() == 40*10);
	REQUIRE(blondie.network[2].size() == 10);

	REQUIRE(blondie.nodes.size() == 4);
	REQUIRE(blondie.nodes[0].size() == 32);
	REQUIRE(blondie.nodes[1].size() == 40);
	REQUIRE(blondie.nodes[2].size() == 10);
	REQUIRE(blondie.nodes[3].size() == 1);
}

TEST_CASE("Timing Blondie24.", "{32, 40, 10, 1}"){

	//Timing
	stdBoard b; //Default
	stdBoard b1;
	b1.updateBoard("           r       b            ");
	stdBoard b2;
	b2.updateBoard("rrrrrrrrrr r  r  b  bb bbbbbbbbb");
	stdBoard b3;
	b3.updateBoard("rrrrr    r       r    b bbbbbbbb");
	std::vector<stdBoard> boards{b, b1, b2, b3};

	std::vector<int> nodes{ 32, 40, 10, 1};

	std::mt19937 gen(time(0));
	std::uniform_int_distribution<int> dis(0,boards.size()-1);

	clock_t start;
	double duration;
	NN blondie(nodes, "blondie24");
	int runs = 100000;
	int boardsel[100000];
	for (int i=0; i<runs;++i) {
        boardsel[i] = dis(gen);
	}
	double outputTotal = 0;

	start = clock();
	for(int i=0; i<runs; ++i){
		outputTotal += blondie.calculateBoard(boards[boardsel[i]]);
	}

	duration = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "To not compiler optimize: " << outputTotal << endl;
	cout << "Blondie24 Time Difference: " << duration << endl;

	double timePerBoard = duration/runs;
	cout << "Time per board: " << timePerBoard << endl;
	cout << "Board Evals per second: " << runs/duration << endl;
	cout << endl;
	//We should have at least 12,000 boards per second
	REQUIRE(runs/duration > 10000);
}


TEST_CASE("Construction of Big Neural Network.",
	"{32, 50, 50, 50, 100, 50, 1}"){

	//Test a bigger network
	std::vector<int> nodes{32, 50, 50, 50, 100, 50, 1};
	NN bigN(nodes, "bigN");

	REQUIRE(bigN.network.size() == 6);
	REQUIRE(bigN.network[0].size() == 32*50);
	REQUIRE(bigN.network[1].size() == 50*50);
	REQUIRE(bigN.network[2].size() == 50*50);
	REQUIRE(bigN.network[3].size() == 50*100);
	REQUIRE(bigN.network[4].size() == 100*50);
	REQUIRE(bigN.network[5].size() == 50);

	REQUIRE(bigN.nodes.size() == 7);
	REQUIRE(bigN.nodes[0].size() == 32);
	REQUIRE(bigN.nodes[1].size() == 50);
	REQUIRE(bigN.nodes[2].size() == 50);
	REQUIRE(bigN.nodes[3].size() == 50);
	REQUIRE(bigN.nodes[4].size() == 100);
	REQUIRE(bigN.nodes[5].size() == 50);
	REQUIRE(bigN.nodes[6].size() == 1);
}

TEST_CASE("Timing for Big Neural Network.",
	"{32, 50, 50, 50, 100, 50, 1}"){

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
	clock_t start, stop;
	double duration;

	std::vector<int> nodes{32, 50, 50, 50, 100, 50, 1};
	NN loopTimedNN(nodes, "bigNTimed");

	int randIndex = 0;
	//For loop for timing purposes, testing 4 boards chosen randomly
	int runs = 1000;
  	double answers[1000];
	start = clock();

	for(int i=0; i<runs; ++i){
		randIndex = dis(gen);
    	answers[i] = loopTimedNN.calculateBoard(boards[randIndex]);
	}
	stop = clock();
	duration = (stop - start) / (double) CLOCKS_PER_SEC;

  	std::uniform_int_distribution<int> dis1k(0,1000);
  	cout << "Random Board Value: " << answers[dis1k(gen)] << endl;


	cout << "Time Difference: " << duration << endl;

	double timePerBoard = duration/runs;
	cout << "Time per board: " << timePerBoard << endl;
	cout << "Board Evals per second: " << runs/duration << endl;

	REQUIRE(runs/duration > 10000); //at least 10,000 boards per sec
}

//Test Saving & Loading feature
TEST_CASE("Saving and Loading NN from file. ",
	"{32, 40, 10, 1}"){

	vector<int> nodes{32, 40, 10, 1};
	NN blondie24(nodes, "blondie");
	blondie24.saveToFile();
	blondie24.loadFromFile("blondie_NN_0.txt");

	NN copy = blondie24;
	copy.generation+=1;
	copy.saveToFile();

	ifstream file("blondie_NN_0.txt");
	ifstream file1("blondie_NN_1.txt");

	REQUIRE(checkIfFilesAreEqual(file, file1)==0);
}


// 	//To create NN & evaluate: Max Time (not loop) = 0.006 to 0.007 seconds = 6 miliseconds = 6 mil nanoseconds
// 	//If O2 flag = 11955.3 boards per sec (around 12,000)
// 		//179,329.5 boards in 15 sec

// Jason's hella old home computer:
// A Beast when new, a decade ago.
// No Opt: 1515.15 boards/second.
// O2: 15384.6 boards/second.
// O3: 16393.4


