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

//For checking purposes.
int ImpulseCount = 0; //Number of impulses performed
int TrimCount = 0; //Number of trims done.


double MaxMove (NeuralNet & Player, char Board[32], int depth, NETPRECISION alpha, NETPRECISION beta) {
	char BoardReturns[MAXBOARDS][BOARDSIZE];
	int NumMoves = MoveGenerator(Board, BoardReturns); //Get the next set of moves
	if (NumMoves == 0) { //If no moves are found, this is a really lousy choice because I lose
//		PrintBoardLine(Board);
//		cout << " Layer " << depth << " Losing Move found!" << endl;
		return (-depth - WINNINGVALUE); // The further out the loss is, the better.  Slightly. 
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
		return (WINNINGVALUE + depth); // The further out the win is, the worse.  Slightly. 
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

void MinMoveWrap (NeuralNet & Player, char Board[32], int depth, NETPRECISION alpha, NETPRECISION beta, double & retValue) {
	retValue = MinMove(Player, Board, depth, alpha, beta);
}

bool PlayMove(NeuralNet & Player, std::string & Boardstr, int playDepth) {
	char BoardReturns[MAXBOARDS][BOARDSIZE];
	char Board[32];
	Boardstr.copy(Board,BOARDSIZE,0);
	int BoardOrder[MAXBOARDS]; //Order the boards
	for (int i = 0;i<MAXBOARDS;++i) {
		BoardOrder[i] = i;
	}
	NETPRECISION BoardScores[MAXBOARDS]; //KEEP the scores of the boards.
	int bestMove = 0;
	int NumMoves;
// Start the timer
	time_t time_start; //We have limited amount of time.
	time_t time_check;
	time(&time_start);
		
//Find the initial moves
	NumMoves = MoveGenerator(Board, BoardReturns); //Get the next set of moves
	if (NumMoves == 0) {
//		cout << "no moves found!" << endl;
//		cout << "Impulses: " << ImpulseCount << endl;
//		cout << "Trims " << TrimCount << endl;		
		return true;
	} else if (NumMoves == 1) { //Only 1 move; quick.
		for(int x= 0; x < BOARDSIZE; ++x) {
			Boardstr[x] = BoardReturns[0][x];
		}
		return false;
	} else if (NumMoves > 7) {  //Number of moves is too great, look 2 less than normal
//		playDepth = playDepth - 2;
	} else if (NumMoves < 5) {
//		playDepth = playDepth + 2; //Not many moves availabe, look deeper.
	}

	while (true) {	
	// Test the moves 	
		NETPRECISION bestMoveVal = -std::numeric_limits<NETPRECISION>::max();
		NETPRECISION curMoveVal;
		std::vector<std::thread> threads;
		double moveReturn[50];
		for(int i = 0; i < NumMoves; ++i) {  //Work through boards, generating the threads.
	//		Player.setInputs(BoardReturns[i]);
	//		cout << "Preboard check: " << Player.Impulse() << endl;
			threads.push_back(std::thread(MinMoveWrap, std::ref(Player), BoardReturns[BoardOrder[i]], playDepth, bestMoveVal, std::numeric_limits<NETPRECISION>::max(),std::ref(BoardScores[BoardOrder[i]])));
		}
		for(int i = 0; i < NumMoves; ++i) {  //Work through boards, resolving the threads
			time(&time_check);
//			cout << "Time check: " << difftime(time_check,time_start) << endl;
			if(difftime(time_check,time_start) >= MAXTIME) {
				for(int j = i; j < NumMoves; ++j) {
					threads[j].detach(); //Cleanup the threads, just let them run out.
				}
				cout << "Trimming!" << endl;
				return false; //Times's up, head back!
			}
			threads[i].join();
	//		cout << "moveReturn: " << moveReturn[i] << " bestMoveVal: " << bestMoveVal << endl;
			if (BoardScores[BoardOrder[i]] > bestMoveVal) {
	//			cout << "better move found: ";
	//			PrintBoardLine(BoardReturns[i]);
	//			cout << " Value: " << moveReturn[i] << endl;
				int temp = BoardOrder[i];
				BoardOrder[i] = BoardOrder[0];
				BoardOrder[0] = temp;
				bestMove = BoardOrder[i];
				bestMoveVal = BoardScores[BoardOrder[i]];
				for(int x= 0; x < BOARDSIZE; ++x) {
					Boardstr[x] = BoardReturns[bestMove][x];
				}
			}
		}
		time(&time_check);
		if(difftime(time_check,time_start) >= TIMELIMIT) {
			return false;
		}	else {
			playDepth = playDepth + 2;
//			cout << "Depth +2!" << endl;
		}
	}
		

	

//	cout << "Boardstr: " << Boardstr << endl;
//	cout << "Final Move: ";
//	PrintBoardLine(Board);
//	cout << endl;
//	PrintBoard(Board);
	return false;
}

/*
int main(int argc, char * argv[]) {
	int NumMoves;
	char BoardReturns[MAXBOARDS][BOARDSIZE];
	int curPlayer = 0;
	int round = 0;
	bool gamelost = false;
	NeuralNet Brain[2];
	Brain[0].Load(argv[1]); //black, moves first
	Brain[0].Print();
	PlayMove(Brain[0],argv[2]);
	cout << "Impulses: " << ImpulseCount << endl;
	cout << "Trims " << TrimCount << endl;
	return 0;
}
*/