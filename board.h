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
		i32 const mask5U = 0x07070707; //Should be correct now.
		i32 const mask3U = 0x00E0E0E0; //dyslexia sucks. (beware of getting this backwards)
		cout << endl;
		int moveCount = 0;
		i32 mOpen = ~(red | black);  //bitwise OR, then compliment.  Shows valid open positions.
		// move by displacement 4, up
		i32 c4 = mOpen & mask4U;
		i32 move4 = (black >> 4) & c4;
		i32 mask4M = 17; // binary: 10001
		while (move4 > 0) {
			if (move4 & 1) {
				boardList[moveCount] = stdBoard(black ^ mask4M, red, blackK, redK);
				++moveCount;
			}
			move4 = move4 >> 1;
			mask4M = mask4M << 1;
		}
		// move by displacement 3, up
		i32 c3 = mOpen & mask3U;
		//stdBoard(c3, 0, 0, 0).draw();
		i32 move3 = (black >> 3) & c3;
		i32 mask3M = 9; // binary: 1001
		while (move3 > 0) {
			if (move3 & 1) {
				boardList[moveCount] = stdBoard(black ^ mask3M, red, blackK, redK);
				++moveCount;
			}
			move3 = move3 >> 1;
			mask3M = mask3M << 1;
		}
		// move by displacement 5, up
		i32 c5 = mOpen & mask5U;
		//stdBoard(c3, 0, 0, 0).draw();
		i32 move5 = (black >> 5) & c5;
		i32 mask5M = 33; // binary: 100001
		while (move5 > 0) {
			if (move5 & 1) {
				boardList[moveCount] = stdBoard(black ^ mask5M, red, blackK, redK);
				++moveCount;
			}
			move5 = move5 >> 1;
			mask5M = mask5M << 1;
		}

		return moveCount;
	}  
	stdBoard flipBoard();  //returns view of board from other side.
	
//->	//draws board on gui *for now is console----TODO
	void draw() {
		string boardString = str();
		for (int i = 0; i < 32; i+=8) {
			cout << "." << boardString[i] << "." << boardString[i + 1] << "." << boardString[i + 2] << "." << boardString[i + 3] << endl;
			cout << boardString[i + 4] << "." << boardString[i + 5] << "." << boardString[i + 6] << "." << boardString[i + 7] << "." << endl;
		}
	}
};

#endif /* RANDOM_CHECKERS_INCLUDED */ 