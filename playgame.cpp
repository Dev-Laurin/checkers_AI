//playgame.cpp
//3-6-18 Revised
//Play a game of NN's

#include <iostream>
#include <vector>
using std::vector;
#include "board.h"
//#include "opponent_AI.h"
#include "neuralNetwork.h"

int main(){
	vector<int> nodes{32, 40, 10, 1};
	NN blondie24(nodes, "blondie24");

	vector<int> nodes2{32, 91, 50, 1};
	NN other(nodes2, "other");

	//start a new game
	stdBoard b;

	//First 3 ply are random
	RandomPlayer RPlayer;

	//200 moves max
	b.draw();
  b = RPlayer.getMove(b,0);
  b.draw();
  b = RPlayer.getMove(b,1);
  b.draw();
  b = RPlayer.getMove(b,0);
  b.draw();

	for(int i=0; i<200; ++i){
	}
	return 0;
}
