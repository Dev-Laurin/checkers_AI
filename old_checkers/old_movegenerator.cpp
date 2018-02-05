//Move Generator program

#include "old_movegenerator.h"

namespace altCheckers {

	//int maxMoveFactor = 0;

	void PrintBoard(char Board[32]) {
		for (int i = 0; i < 32; i += 8) {
			std::cout << " " << Board[i] << " " << Board[i + 1] << " " << Board[i + 2] << " " << Board[i + 3] << std::endl;
			std::cout << Board[i + 4] << " " << Board[i + 5] << " " << Board[i + 6] << " " << Board[i + 7] << std::endl;
		}
	}

	void PrintBoardLine(char Board[32]) {
		for (int i = 0; i < 32; ++i) {
			std::cout << Board[i];
		}
	}

	//flips the Board around
	void flipBoard(char BoardStr[]) {
		char tempboard[BOARDSIZE];
		for (unsigned int x = 0; x < BOARDSIZE; ++x) {
			switch (BoardStr[x]) {
			case REDPAWN:
				tempboard[BOARDSIZE - x - 1] = BLACKPAWN;
				break;
			case BLACKPAWN:
				tempboard[BOARDSIZE - x - 1] = REDPAWN;
				break;
			case REDKING:
				tempboard[BOARDSIZE - x - 1] = BLACKKING;
				break;
			case BLACKKING:
				tempboard[BOARDSIZE - x - 1] = REDKING;
				break;
			default:
				tempboard[BOARDSIZE - x - 1] = EMPTYCELL;
				break;
			}
		}
		std::memcpy(BoardStr, tempboard, 32);
	}

	void flipBoard(std::string & Board) {
		char temp[33];
		Board.copy(temp, BOARDSIZE);
		flipBoard(temp);
		temp[32] = 0;
		Board = temp;
	}

	void kingMaker(char Boardret[]) { //Kings pieces
		for (int x = 0; x < 4; ++x) {
			if (Boardret[x] == BLACKPAWN)
				Boardret[x] = BLACKKING;
		}
	}

	//Checks for subsequent jumps
	//x = position of jump piece
	bool jumpcheck(const char ValidMoves[], const char BoardStr[], int x, int pieceValue, char Boardret[MAXBOARDS][BOARDSIZE], int & numMoves) {
		bool bFound = true; //Note: Reversed
		for (int y = 0; y < pieceValue; ++y) {  //check left and right
			if (BoardStr[ValidMoves[x*VALIDMOVEWIDTH + 4 + y]] == EMPTYCELL) { //spot is free to jump up left is valid
				if (BoardStr[ValidMoves[x*VALIDMOVEWIDTH + y]] == REDPAWN || BoardStr[ValidMoves[x*VALIDMOVEWIDTH + y]] == REDKING) {
					char tempboard[BOARDSIZE];
					memcpy(tempboard, BoardStr, BOARDSIZE);
					tempboard[ValidMoves[x*VALIDMOVEWIDTH + 4 + y]] = tempboard[x]; //move the piece
					tempboard[ValidMoves[x*VALIDMOVEWIDTH + y]] = EMPTYCELL; //remove the enemy piece
					tempboard[x] = EMPTYCELL; //remove the old piece
					//recursive check for MORE jumps
					if (jumpcheck(ValidMoves, tempboard, ValidMoves[x*VALIDMOVEWIDTH + 4 + y], pieceValue, Boardret, numMoves)) {//if jumpcheck didn't find a new move, add to list
						memcpy(Boardret[numMoves], tempboard, BOARDSIZE);
						kingMaker(Boardret[numMoves]);
						++numMoves;  //increment moves done
					}
					bFound = false;  //We found a new board, signal to caller not to add.
				}
			}
		}
		return bFound;
	}

	//takes the board, returns the number of boards generated, having placed the boards into the passed array
	//Using an array for maximum speed.
	//Note:  MoveGenerator always assumes Black.  Invert board to find red.
	//Ruleset assumes that a jump MUST be taken if available.
	//Note:  If no moves are found, that means that this side loses!
	//BoardStr: 33rd spot is null, IE '0'
	int MoveGenerator(char BoardStr[], char Boardret[MAXBOARDS][BOARDSIZE], int side) {
	    bool isBlack=true;
	    if (side==1) {
            isBlack = false;
	    }
		if (!isBlack) {
			flipBoard(BoardStr);
		}


		//Upleft, Upright, Downleft, Downright, then jumps.
		static const char ValidMoves[] = { 0, 0, 4, 5, 0, 0, 0, 9, //Board position 0
											1, 1, 5, 6, 1, 1, 8,10, //1
											2, 2, 6, 7, 2, 2, 9,11, //2
											3, 3, 7, 3, 3, 3,10, 3, //3
											4, 0, 4, 8, 4, 4, 4,13, //4
											0, 1, 8, 9, 5, 5,12,14, //5
											1, 2, 9,10, 6, 6,13,15, //6
											2, 3,10,11, 7, 7,14, 7, //7
											4, 5,12,13, 8, 1, 8,17, //8
											5, 6,13,14, 0, 2,16,18, //9
											6, 7,14,15, 1, 3,17,19, //10
											7,11,15,11, 2,11,18,11, //11
											12, 8,12,16,12, 5,12,21, //12
											8, 9,16,17, 4, 6,20,22, //13
											9,10,17,18, 5, 7,21,23, //14
											10,11,18,19, 6,15,22,15, //15
											12,13,20,21,16, 9,16,25, //16
											13,14,21,22, 8,10,24,26, //17
											14,15,22,23, 9,11,25,27, //18
											15,19,23,19,10,19,26,19, //19
											20,16,20,24,20,13,20,29, //20
											16,17,24,25,12,14,28,30, //21
											17,18,25,26,13,15,29,31, //22
											18,19,26,27,14,23,30,23, //23
											20,21,28,29,24,17,24,24, //24
											21,22,29,30,16,18,25,25, //25
											22,23,30,31,17,19,26,26, //26
											23,27,31,27,18,27,27,27, //27
											28,24,28,28,28,21,28,28, //28
											24,25,29,29,20,22,29,29, //29
											25,26,30,30,21,23,30,30, //30
											26,27,31,31,22,31,31,31  //31
		};

		int numMoves = 0;
		//Check for jumps.
		for (int x = 0; x < 32; ++x) { //cycle through board for jumps.
			int pieceValue;
			if (BoardStr[x] == BLACKPAWN)
				pieceValue = 2;
			else if (BoardStr[x] == BLACKKING)
				pieceValue = 4;
			else
				pieceValue = 0;
			jumpcheck(ValidMoves, BoardStr, x, pieceValue, Boardret, numMoves);
		}

		if (numMoves > 0) { //At least one jump has been found, return.
			if (!isBlack) {
				flipBoard(BoardStr);
				for (int i = 0; i < numMoves; ++i)
					flipBoard(Boardret[i]);
			}
			return(numMoves);
		}
		//No Jumps available
		//Check to see if moves are available.
		for (int x = 0; x < 32; ++x) { //cycle through board
			int pieceValue;
			if (BoardStr[x] == BLACKPAWN)
				pieceValue = 2; //The first two moves are up
			else if (BoardStr[x] == BLACKKING)
				pieceValue = 4;  //Kings can move down as well
			else
				pieceValue = 0;  //Nothing there
			for (int y = 0; y < pieceValue; ++y) {
				if (BoardStr[ValidMoves[x*VALIDMOVEWIDTH + y]] == EMPTYCELL) {//Move check
					memcpy(Boardret[numMoves], BoardStr, BOARDSIZE);
					std::swap(Boardret[numMoves][x], Boardret[numMoves][ValidMoves[x*VALIDMOVEWIDTH + y]]);
					kingMaker(Boardret[numMoves]);
					++numMoves;
				}
			}
		}
		if (!isBlack) {
			flipBoard(BoardStr);
			for (int i = 0; i < numMoves; ++i)
				flipBoard(Boardret[i]);
		}
		//	if(numMoves > maxMoveFactor) {
		//		maxMoveFactor = numMoves;
		//		std::cout << "New Max Moves found: " << maxMoveFactor << std::endl;
		//	}
		return(numMoves);
	}
}
