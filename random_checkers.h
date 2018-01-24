#ifndef RANDOM_CHECKERS_INCLUDED
#define RANDOM_CHECKERS_INCLUDED

#include <stdint.h>
#include <string>
using std::string;
#include <iostream>
using std::cout; 
using std::endl; 

typedef uint32_t i32; //unsigned 32bit int.  Used to store checkers board.

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
	//todo: Add jump flag. (int * jump)?
	int genMoves(stdBoard boardList[], int side = 0) {//boardlist is previously allocated vector/array for boards to be stored in.  Return is number of moves found.
		//magic numbers.  Mask of which places have a valid move, up 4, up 3, up 5 positions.
		i32 const mask4U = 0xFFFFFFF0;
		i32 const mask3U = 0x00E0E0E0; //needs to be recalced.
		i32 const mask5U = 0x07070707; //needs to be recalced.
		int moveCount = 0;
		i32 mOpen = ~(red | black);  //bitwise OR, then compliment.  Shows valid open positions.
		// move by displacement 4, up
		i32 c4 = cBoard & mask4U;

		
		return 0;
	}  
	stdBoard flipBoard();  //returns view of board from other side.
	
//->	//draws board on gui *for now is console----TODO
	void draw() {
		string boardString = str();
		for (int i = 0; i < 32; i+=8) {
			cout << " " << boardString[i] << " " << boardString[i + 1] << " " << boardString[i + 2] << " " << boardString[i + 3] << endl;
			cout << boardString[i + 4] << " " << boardString[i + 5] << " " << boardString[i + 6] << " " << boardString[i + 7] << " " << endl;
		}
	}
};

#endif /* RANDOM_CHECKERS_INCLUDED */ 