#ifndef OPPONENT_AI_H
#define OPPONENT_AI_H

#include <random>
#include <string> 

class Opponent{
public: 
	//Give a char representing a type of AI 
	Opponent(char AIType){
		AIType_ = AIType; 
	}
	//Give the gui the move that was chosen 
	std::string getMove(stdBoard&b){
		switch(AIType_){
			//Random Move Picker AI
			case 'r': {
				//get random number
				stdBoard possibleBoards[30];
				int moves = b.genMoves(possibleBoards,1); //red
				int randMove;

				if(moves==1){
					randMove = 0;
				}
				else{
					std::mt19937 gen(time(0)); //seed
					std::uniform_int_distribution<int> dis(0,moves-1);
					randMove = dis(gen);
				}
				return possibleBoards[randMove].str(); 
				break;
			}
			default: 
				cout << "That AI Doesn't exist" << endl; 
				return "Error"; 
		}
	}

	char AIType_; 
}; 

#endif /* OPPONENT_AI_H */ 