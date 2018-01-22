#ifndef RANDOM_CHECKERS_INCLUDED
#define RANDOM_CHECKERS_INCLUDED

#include <stdint.h>
#include <string>
using std::string;
#include <iostream>
using std::cout; 
using std::endl; 

typedef int32_t i32;

class stdBoard {
public:
	i32 black;  //black pieces - pawns and kings
	i32 red;  //red pieces - pawns and kings
	i32 blackK; //black kings
	i32 redK;  //red kings
	stdBoard() { //Initial regular board
		black = 4293918720; // last 12 numbers 1
		red = 4095; // first 12 numbers 1
		blackK = 0; 
		redK = 0;
	}
	stdBoard(string board) {
		black = 0;
		red = 0;
		blackK = 0;
		redK = 0;
		for (int i = 0; i < 32; ++i) {
			if (board[i] == 'r') {
				//using bitshifting rather than power.
				red += 1 << i;
			}
			if (board[i] == 'R') {
				red += 1 << i;
				redK += 1 << i;
			}
			if (board[i] == 'b') {
				//using bitshifting rather than power.
				black += 1 << i;
			}
			if (board[i] == 'B') {
				black += 1 << i;
				blackK += 1 << i;
			}
		}
	}
	stdBoard(i32 b, i32 r, i32 bK, i32 rK) {
		black = b;
		red = r;
		blackK = bK;
		redK = rK;
	}

	//returns string representation of a board.
	string str(){
		string board;
		for (int i = 0;i<32;++i) {
			if (redK & (1 << i )) {
				board.append("R");
			} else if (blackK & ( 1 << i )) {
				board.append("B");
			} else if (red & ( 1 << i )) {
				board.append("r");
			} else if (black & ( 1 << i )) {
				board.append("b");
			} else {
				board.append(" ");
			}
		}
		return board;
		//return " b b b bb b b b"; 
	}  
	//generateMoves - fills an array/vector with the valid moves possible on the board.
	//Returns false if no valid moves found.
	bool generateMoves(stdBoard boardList[]);  //boardlist is previously allocated vector/array for boards to be stored in.  Return is false if no moves
	stdBoard flipBoard();  //returns view of board from other side.
	
//->	//draws board on gui *for now is console----TODO
	void draw(){
		string boardString = str();

		for(int i=0; i<boardString.size(); ++i){
			if(i%8==0)
				cout << endl; 
			
			cout << boardString[i]; 
		} 
		
	}
};

#endif /* RANDOM_CHECKERS_INCLUDED */ 