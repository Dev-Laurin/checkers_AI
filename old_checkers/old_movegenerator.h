#ifndef MOVEGENERATOR
#define MOVEGENERATOR

#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>

namespace altCheckers
{
	const unsigned int MAXBOARDS = 32;
	const unsigned int BOARDSIZE = 32;
	const unsigned int VALIDMOVEWIDTH = 8;
	const unsigned char REDPAWN = 'r';
	const unsigned char BLACKPAWN = 'b';
	const unsigned char REDKING = 'R';
	const unsigned char BLACKKING = 'B';
	const unsigned char EMPTYCELL = ' ';

	void PrintBoard(char Board[32]);
	void PrintBoardLine(char Board[32]);

	void kingMaker(char Boardret[]);

	bool jumpcheck(const char ValidMoves[], const char BoardStr[], int x, int pieceValue, char Boardret[MAXBOARDS][BOARDSIZE], int & numMoves);

	int MoveGenerator(char BoardStr[], char Boardret[MAXBOARDS][BOARDSIZE], int side = 0);

	void flipBoard(char BoardStr[]);
	void flipBoard(std::string & Board);
}
#endif
