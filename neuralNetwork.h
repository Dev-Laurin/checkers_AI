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
using std::time_t;

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

#include<unordered_map>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


#include "board.h"



#ifndef CONSTS
#define CONSTS
typedef double sNN;
unsigned const static int MAXMOVES = 32;
constexpr sNN LOWEST = -10000;
constexpr sNN HIGHEST = 10000;
#endif // CONSTS

const static double MOVETIME = 14.0;  //Seconds.
extern std::mt19937_64 gen;

class AIPlayer{
public:
    double kingVal = 0.4;  //Value of kings over pawns.
    double sigma = 0.05; //How much we vary from the parent weights
    double pieceWeight = 1.0; //Value of piece counter input
    double montyWeight = 1.0;
    int montyK = 1; // (w-l)/(w+l+k)
    int generation = 0; //Starts out as parent
    string familyName = "";
    unsigned int searchDepth = 6;
    time_t timeStart;
    time_t timeLimit;
    bool timeExceeded = false;
    int depthReached = 0;
    std::unordered_map<stdBoard, double> boardMem;
    unsigned int cacheHit = 0;
    unsigned int cacheMiss = 0;
    unsigned int trimTotal = 0;

    //For pruning during tournament
    int wins = 0;
    int losses = 0;
    int draws = 0;

    // Returns the value of the board given.
    // A virtual function to be implimented by the specific AI.
    virtual sNN calculateBoard(stdBoard & board) = 0 ;

    // Returns a move given a board and a side.
    virtual stdBoard getMove(stdBoard & board, bool side=false);
    // Beta side of the board
    sNN beta(stdBoard & board, int depth, sNN a, sNN b);
    //  Alpha side of the board
    sNN alpha(stdBoard & board, int depth, sNN a, sNN b);
    // Handles starting up the alpha-beta search
    // sNN alphabeta(stdBoard board, unsigned int side = 0);

    void prntStats();

    //iterative  deepening search, time limited.
    stdBoard IntDeepSearch(stdBoard & board, bool side=false);
    // Clamp function, not part of C++ until 18!
    double clmp(double x, double a, double b);
    //Given a number, calculate a sigmoid function output
    void sigmoid(double & num);
    virtual ~AIPlayer()=default; //base class, needs to have virtual destructor.

    //serialization
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & kingVal;
        ar & sigma;
        ar & pieceWeight;
        ar & montyWeight;
        ar & montyK;
        ar & generation;
        ar & familyName;
        ar & boardMem;
        ar & wins;
        ar & losses;
        ar & draws;
    }
};

class RandomPlayer: public AIPlayer {
public:
  RandomPlayer() {
    distro = std::uniform_real_distribution<double>(0.0,1.0);
  }
  std::uniform_real_distribution<double> distro;

  sNN calculateBoard(stdBoard & board) {
    sNN count = distro(gen);
    cout << "I'm Random!" << endl;
    return count;
  }
  virtual stdBoard getMove(stdBoard & board, bool side=false);
};

class PieceCount: public AIPlayer
{
public:
  PieceCount() {
    distro = std::uniform_real_distribution<double>(0.0,1.0);

  }
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

  int saveToFile(string filename); 
  int loadFromFile(string filename); 

	//generate offspring, randomize this NN
		//(assuming this is a copy of the original)
	void becomeOffspring();

	//Data Members
	std::vector<std::vector<double>> network;
  std::vector<std::vector<double>> nodes; //Count of nodes per layer
  std::vector<int> nodeSizes;
  std::vector<vector<double>> sigmas; //the change in weights
  virtual ~NN()=default;
  template <typename Archive>
  void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<AIPlayer>(*this);
        ar & network;
        ar & nodes;
        ar & nodeSizes;
        ar & sigmas;
    }
};

//Operator != for NN's for testing
bool operator!=(const NN & lhs, const NN & rhs);
bool operator==(const NN & lhs, const NN & rhs);

class NN2: public NN
{
public:
  NN2(std::vector<int>& nS, string familyname) : NN(nS, familyname){}
  void getBoardInput(stdBoard & board);
};

bool operator!=(const NN2 & lhs, const NN2 & rhs);
bool operator==(const NN2 & lhs, const NN2 & rhs);

// //Global Save to File Functions
// //Save this NN to a file
// int saveToFile(const NN & nn, string filename);
int saveToFile(const NN & nn, string filename);
// //Load NN from file
// //Filename = FamilyName/GEN#
// //EX:  Blondie24/GEN100/NNpp
// int loadFromFile(NN & nn, string filename);
int loadFromFile(NN& nn, string filename); 

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
