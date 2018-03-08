#ifndef OPPONENT_AI_H
#define OPPONENT_AI_H

#include <random>
#include <string>
#include "alphabeta.h"

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
			case 'g': {
			    stdBoard possibleBoards[32];
			    stdBoard tBoard = b.flip();
			    int moves = tBoard.genMoves(possibleBoards,0);
			    int selectMove = 0;
			    sNN moveVal = beta(possibleBoards[0],7);
			    sNN tempMove;
			    for (int i = 1;i<moves;++i) {
                    tempMove = beta(possibleBoards[i],7);
                    //cout << "moveVal: " << moveVal << " tempMove: " << tempMove << endl;
                    //cout << "Highest: " << HIGHEST << " Lowest: " << LOWEST << endl;
                    if (tempMove > moveVal) {
                        //better move found
                        moveVal = tempMove;
                        selectMove = i;
                    }
			    }
			    tBoard = possibleBoards[selectMove].flip();
			    return tBoard.str();
			}
			default:
				cout << "That AI Doesn't exist" << endl;
				return "Error";
		}
	}

	stdBoard getBoardMove(stdBoard&b){
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
			return possibleBoards[randMove];
			break;
		}
		case 'g': {
		    stdBoard possibleBoards[32];
		    stdBoard tBoard = b.flip();
		    int moves = tBoard.genMoves(possibleBoards,0);
		    int selectMove = 0;
		    sNN moveVal = beta(possibleBoards[0],7);
		    sNN tempMove;
		    for (int i = 1;i<moves;++i) {
                tempMove = beta(possibleBoards[i],7);
                //cout << "moveVal: " << moveVal << " tempMove: " << tempMove << endl;
                //cout << "Highest: " << HIGHEST << " Lowest: " << LOWEST << endl;
                if (tempMove > moveVal) {
                    //better move found
                    moveVal = tempMove;
                    selectMove = i;
                }
		    }
		    tBoard = possibleBoards[selectMove].flip();
		    return tBoard;
		}
		default:
			cout << "That AI Doesn't exist" << endl;
			stdBoard error; 
			return error;
		}
	}

	char AIType_;
};

#endif /* OPPONENT_AI_H */
