#ifndef RANDOM_CHECKERS_INCLUDED
#define RANDOM_CHECKERS_INCLUDED

#include <stdint.h>
#include <string>
using std::string;

typedef int32_t i32;

class stdBoard {
public:
	i32 black;  //black pieces - pawns and kings
	i32 red;  //red pieces - pawns and kings
	i32 blackK; //black kings
	i32 redK;  //red kings
	void stdBoard() { //Initial regular board
		black = 4095; // last 12 numbers 1
		red = 4293918720; // first 12 numbers 1
		blackK = 0; 
		redK = 0;
	}
	void stdBoard(string board) {
		black = 0;
		red = 0;
		blackK = 0;
		redK = 0;
		for (int i = 0; i < 32; ++i) {
			if (board[i] == 'r') {
				//using bitshifting rather than power.
				red += 1 << (31 - i);
			}
			if (board[i] == 'R') {
				red += 1 << (31 - i);
				redK += 1 << (31 - i);
			}
			if (board[i] == 'b') {
				//using bitshifting rather than power.
				black += 1 << (31 - i);
			}
			if (board[i] == 'B') {
				black += 1 << (31 - i);
				blackK += 1 << (31 - i);
			}
		}
	}
	void stdBoard(i32 b, i32 r, i32 bK, i32 rK) {
		black = b;
		red = r;
		blackK = bK;
		redK = rK;
	}
	string strBoard();  //returns string representation of a board.
	//generateMoves - fills an array/vector with the valid moves possible on the board.
	//Returns false if no valid moves found.
	bool generateMoves(stdBoard boardList[]);  //boardlist is previously allocated vector/array for boards to be stored in.  Return is false if no moves
	strdBoard flipBoard();  //returns view of board from other side.
};

#endif /* RANDOM_CHECKERS_INCLUDED */ 