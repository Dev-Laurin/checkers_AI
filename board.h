#ifndef BOARD_INCLUDED
#define BOARD_INCLUDED

#include <stdint.h>
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <array>
#include <bitset>
using std::bitset;

#include <boost/functional/hash.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/bitset.hpp>

#include<unordered_map>



//typedef uint32_t i32; //unsigned 32bit int.  Used to store checkers board.
typedef bitset<32> i32;


//changed the defining of individual board sections as indepedent ints
//to an array. By using defines, I maintain compatibility

class cmpBoard;

class stdBoard {
public:
    i32 pieces[4];
	//Constructors
	stdBoard() {
	  pieces[0] = 0xFFF00000;//Initial regular board
		pieces[1] = 0x00000FFF;
		pieces[2] = 0x00000000;
		pieces[3] = 0x00000000;
	}
	stdBoard(string board) {
	  pieces[0] = 0x00000000;//Initial regular board
		pieces[1] = 0x00000000;
		pieces[2] = 0x00000000;
		pieces[3] = 0x00000000;
		for (int i = 0; i < 32; ++i) {
			if (board[i] == 'r') {
				//using bitshifting rather than power.
				pieces[1][i] = 1;
			}
			if (board[i] == 'R') {
				pieces[1][i] = 1;
				pieces[3][i] = 1;
			}
			if (board[i] == 'b') {
				//using bitshifting rather than power.
				pieces[0][i] = 1;
			}
			if (board[i] == 'B') {
				pieces[0][i] = 1;
				pieces[2][i] = 1;
			}
		}
	}
	stdBoard(i32 b, i32 r, i32 bK, i32 rK) {
		pieces[0] = b;
		pieces[1] = r;
		pieces[2] = bK;
		pieces[3] = rK;
	}

	//returns string representation of a board.
	string str(){
		string board;
		for (int i = 0;i<32;++i) {
			if (pieces[3][i]) {
				board.append("R");
			} else if (pieces[2][i]) {
				board.append("B");
			} else if (pieces[1][i]) {
				board.append("r");
			} else if (pieces[0][i]) {
				board.append("b");
			} else {
				board.append(" ");
			}
		}
		return board;
	}

	void updateBoard(string board) {
		for (int i = 0;i<4;++i) {
			pieces[i] = 0;
		}
		for (int i = 0; i < 32; ++i) {
			if (board[i] == 'r') {
				//using bitshifting rather than power.
				pieces[1][i] = 1;
			}
			if (board[i] == 'R') {
        pieces[1][i] = 1;
        pieces[3][i] = 1;
			}
			if (board[i] == 'b') {
				//using bitshifting rather than power.
				pieces[0][i] = 1;
			}
			if (board[i] == 'B') {
        pieces[0][i] = 1;
        pieces[2][i] = 1;
			}
		}
	}

	//KingMaker ensures kings are in place.  Simply call function.
	void kingMaker() {
		i32 rowBlack = 0x0000000F;
		i32 rowred_p = 0xF0000000;
		pieces[2] = pieces[2] | (pieces[0] & rowBlack);
		pieces[3] = pieces[3] | (pieces[1] & rowred_p);
		return;
	}


	//genJumps - generate any boards with jumps.
	//currently not fully functional.
	//The last 2 variables are to enable recursion on the jump by restricting valid moves.
	//maskP is either all F, indicating all positions valid, or the location of the piece just moved.
	//consideration:  keep maskP out of this version, create copy.
	//test later in optimization.
	int genJumps(stdBoard boardList[], int side = 0, int moveCount = 0, i32 maskP = 0xFFFFFFFF) {
		//up left 1, up right 1, up left 2, down right 2
		i32 const maskM[2][4] = { { 0x00707070,0x00070707,0x00E0E0E0,0x000E0E0E },
			{ 0x0E0E0E00,0xE0E0E000,0x07070700,0x70707000 } };

		i32 mOpen = ~(pieces[0] |pieces[1]); //open board spots marked with a 1
		//Up&left,Up%right
		i32 moves[4];
		//Jumping UP
		for (int j = 0; j < 2; ++j) {
			// side | side << 1 leaves the value at 0 if zero, if 1 changes it to 11(king pieces)
			moves[j] = maskM[0][j] & mOpen & (pieces[1 - side] >> (j + 4)) & ((maskP & pieces[side | side << 1]) >> 9); //up left
			for (int i = 0; moves[j] != 0; ++i) {
				if (moves[j][0]) {
					stdBoard jmp = *this;
					//remove the jumped piece.
					jmp.pieces[1 - side][j+i+4] = 0;
					//remove the king if necessary:
					jmp.pieces[(1-side) | 2][j+i+4] = 0;
					//Move the jump piece
					//move the king symbol first
					jmp.pieces[side|2][i] = jmp.pieces[side|2][i+9];
					jmp.pieces[side|2][i+9] = 0;
					jmp.pieces[side][i+9] = 0;
					jmp.pieces[side][i] = 1;

					//Check for new move.  If it returns additional moves, it means further jumps were possible.
					//don't add this board in that case.
					int newMoves;
					newMoves = jmp.genJumps(boardList, side, moveCount, (1 << i));
					if (newMoves > moveCount) {
						moveCount = newMoves;
					}
					else {  //No additional boards returned,
						boardList[moveCount] = jmp;
						boardList[moveCount].kingMaker();
						++moveCount;
					}
				}
				moves[j] = moves[j] >> 1;
			}
		}
		for (int j = 2; j < 4; ++j) {
			moves[j] = maskM[0][j] & mOpen & (pieces[1 - side] >> (j + 1)) & ((maskP & pieces[side | side << 1]) >> 7);  //up right
			for (int i = 0; moves[j] != 0; ++i) {
				if (moves[j][0]) {
					stdBoard jmp = *this;
					//remove the jumped piece.
					jmp.pieces[1 - side][j+i+1] = 0;
					jmp.pieces[(1-side)|2][j+i+1] = 0;
					//Move the piece
          jmp.pieces[side|2][i] = jmp.pieces[side|2][i+7];
					jmp.pieces[side|2][i+7] = 0;
					jmp.pieces[side][i+7] = 0;
					jmp.pieces[side][i] = 1;

					//move the king symbol.
					//Check for more jumps.
					int newMoves;
					newMoves = jmp.genJumps(boardList, side, moveCount, (1 << i));
					if (newMoves > moveCount) {
						moveCount = newMoves;
					}
					else {  //No additional boards returned,
						boardList[moveCount] = jmp;
						boardList[moveCount].kingMaker();
						++moveCount;
					}

				}
				moves[j] = moves[j] >> 1;
			}
		}
		//Jumping DOWN
		for (int j = 0; j < 2; ++j) {
			// 2 >> side is to specificy king
			moves[j] = maskM[1][j] & mOpen & (pieces[1 - side] << (j + 4)) & ((maskP & pieces[2 >> side]) << 9); //down right
			for (int i = 0; moves[j] != 0; ++i) {
				if (moves[j][0]) {
					stdBoard jmp = *this;
					//remove the jumped piece.
					jmp.pieces[1 - side][i-j-4] = 0;
					jmp.pieces[(1-side) | 2][i-j-4] = 0;

					//move the jump piece
          jmp.pieces[side|2][i] = jmp.pieces[side|2][i-9];
					jmp.pieces[side|2][i-9] = 0;
					jmp.pieces[side][i-9] = 0;
					jmp.pieces[side][i] = 1;

					//Check for more jumps.
					int newMoves;
					newMoves = jmp.genJumps(boardList, side, moveCount, (1 << i));
					if (newMoves > moveCount) {
						moveCount = newMoves;
					}
					else {  //No additional boards returned,
						boardList[moveCount] = jmp;
						boardList[moveCount].kingMaker();
						++moveCount;
					}

				}
				moves[j] = moves[j] >> 1;
			}
		}
		for (int j = 2; j < 4; ++j) {
			moves[j] = maskM[1][j] & mOpen & (pieces[1 - side] << (j + 1)) & ((maskP & pieces[2 >> side]) << 7); //down left
			for (int i = 0; moves[j] != 0; ++i) {
				if (moves[j][0]) {
					stdBoard jmp = *this;
					//remove the jumped piece.
					jmp.pieces[1 - side][i-j-1] = 0;
					jmp.pieces[(1 - side) | 2] = 0;
					//move the jump piece
          jmp.pieces[side|2][i] = jmp.pieces[side|2][i-7];
					jmp.pieces[side|2][i-7] = 0;
					jmp.pieces[side][i-7] = 0;
					jmp.pieces[side][i] = 1;

					//Check for more jumps.
					int newMoves;
					newMoves = jmp.genJumps(boardList, side, moveCount, (1 << i));
					if (newMoves > moveCount) {
						moveCount = newMoves;
					}
					else {  //No additional boards returned,
						boardList[moveCount] = jmp;
						boardList[moveCount].kingMaker();
						++moveCount;
					}

				}
				moves[j] = moves[j] >> 1;
			}
		}
		return moveCount;
	}
	//generateMoves - fills an array/vector with the valid moves possible on the board.
	//Returns false if no valid moves found.
	//todo: Add jump flag. (int * jump)?
	int genMoves(stdBoard boardList[], int side = 0) {//boardlist is previously allocated vector/array for boards to be stored_p in.  Return is number of moves found.
		//magic numbers.  Mask of which places have a valid move, up 4, up 3, up 5 positions.

		i32 const maskU[2][3] = { { 0x00E0E0E0,0xFFFFFFFF,0x07070707},{ 0x07070700,0xFFFFFFFF,0xE0E0E0E0 } };
		i32 const maskM[3] = { 9,17,33 };
		int moveCount = 0;
		moveCount = genJumps(boardList, side);
		if (moveCount) { //Jump is available, have to take it.
			return moveCount;
		}
		i32 mOpen = ~(pieces[0] | pieces[1]);  //bitwise OR, then compliment.  Shows valid open positions.
		//Black King Moves
		//red_p Pawn Moves
		for (int i = 0; i < 3; ++i) {
			i32 moves = (pieces[2 >> side]) & ((mOpen & maskU[1][i]) >> (i + 3));
			i32 mask = maskM[i];
			while (moves != 0) {
				if (moves[0]) {
					boardList[moveCount] = *this; //copy the existing board.
					boardList[moveCount].pieces[side] = pieces[side] ^ mask; //apply the move.
					//bitwise and with 1 makes it always move the pawn.

					//Move is noted on pawn board, need to move king as well.
					//change is to allow use for red_p pawns.
					i32 king = boardList[moveCount].pieces[side | 2] & (~boardList[moveCount].pieces[side]);
					boardList[moveCount].pieces[side | 2] = (boardList[moveCount].pieces[side | 2] ^ king) ^ (king << (i + 3));
					boardList[moveCount].kingMaker();
					++moveCount;
				}
				moves = moves >> 1;
				mask = mask << 1;
			}
		}
		//Black Pawn Moves.
		//red_p King Moves
		for (int i = 0; i < 3; ++i) {
			i32 moves = (pieces[side | side << 1] >> (i+3)) & (mOpen & maskU[0][i]);
			i32 mask = maskM[i];
			while (moves != 0) {
				if (moves[0]) {
					boardList[moveCount] = *this;
					boardList[moveCount].pieces[side] = pieces[side] ^ mask;
					//Need to move the king as well, if it is one.
					//Take the complement of the black list and 'AND' with king list.
					//should be all zero.  Otherwise, king to move.
					i32 king = boardList[moveCount].pieces[side | 2] & (~boardList[moveCount].pieces[side]);
					boardList[moveCount].pieces[side | 2] = (boardList[moveCount].pieces[side | 2] ^ king) ^ (king >> (i + 3));

					//i32 king = boardList[moveCount].blackK & (~boardList[moveCount].black);
					//move the king.  Note: No if statements!
					//boardList[moveCount].blackK = (boardList[moveCount].blackK ^ king) ^ (king >> (i+3));
					boardList[moveCount].kingMaker();
					++moveCount;
				}
				moves = moves >> 1;
				mask = mask << 1;
			}
		}
		/*
		if (moveCount > 16) {
      cout << "High MoveCount! Moves = " << moveCount << endl;
		}
		*/
		return moveCount;
	}
	stdBoard flip() {
        stdBoard temp;
        for(int i = 0;i<32;++i) {
            temp.pieces[0][i] = pieces[1][31-i];
            temp.pieces[1][i] = pieces[0][31-i];
            temp.pieces[2][i] = pieces[3][31-i];
            temp.pieces[3][i] = pieces[2][31-i];
        }
        return temp;
	}

	string flipBoard()
	{
		string oldBoard = str();
		string newBoard = "";
		for(int i=oldBoard.length()-1; i>=0; i--){
			newBoard+=oldBoard[i];
		}
		return newBoard;
	}  //returns view of board from other side.

	void draw() {
		string boardString = str();
		const static char wh = 177;
		for (int i = 0; i < 32; i+=8) {
			cout << wh << boardString[i] << wh << boardString[i + 1] << wh << boardString[i + 2] << wh << boardString[i + 3] << endl;
			cout << boardString[i + 4] << wh << boardString[i + 5] << wh << boardString[i + 6] << wh << boardString[i + 7] << wh << endl;
		}
		cout << endl;
	}
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & pieces;
    }
};

inline bool operator==(const stdBoard & lhs, const stdBoard & rhs){ /* do actual comparison */
    return lhs.pieces[0] == rhs.pieces[0] &&
        lhs.pieces[1] == rhs.pieces[1] &&
        lhs.pieces[2] == rhs.pieces[2] &&
        lhs.pieces[3] == rhs.pieces[3];
}


namespace std
{
    template<> struct hash<stdBoard>
    {
      typedef stdBoard argument_type;
      typedef std::size_t result_type;
      result_type operator()(argument_type const& b) const noexcept
      {
        return boost::hash_range((uint32_t*)b.pieces, (uint32_t*)b.pieces+4);
      }
    };
}


#endif /* RANDOM_CHECKERS_INCLUDED */
