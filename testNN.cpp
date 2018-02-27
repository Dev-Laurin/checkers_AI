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


//Check's if two NN save files are equal (binary)
	//disregarding generation numbers 
int checkIfFilesAreEqual(ifstream & file1, ifstream & file2){

  //read in generation number, but ignore it
  int gen, gen2; 
  file1.read(reinterpret_cast<char *>(&gen), sizeof(gen)); 
  file2.read(reinterpret_cast<char *>(&gen2), sizeof(gen2));

  int placeValue = 1000; 
  //read in the kingvalue 
  double kingVal, kingVal2; 
  file1.read(reinterpret_cast<char *>(&kingVal), sizeof(kingVal));
  file2.read(reinterpret_cast<char *>(&kingVal2), sizeof(kingVal2)); 

  if((int)(kingVal*placeValue)!=(int)(kingVal2*placeValue))
  	return -1; 

  //read in the overall size of the weights
  int weightSize, weightSize2; 
  file1.read(reinterpret_cast<char *>(&weightSize), sizeof(int)); 
  file2.read(reinterpret_cast<char *>(&weightSize2), sizeof(int));
 
  if(weightSize!=weightSize2)
  	return -1; 

  vector<vector<double>> network(weightSize2); 
  vector<vector<double>> network2(weightSize2); 

  vector<vector<double>> sigmas(weightSize2); 
  vector<vector<double>> sigmas2(weightSize2); 

  //read in size of individual vectors (2D)
  for(int i=0; i<weightSize; ++i){
    int size, size2; 
    file1.read(reinterpret_cast<char *>(&size), sizeof(int));
    file2.read(reinterpret_cast<char *>(&size2), sizeof(int));

    if(size!=size2)
    	return -1; 

    network.resize(size); 
    network2.resize(size); 
    sigmas.resize(size); 
    sigmas2.resize(size); 
  }


  //read in the actual weights 
  for(int i=0; i<network.size(); ++i){
    file1.read((char *)network[i].data(), network[i].size()*sizeof(double)); 
  	file2.read((char *)network2[i].data(), network2[i].size()*sizeof(double)); 
  	
  	if(network[i]!=network2[i])
  		return -1; 
  }

  //read in sigmas 
  for(int i=0; i<sigmas.size(); ++i){
    file1.read((char *)sigmas[i].data(), sigmas[i].size()*sizeof(double)); 
 	file2.read((char *)sigmas2[i].data(), sigmas2[i].size()*sizeof(double)); 
  	if(sigmas[i]!=sigmas2[i])
  		return -1; 
  }
  
  file1.close();
  file2.close(); 

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
	blondie24.loadFromFile("blondie_NN_0");

	NN copy = blondie24;
	copy.generation+=1;
	copy.saveToFile();

	ifstream file("NeuralNetworkFiles/blondie_NN_0.bin", std::ios::binary);
	ifstream file1("NeuralNetworkFiles/blondie_NN_1.bin", std::ios::binary);

	REQUIRE(file); 
	REQUIRE(file1); 
	//REQUIRE(file==file1); 

	REQUIRE(checkIfFilesAreEqual(file, file1)==0);
}

TEST_CASE("Test Loading NN file.", "{32, 40, 10, 1}"){
	
	vector<int> nodes{32, 40, 10, 1};
	NN blondie24(nodes, "blondie");

	NN copy = blondie24; 

	//Check saving files & loading
	blondie24.saveToFile();
	blondie24.becomeOffspring(); //change the NN 
	blondie24.saveToFile(); 
	blondie24.loadFromFile("blondie_NN_0");

	//Check the NN loading 
	REQUIRE(blondie24!=copy); 

	ifstream file("NeuralNetworkFiles/blondie_NN_0.bin", std::ios::binary); 
	ifstream file2("NeuralNetworkFiles/blondie_NN_1.bin", std::ios::binary); 

	REQUIRE(file); 
	REQUIRE(file2); 

	REQUIRE(checkIfFilesAreEqual(file, file2)!=0); 

}


TEST_CASE("Testing Normal Distribution Random Number Generator.",
	"std::normal_distribution<double> nDis(0, 1);"){

	std::mt19937_64 gen(time(0)); 
	//Mean = 0, StdDev = 1 
	std::normal_distribution<double> nDis(0, 1.0);
	//-1.0 -0.9 -0.8 -0.7 -0.6...
	//0 	1 	  2    3    4   
	//10 different buckets from 0.0 to 1.0 
	vector<int> buckets(20, 0); 

	//For a 1000 normal distributed random numbers 
	int runs = 100000; 
	for(int i=0; i<runs; ++i){

		double rand = nDis(gen);

		//If within our range
		if(rand>-1.0 and rand<1.0){ 

			int index = (int)((rand*10)); 

			//If negative 
			if(index<=0){
				index = (-index - 9)*-1;
			}
			else{//number is positive
				index = index + 9; 
			}
			++buckets[index]; 			
		}
	} 

	//Write numbers to a file 

	ofstream file("generatedTestFiles/normalDistributionNumbers.txt", std::ios::out); 
	for(int i=0; i<buckets.size(); i++){
		file << buckets[i] << endl; 
	}
	file.close(); 

	//Test sum all the numbers and see if they are in the correct percentage
	//for a standard deviation. 
	double sum =0; 
	for(int i=0; i<buckets.size(); ++i){
		sum+=buckets[i]; 
	}
	double percentage = sum/runs;
	REQUIRE(percentage>0.67); 
	REQUIRE(percentage<0.7); //should be around 68% 

}


//Calculate the new weights based on oldweights + random number
	//re-created with new weights 
TEST_CASE("Testing child generation.", "{32, 40, 10, 1}"){
	vector<int> nodes{32, 40, 10, 1};
	NN blondie24(nodes, "blondie_child_gen_test"); 
	NN child = blondie24; 


	//save the old weights
	vector<vector<double>> oldWeights(child.network.size()); 
 
	for(int i=0; i<child.network.size(); ++i){
		oldWeights[i].resize(child.network[i].size()); 

		for(int j=0; j<child.network[i].size(); ++j){
			oldWeights[i][j] = child.network[i][j]; 
		}
	}

	//save the old sigmas 
	vector<vector<double>> oldSigmas(child.sigmas.size()); 

	for(int i=0; i<child.sigmas.size(); ++i){
		oldSigmas[i].resize(child.sigmas[i].size()); 
		for(int j=0; j<child.sigmas[i].size(); ++j){
			oldSigmas[i][j] = child.sigmas[i][j]; 
		}
	}

	child.becomeOffspring(); 

	//test if each one is just an operation
	for(int i=0; i<child.network.size(); ++i){
		for(int j=0; j<child.network[i].size(); ++j){
			
			//Get new values 
			double newWeight = child.network[i][j]; 
			double newSig = child.sigmas[i][j];

			//Calculate tau
			double tau = (1.0/sqrt(2*sqrt(oldSigmas[i].size()))); 

			//Get what random numbers were 
			double randSigNum = log(newSig) / (tau*log(oldSigmas[i][j])); 
			double randWeightNum = (newWeight - oldWeights[i][j])/newSig;

			//calculate it to see if we get the same thing
			double testNewSig = pow(oldSigmas[i][j], tau*randSigNum); 
			double testNewWeight = 	oldWeights[i][j] + testNewSig *randWeightNum;

			int placeValue = 1000; 
			//Round to int (hundredth's place) for checking (doubles aren't accurate)
			REQUIRE((int)(child.network[i][j]*placeValue)==(int)(testNewWeight*placeValue));
			REQUIRE((int)(child.sigmas[i][j]*placeValue)==(int)(testNewSig*placeValue));  
		}
	}
}


//Test child generation values by getting the difference from the parent
//for each value then graphing (should be uniform dist?)
TEST_CASE("Testing child generation, with plotting random numbers.",
	"{32, 40, 10, 1}"){

	vector<int> nodes{32, 40, 10, 1};
	NN blondie24(nodes, "blondie_child_gen_test2"); 
	NN child = blondie24; 

	child.becomeOffspring(); 

	//Subtract parent values by child values to get difference

	//sigma difference 
	ofstream sigmaDifferencesFile("generatedTestFiles/sigmaDiff_childGenerationTest2.txt"); 

	for(int i=0; i<blondie24.sigmas.size(); ++i){
		for(int j=0; j<blondie24.sigmas[i].size(); ++j){
			sigmaDifferencesFile << blondie24.sigmas[i][j] - child.sigmas[i][j] << endl;
		}
	}

	//weight difference 
	//write to file for graphing into excel 
	ofstream weightDifferencesFile("generatedTestFiles/weightDifferences_childGenerationTest2.txt"); 

	for(int i=0; i<blondie24.network.size(); ++i){
		for(int j=0; j<blondie24.network[i].size(); ++j){
			weightDifferencesFile <<  blondie24.network[i][j] - child.network[i][j] << endl;
		} 
	}

}

