


#ifndef PLAYMOVE_H_INCLUDED
#define PLAYMOVE_H_INCLUDED

#include <algorithm>
#include <limits>
#include <thread>
#include <ctime>

#define NETPRECISION double
#define TIMELIMIT 2
#define MAXTIME 15

void PrintBoard(char Board[32]);
double MaxMove (NeuralNet & Player, char Board[32], int depth, NETPRECISION alpha=std::numeric_limits<double>::min(), NETPRECISION beta=std::numeric_limits<double>::max());
double MinMove (NeuralNet & Player, char Board[32], int depth, NETPRECISION alpha=std::numeric_limits<double>::min(), NETPRECISION beta=std::numeric_limits<double>::max());
bool PlayMove(NeuralNet & Player, std::string & Boardstr, int playDepth);


#endif

/*
BoardScores[]
BoardOrder[]

*/