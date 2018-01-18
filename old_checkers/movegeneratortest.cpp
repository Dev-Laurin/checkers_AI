#include "movegenerator.h"


int main(int argc, char * argv[]) {
	char BoardIn[32];
	int NumMoves;
	char BoardReturns[MAXBOARDS][BOARDSIZE];
	std::memset(BoardReturns, ' ', BOARDSIZE*MAXBOARDS);
	
	for(int x = 0; x < 32;++x)
		BoardIn[x] = argv[1][x];
	PrintBoardLine(BoardIn);
	std::cout << std::endl;
	NumMoves = MoveGenerator(BoardIn, BoardReturns, false);
	std::cout << "Number of moves returned: " << NumMoves << std::endl;
	PrintBoard(BoardReturns[0]);
	for(int x = 0; x < NumMoves; ++x) {
		PrintBoardLine(BoardReturns[x]);
		std::cout << std::endl;
	}
}