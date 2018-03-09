//neuralNetwork.h
//Neural Network

#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <cmath>
using std::exp;

#include <random>
#include <vector>
using std::vector;

#include <chrono>
using std::time;

#include <iostream>
using std::cout;
using std::endl;
using std::getline;

#include <fstream>
using std::ofstream;
#include <string>
using std::string;
using std::to_string;
#include <sstream>
using std::istringstream;
using std::ifstream;

#include <algorithm>
using std::max;
using std::min;

#include "board.h"


unsigned const static int MAXMOVES = 16;
typedef double sNN;
constexpr sNN LOWEST = std::numeric_limits<sNN>::lowest();
constexpr sNN HIGHEST = std::numeric_limits<sNN>::max();

class AIPlayer{
public:
    double kingVal = 0.4;  //Value of kings over pawns.
    double sigma = 0.05; //How much we vary from the parent weights
    double pieceWeight = 1.0; //Value of piece counter input
    double montyWeight = 1.0;
    int montyK = 1; // (w-l)/(w+l+k)
    int generation = 0; //Starts out as parent
    string familyName = "";
    // Returns the value of the board given.
    // A virtual function to be implimented by the specific AI.
    virtual sNN calculateBoard(stdBoard & board){return 1.0;}

    // Returns a move given a board and a side.
    stdBoard getMove(stdBoard & board, bool side=false);
    // Beta side of the board
    sNN beta(stdBoard & board, int depth);
    //  Alpha side of the board
    sNN alpha(stdBoard & board, int depth);
    // Handles starting up the alpha-beta search
//    sNN alphabeta(stdBoard board, unsigned int side = 0);
};

class RandomPlayer: public AIPlayer {
public:
  RandomPlayer() {
    distro = std::uniform_real_distribution<double>(0.0,1.0);
    gen.seed(time(0));
  }
  std::mt19937_64 gen;
  std::uniform_real_distribution<double> distro;

  sNN calculateBoard(stdBoard & board) {
    sNN count = distro(gen);
    return count;
  }
  stdBoard getMove(stdBoard & board, bool side=false) {
    stdBoard possibleBoards[16];
    if(side) {
      board = board.flip();
    }
    int moveCount = board.genMoves(possibleBoards,0);
    if (moveCount == 0) {
        return stdBoard(0,0,0,0);
    }
    int moveSelect = int(distro(gen)*moveCount);

    if(side) {
      board = board.flip();
      possibleBoards[moveSelect] = possibleBoards[moveSelect].flip();
    }
    return possibleBoards[moveSelect];
  }
};

class PieceCount: public AIPlayer
{
public:
  PieceCount() {
    distro = std::uniform_real_distribution<double>(0.0,1.0);
    gen.seed(time(0));

  }
  std::mt19937_64 gen;
  std::uniform_real_distribution<double> distro;

  sNN calculateBoard(stdBoard & board) {
    sNN count = (int)(
          board.pieces[0].count() +
          board.pieces[2].count() -
          board.pieces[1].count() -
          board.pieces[3].count()) +
          distro(gen);
    return count;
  }
};

class NN: public AIPlayer
{
public:
	//Constructor setting a new neural network to random weights
	NN(std::vector<int>& nS, string familyname);
	//Given a board, calculate the output of the NN
	double calculateBoard(stdBoard & board);
	//Puts board input into nodes vector in user specified index
	void getBoardInput(stdBoard & board);
	double boardCount(stdBoard & board);
	//Given a number, calculate a sigmoid function output
	void sigmoid(double & num);
	//Save this NN to a file
	int saveToFile(string filename);

	//Load NN from file
	int loadFromFile(string filename);
	//generate offspring, randomize this NN
		//(assuming this is a copy of the original)
	void becomeOffspring();

	//Data Members
	std::vector<std::vector<double>> network;
    std::mt19937_64 gen;
    std::vector<std::vector<double>> nodes; //Count of nodes per layer
    std::vector<int> nodeSizes;
    std::vector<vector<double>> sigmas; //the change in weights

};

//Operator != for NN's for testing
bool operator!=(const NN & lhs, const NN & rhs);
bool operator==(const NN & lhs, const NN & rhs);

class NN2: public NN
{
public:
  void getBoardInput(stdBoard & board);
};

#endif /* NEURAL_NETWORK_H */



//Possible Configurations ----------

//Neural Network Weights ----
//Monte Carlo
//Alpha Beta = adapt winning weights
//Breeding = mix NN's weights together if they win

//Search Tree ----
//Trim = trim search tree for faster searching
//Don't search every board, pick (like 3) a constant and search those
	//as far down as you can
