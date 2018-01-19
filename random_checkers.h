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
	void stdBoard();  //4,095; 4,293,918,720
	void stdBoard(string board);
	void stdBoard(i32 b, i32 r, i32 bK, i32 rK);
	string strBoard();  //returns string representation of a board.
	//generateMoves - fills an array/vector with the valid moves possible on the board.
	//Returns false if no valid moves found.
	bool generateMoves(stdBoard boardList[]);  //boardlist is previously allocated vector/array for boards to be stored in.  Return is false if no moves
	strdBoard flipBoard();  //returns view of board from other side.
};

#endif /* RANDOM_CHECKERS_INCLUDED */ 