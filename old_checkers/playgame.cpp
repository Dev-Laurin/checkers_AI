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

#include <ctime>
#include "neuralnet.h"
#include "movegenerator.h"
#include "playmove.h"

using std::cout;
using std::endl;

#define MAXGAMEMOVES 100
#define WINVALUE 12
#define LOSEVALUE -12
#define DEPTH 4


void PlayGame(NeuralNet & Player1, NeuralNet & Player2, int depth) {
	int NumMoves;
	char BoardReturns[MAXBOARDS][BOARDSIZE];
	NeuralNet * curPlayer = & Player1;
	NeuralNet * altPlayer = & Player2;
	int round = 0;
	bool gamelost = false;
	std::string board = "rrrrrrrrrrrr________bbbbbbbbbbbb";
//	cout << board << endl;
	while ((round < MAXGAMEMOVES) && !(gamelost)) {
		int bestMove = 0;
//		NumMoves = MoveGenerator(board, BoardReturns); //Get the next set of moves
		if (PlayMove(* curPlayer, board, depth)) {
			gamelost = true;
			break; //PlayMove returns true if the game is lost.
		}
		++round;  //Move to next round.
		if (curPlayer == & Player1) {
//			cout << board << endl;
			flipBoard(board);
		}
		else {
			flipBoard(board);
//			cout << board << endl;
		}
		std::swap(curPlayer,altPlayer);
	}
	if (gamelost) {
		curPlayer->Score += LOSEVALUE;
		altPlayer->Score += WINVALUE;
		if (curPlayer==&Player1) {
			cout << "Player 1 lost!" << endl;
		} else {
			cout << "Player 2 lost!" << endl;
		}
	}	else {
		int boardscore=0;
		for(int i = 0; i<BOARDSIZE; ++i) {
			if (board[i] == REDPAWN || board[i] == REDKING)
				--boardscore;
			else if (board[i] == BLACKPAWN || board[i] == BLACKKING)
				++boardscore;
		}
			curPlayer->Score += boardscore;
			altPlayer->Score -= boardscore;
			cout << "Stalemate, Differential: " << boardscore << endl;
	}
}
// brain1 brain2 depth
/*
int main(int argc, char * argv[]) {
	int NumMoves;
	char BoardReturns[MAXBOARDS][BOARDSIZE];
	int curPlayer = 0;
	int round = 0;
	bool gamelost = false;
	NeuralNet Brain[2];
	char board[33] = "rrrrrrrrrrrr________bbbbbbbbbbbb";
	Brain[0].Load(argv[1]); //black, moves first
	Brain[1].Load(argv[2]);
	int depth = atoi(argv[3]);
	//timing code
	time_t time_start1;
	time_t time_start2;
	time_t time_end1;
	time_t time_end2;
	time(&time_start1);
	//end timing code
	PlayGame(Brain[0],Brain[1], depth);
	time(&time_end1);
	std::cout << "It took " << difftime(time_end1,time_start1) << " seconds for game 1" << endl;

	if(Brain[0].Score == 12) 
		cout << "Player 1 Won game 1!" << endl;
	else if (Brain[1].Score == 12)
		cout << "Player 2 Won game 1!" << endl;
	else
		cout << "Game 1 was a draw!" <<endl;
	Brain[0].Score = 0;
	Brain[1].Score = 0;
	time(&time_start2); //timing for game 2
	PlayGame(Brain[1],Brain[0], depth);
	//timing for game 2
	time(&time_end2);
	std::cout << "It took " << difftime(time_end2,time_start2) << " seconds for game 2" << endl;
	//end timing
	if(Brain[0].Score == 12) 
		cout << "Player 1 Won game 2!" << endl;
	else if (Brain[1].Score == 12)
		cout << "Player 2 Won game 2!" << endl;
	else
		cout << "Game 2 was a draw!" <<endl;
	return 0;
}
*/