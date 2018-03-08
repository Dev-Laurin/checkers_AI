//playgame.cpp 
//3-6-18 Revised 
//Play a game of NN's

#include <iostream>
#include "neuralNetwork.h"
#include <vector> 
using std::vector; 
#include "board.h"
#include "opponent_AI.h"

int main(){
	vector<int> nodes{32, 40, 10, 1};
	NN blondie24(nodes, "blondie24");

	vector<int> nodes2{32, 91, 50, 1}; 
	NN other(nodes2, "other"); 

	//start a new game 
	stdBoard b; 

	//First 3 ply are random
	Opponent random('r'); 

	//200 moves max 
	for(int i=0; i<200; ++i){

		//other start first 
		if(i<3){
			//random moves first 
			b = random.getBoardMove(b); 
		}

		

	}
	return 0; 
}
