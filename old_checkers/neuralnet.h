/*
neuralnet.h
Jason Bright
Neural Net header
20160215 

*/


#ifndef NEURALNET_H_INCLUDED
#define NEURALNET_H_INCLUDED

#include <vector>
#include <string>
#include <sstream>
#include <new>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <random>
#include <cmath>
#include <fstream>
#include <algorithm>

#define MAXBOARDS 20  //Experiments have shown that the maximum number of moves returned is 17; I'm maintaining some overhead.
#define BOARDSIZE 32
#define REDPAWN 'r'
#define BLACKPAWN 'b'
#define REDKING 'R'
#define BLACKKING 'B'
#define EMPTYCELL '_'
#define VALIDMOVEWIDTH 8
#define NETPRECISION double
#define STDMUTATEFACT 1.0
#define INITKINGVAL 1.4
#define INITPIECEVAL 1.0
#define INITMUTATE	.25

double sigmoid(double x, double b);


class NeuralNet
{
//private:
public:
	int Nlayers;  //holds number of layers
	int NumFact=0; //Number of factors in Net.
	int Generation=1; //shows the generation of the neural network
	int Place=1; // Where it's ranked.
	int Score=0; // Wins, losses, and such are tracked here.
	int PieceCount=0; //Used in setInputs to calculate relative piecewise standing.
	std::vector<int> neuronCount; //holds number of neurons per layer
	std::vector<std::vector<NETPRECISION>> NeuralLayer; //Holds values of nodes
	std::vector<std::vector<NETPRECISION>> Dendrites; //Holds values of dendrites
	std::vector<std::vector<NETPRECISION>> DendriteStability; //Holds the 'mutability' of dendrites
	double KingValue = 1.4; // Value of Kings
	double KingMutate = INITMUTATE; //MutateFactor
	double PieceValue = 1; //value of the piececount
	double PieceMutate = INITMUTATE; //MutateFactor for piececount.

public:
	// No default ctor for NeuralNet
	//NeuralNet() = delete;
	//NeuralNet();
	// Ctor for NeuralNet
	NeuralNet();
	NeuralNet(int layers,int neurons[]);
	
	// Dtor
	//~NeuralNet(); //Not needed at moment.
	
	// Fill the weights
	void RandomizeDendrites();

// Set the function to be used by all neurons on the specified layer.

//Sets the inputs at layer 0 of the neural net
	void setInputs(char sBoard[]);

// Test the given input on the current neural net.
	NETPRECISION Impulse();

//Outputs the neural network to file, ready to paste into Skynet for testing.
	void Print(std::string fileName = "NeuralNet.txt");
//Outputs the neural network to a binary file.
	bool Save(std::string fileName="");
	bool Load(std::string fileName="");
//Mutates the network
	void Evolve(NETPRECISION mutateFactor=STDMUTATEFACT);
};


#endif