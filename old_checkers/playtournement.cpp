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
#include "playgame.h"

using std::cout;
using std::endl;

#define MAXGAMEMOVES 150
#define NUMBRAINS 50
#define WINVALUE 12
#define LOSEVALUE -12
#define NUMGAMESPERPLAYER 10

/*
void PlayGame(NeuralNet & Player1, NeuralNet & Player2) {
	int NumMoves;
	char BoardReturns[MAXBOARDS][BOARDSIZE];
	NeuralNet * curPlayer = & Player1;
	NeuralNet * altPlayer = & Player2;
	int round = 0;
	bool gamelost = false;
	char board[33] = "rrrrrrrrrrrr________bbbbbbbbbbbb";
	while ((round < MAXGAMEMOVES) && !(gamelost)) {
		int bestMove = 0;
		NumMoves = MoveGenerator(board, BoardReturns); //Get the next set of moves
		if (NumMoves == 0) {
			gamelost = true; //No moves left, lost.
			break;
		}
		curPlayer->setInputs(BoardReturns[0]);
		NETPRECISION bestMoveVal = curPlayer->Impulse();
		NETPRECISION curMoveVal;
		for(int i = 1; i < NumMoves; ++i) {  //Work through boards, decide best one.
			curPlayer->setInputs(BoardReturns[i]);
			curMoveVal = curPlayer->Impulse();
			if (curMoveVal > bestMoveVal) {
				bestMove = i;
				bestMoveVal = curMoveVal;
			}
		}
		memcpy(board, BoardReturns[bestMove], BOARDSIZE);
		++round;  //Move to next round.
		flipBoard(board);
		std::swap(curPlayer,altPlayer);
	}
	if (gamelost) {
		curPlayer->Score += LOSEVALUE;
		altPlayer->Score += WINVALUE;
//		cout << "Game won" << endl;
	}
	else {
		int boardscore=0;
		for(int i = 0; i<BOARDSIZE; ++i) {
			if (board[i] == REDPAWN || board[i] == REDKING)
				--boardscore;
			else if (board[i] == BLACKPAWN || board[i] == BLACKKING)
				++boardscore;
		}
		curPlayer->Score += boardscore;
		altPlayer->Score -= boardscore;
//		cout << "Game Tied " << boardscore << " Points" << endl;
//			cout << "Winner's Score: " << curPlayer->Score << endl;
	}
}
*/

int main(int argc, char * argv[]) {
	std::random_device rd;  //Note:  Change this out and one can have full repeatability.
	std::mt19937_64 gen(rd()); //use a decent random number
	std::uniform_int_distribution<int> distro(0,NUMBRAINS-1); //flat 
	while (true) {
		NeuralNet Brain[NUMBRAINS];
		for (int i = 0; i < NUMBRAINS; ++i) {
			std::stringstream fileName;
			fileName << argv[1] << "/brain" << i << ".NN";
			Brain[i].Load(fileName.str());
		}
		for (int i = 0; i < NUMBRAINS; ++i) {
			for (int j = 0; j < NUMGAMESPERPLAYER; ++j) {
				int oppNum; //number of opponent to play
				while(true) { //Select an opponent other than self.
					oppNum = distro(rd);
					if(oppNum != i) {
						break;
					}
				}
				cout << "Brain " << i << " vs Brain " << oppNum << endl;
				PlayGame(Brain[i],Brain[oppNum],4);
				cout << "Brain" << i << " Score: " << Brain[i].Score
					<< " Brain" << oppNum << " Score: " << Brain[oppNum].Score << endl;	
			}
		}
		for (int i = 0; i < NUMBRAINS; ++i) {
			cout << "Brain " << i << ", gen " << Brain[i].Generation << ": "<< Brain[i].Score << endl;
		}
		//Brains are sorted.
		std::sort(Brain,Brain+NUMBRAINS,[](const NeuralNet & x, const NeuralNet & y) {
		return x.Score > y.Score;});
		cout << "Please wait, saving brain states" << endl;
		for(int i = 0;i < (NUMBRAINS/2);++i) {
			int halfpoint = (NUMBRAINS/2);
			Brain[i+halfpoint] = Brain[i];
			Brain[i+halfpoint].Evolve();
		}
		for(int i = 0;i < NUMBRAINS;++i) {
			std::stringstream fileName;
			fileName << argv[1] << "/brain" << i << ".NN";
			Brain[i].Save(fileName.str());
		}
		cout << "Finished saving brains" << endl;
	}
}
