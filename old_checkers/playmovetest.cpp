/*
NeuralNetTest.cpp
Jason Bright
Neural Net Test program
20160215

*/
/*
gen wrong moves/jumps
Bad iterator
Switch maximum

*/


#include "neuralnet.h"
#include "movegenerator.h"
#include "playmove.h"


using std::cout;
using std::endl;

#define MAXGAMEMOVES 200
#define WINVALUE 12
#define LOSEVALUE -12
//just needs to be big
#define WINNINGVALUE 1000000 
#define DEPTH 4

//For checking purposes.

/*
double MaxMove (NeuralNet & Player, char Board[32], int depth, NETPRECISION alpha, NETPRECISION beta) {
	char BoardReturns[MAXBOARDS][BOARDSIZE];
	int NumMoves = MoveGenerator(Board, BoardReturns); //Get the next set of moves
	if (NumMoves == 0) { //If no moves are found, this is a really lousy choice because I lose
//		PrintBoardLine(Board);
//		cout << " Layer " << depth << " Losing Move found!" << endl;
		return (depth - WINNINGVALUE); // The further out the loss is, the better.  Slightly. 
	}
	double BestValue = -std::numeric_limits<double>::max();
	if (depth > 0) {
		for (int i = 0; i < NumMoves; ++i) {
			BestValue = std::max(BestValue, MinMove(Player, BoardReturns[i], depth - 1));
			alpha = std::max(BestValue, alpha);
			if (beta <= alpha) {
				++TrimCount;
				return BestValue;
			}
		}
	}
	else {
		Player.setInputs(Board);
//		PrintBoardLine(Board);
//		cout << " MaxMove Impulse: " << Player.Impulse() << endl;
		++ImpulseCount;
		return Player.Impulse();
	}
//	PrintBoardLine(Board);
//	cout << " Layer " << depth << " MaxMove Value: " << BestValue << endl;
	return BestValue;
}

double MinMove (NeuralNet & Player, char Board[32], int depth, NETPRECISION alpha, NETPRECISION beta) {
	char BoardReturns[MAXBOARDS][BOARDSIZE];
	int NumMoves = MoveGenerator(Board, BoardReturns, false); //Get the next set of moves
	if (NumMoves == 0) { //No moves found means that this would be a really good move for me.
//		PrintBoardLine(Board);
//		cout << " Layer " << depth << " Winning move found!" << endl;
		return (WINNINGVALUE - depth); // The further out the win is, the worse.  Slightly. 
	}
	double BestValue = std::numeric_limits<double>::max();
	if (depth > 0) {
		for (int i = 0; i < NumMoves; ++i) {
			BestValue = std::min(BestValue, MaxMove(Player, BoardReturns[i], depth - 1));
			beta = std::min(BestValue, beta);
			if (beta <= alpha) {
				++TrimCount;
				return BestValue;
			}
		}
	}
	else {
		Player.setInputs(Board);
//		PrintBoardLine(Board);
//		cout << "  MinMove Impulse: " << Player.Impulse() << endl;
		++ImpulseCount;
		return Player.Impulse();
	}
//	PrintBoardLine(Board);
//	cout << " Layer " << depth << " MinMove Value: " << BestValue << endl;
	return BestValue;
}


void PlayMove(NeuralNet & Player, char Board[32]) {
	char BoardReturns[MAXBOARDS][BOARDSIZE];
	cout << Board << endl;
	int bestMove = 0;
	int NumMoves;
	NumMoves = MoveGenerator(Board, BoardReturns); //Get the next set of moves
	NETPRECISION bestMoveVal = -std::numeric_limits<NETPRECISION>::max();
	NETPRECISION curMoveVal;
	for(int i = 0; i < NumMoves; ++i) {  //Work through boards, decide best one.
		Player.setInputs(BoardReturns[i]);
		cout << "Preboard check: " << Player.Impulse() << endl;
		
		curMoveVal = MinMove(Player, BoardReturns[i], 7, bestMoveVal, std::numeric_limits<NETPRECISION>::max());
		PrintBoardLine(BoardReturns[i]);
		cout << " PlayMoveValue: " << curMoveVal << endl;
		if (curMoveVal > bestMoveVal) {
			bestMove = i;
			bestMoveVal = curMoveVal;
		}
	}
	memcpy(Board, BoardReturns[bestMove], BOARDSIZE);
	cout << "Final Move: ";
	PrintBoardLine(Board);
	cout << endl;
	PrintBoard(Board);
	}

*/
int main(int argc, char * argv[]) {
	int NumMoves;
	char BoardReturns[MAXBOARDS][BOARDSIZE];
	int curPlayer = 0;
	int round = 0;
	bool gamelost = false;
	NeuralNet Brain[2];
	Brain[0].Load(argv[1]); //black, moves first
	Brain[0].Print();
	int depth = atoi (argv[2]);
	cout << "Neural Depth " << depth << endl;
	PlayMove(Brain[0],argv[3], DEPTH);
	return 0;
}
