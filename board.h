#ifndef RANDOM_CHECKERS_INCLUDED
#define RANDOM_CHECKERS_INCLUDED

#include <stdint.h>
#include <string>
using std::string;
#include <iostream>
using std::cout; 
using std::endl; 

typedef uint32_t i32; //unsigned 32bit int.  Used to store checkers board.

//changed the defining of individual board sections as indepedent ints
//to an array. By using defines, I maintain compatibility

#define black pieces[0] //black pieces - pawns and kings
#define red pieces[1] //red pieces - pawns and kings
#define blackK pieces[2] //black kings
#define redK pieces[3] //red kings

class stdBoard {
public:
	i32 pieces[4];
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
			if (pieces[3] & (1 << i )) {
				board.append("R");
			} else if (pieces[2] & ( 1 << i )) {
				board.append("B");
			} else if (pieces[1] & ( 1 << i )) {
				board.append("r");
			} else if (pieces[0] & ( 1 << i )) {
				board.append("b");
			} else {
				board.append(" ");
			}
		}
		return board;
	}  
	int genJumps(stdBoard boardList[], int side = 0) {
		//up left 1, up right 1, up left 2, down right 2
		i32 const maskM[4] = { 0x00070707 };
		i32 mOpen = ~(red | black); //open board spots marked with a 1
		int moveCount = 0;
		//Up&left
		i32 moves[4];
		moves[0] = maskM[0] & mOpen & (pieces[1 - side] >> 5) & (pieces[side] >> 9); //up left1
		
		for(int i = 0; moves[0] > 0; ++i) {
			if ((1 & moves[0])==0) {
				stdBoard jmp = *this;
				jmp.draw();
				jmp.pieces[1 - side] = pieces[1 - side] ^ (1 << (5 + i));
				jmp.pieces[side] = pieces[side] ^ (1 << (9 + i) | 1 << i);
				jmp.draw();
				
			}
			moves[0] = moves[0] >> 1;
		}

		return 0;
	}
	//generateMoves - fills an array/vector with the valid moves possible on the board.
	//Returns false if no valid moves found.
	//todo: Add jump flag. (int * jump)?
	int genMoves(stdBoard boardList[], int side = 0) {//boardlist is previously allocated vector/array for boards to be stored in.  Return is number of moves found.
		//magic numbers.  Mask of which places have a valid move, up 4, up 3, up 5 positions.
		int mUp;
		int mDown;
		if (side) {
			mUp = 3; // redKing
			mDown = 1; // redPawn

		}
		else {
			mUp = 0; // blackPawn
			mDown = 2; // blackKing
		}
		i32 const maskU[2][3] = { { 0x00E0E0E0,0xFFFFFFFF,0x07070707},{ 0x07070700,0xFFFFFFFF,0xE0E0E0E0 } };
		i32 const maskM[3] = { 9,17,33 };
		int moveCount = 0;
		i32 mOpen = ~(pieces[0] | pieces[1]);  //bitwise OR, then compliment.  Shows valid open positions.
		//Black King Moves
		//Red Pawn Moves
		for (int i = 0; i < 3; ++i) {
			i32 moves = (pieces[mDown]) & ((mOpen & maskU[1][i]) >> (i + 3));
			i32 mask = maskM[i];
			while (moves > 0) {
				if (moves & 1) {
					boardList[moveCount] = *this; //copy the existing board.
					boardList[moveCount].pieces[mUp & 1] = pieces[mUp & 1] ^ mask; //apply the move. 
					//bitwise and with 1 makes it always move the pawn.

					//Move is noted on pawn board, need to move king as well.
					//change is to allow use for red pawns.
					i32 king = boardList[moveCount].pieces[mUp | 2] & (~boardList[moveCount].pieces[mUp & 1]);
					boardList[moveCount].pieces[mUp | 2] = (boardList[moveCount].pieces[mUp | 2] ^ king) ^ (king << (i + 3));
					++moveCount;
				}
				moves = moves >> 1;
				mask = mask << 1;
			}
		}
		//Black Pawn Moves.
		//Red King Moves
		for (int i = 0; i < 3; ++i) {
			i32 moves = (pieces[mUp] >> (i+3)) & (mOpen & maskU[0][i]);
			i32 mask = maskM[i];
			while (moves > 0) {
				if (moves & 1) {
					boardList[moveCount] = *this;
					boardList[moveCount].pieces[mUp & 1] = pieces[mUp & 1] ^ mask;
					//Need to move the king as well, if it is one.
					//Take the complement of the black list and 'AND' with king list.
					//should be all zero.  Otherwise, king to move.
					i32 king = boardList[moveCount].pieces[mUp | 2] & (~boardList[moveCount].pieces[mUp & 1]);
					boardList[moveCount].pieces[mUp | 2] = (boardList[moveCount].pieces[mUp | 2] ^ king) ^ (king >> (i + 3));

					//i32 king = boardList[moveCount].blackK & (~boardList[moveCount].black);
					//move the king.  Note: No if statements!
					//boardList[moveCount].blackK = (boardList[moveCount].blackK ^ king) ^ (king >> (i+3));
					++moveCount;
				}
				moves = moves >> 1;
				mask = mask << 1;
			}
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