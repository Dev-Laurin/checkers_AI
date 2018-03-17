//playgame.cpp
//3-12-18 Revised
//Play a game of NN's

#ifndef PLAYGAME
#define PLAYGAME

#include <iostream>
#include <vector>
using std::vector;
#include "board.h"
#include "neuralNetwork.h"

int playGame(AIPlayer & player1, AIPlayer & player2, vector<stdBoard> & gameBoards);
#endif /* playGame */
