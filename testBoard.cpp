#include <iostream>

using std::cout; 
using std::endl; 
#include "board.h"

//print boards horizontally
void printBoardArray(stdBoard boardList[], int numBoards) {
	int BoardsPerRow = 7;
	string boards[numBoards];
	for (int i = 0; i < numBoards; ++i) {
		boards[i] = boardList[i].str();
	}
	for (int i = 0; i < numBoards; i += BoardsPerRow) {
		int boardsLeft = numBoards - i;
		cout << endl;
		for (int j = 0; j < 32; j += 8) {
			for (int k = 0; (i + k < numBoards) && (k < BoardsPerRow); ++k) {
				cout << "  ." << boards[i+k][j] << "." << boards[i+k][j + 1] << "." << boards[i+k][j + 2] << "." << boards[i+k][j + 3];
			}
			cout << endl;
			for (int k = 0; (i + k < numBoards) && (k < BoardsPerRow); ++k) {
				cout << "  " << boards[i+k][j + 4] << "." << boards[i+k][j + 5] << "." << boards[i+k][j + 6] << "." << boards[i+k][j + 7] << ".";
			}
			cout << endl;
		}
		cout << endl << endl;
	}
}

int main() {
	cout << "testing functions" << endl;
	stdBoard test00;
	if (test00.str() != "rrrrrrrrrrrr        bbbbbbbbbbbb") {
		cout << "Default constructor returned unexpected value: " << test00.str() << endl;
	}
	else {
		cout << "Default constructor passed, board is " << test00.str() << endl;
	}
	stdBoard test01("RrRrRrBbBbBb        BrBrBrRbRbRb");
	if (test01.str() != "RrRrRrBbBbBb        BrBrBrRbRbRb") {
		cout << "Default string constructor returned unexpected value: " << test01.str() << endl;
	}
	else {
		cout << "Default string constructor passed, board is " << test01.str() << endl;
	}
	stdBoard test02("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
	if (test02.str() != "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB") {
		cout << "Default string constructor returned unexpected value: " << test02.str() << endl;
	}
	else {
		cout << "Default string constructor passed, board is " << test02.str() << endl;
	}


	//Test Draw function.
	cout << "Printing test board, should be starting board." << endl;
	test00.draw();
	

	//Start testing move generator
	stdBoard test(0,0,0,0);
	stdBoard moveChoices[32];  //try a vector version later.
	int numMoves;

	cout << "Testing Move Generator" << endl;
	test.black = 0xF0000000; //Bottom row all filled.
	numMoves = test.genMoves(moveChoices);
	if (numMoves != 7) {
		cout << "Unexpected number of moves found!" << endl;
	}
	test.draw();
	numMoves = test.genMoves(moveChoices);
	printBoardArray(moveChoices, numMoves);

	test.black = 0xFF000000; //Bottom 2 rows all filled..
	test.draw();
	numMoves = test.genMoves(moveChoices);
	printBoardArray(moveChoices, numMoves);

	test.black = 0xFF000000; //Bottom 2 rows all filled.
	test.blackK = 0xFF000000; //Bottom 2 rows all kings.
	test.draw();
	numMoves = test.genMoves(moveChoices);
	printBoardArray(moveChoices, numMoves);

	test.black = 0x000000FF; //top all filled.
	test.blackK = 0x000000FF; //kings
	test.draw();
	numMoves = test.genMoves(moveChoices);
	printBoardArray(moveChoices, numMoves);

	cout << endl << "red moves" << endl;

	test.black = 0x00000300;
	test.blackK = 0;
	test.red = 0xF000000F;
	test.redK = 0xF0000000;
	test.draw();
	cout << endl;
	numMoves = test.genMoves(moveChoices, 1);
	printBoardArray(moveChoices, numMoves);

	test.black = 0xF0000300;
	test.blackK = 0;
	test.red = 0x0F0000F0;
	test.redK = 0x0F000030;
	test.draw();
	cout << endl;
	numMoves = test.genMoves(moveChoices,1);
	printBoardArray(moveChoices, numMoves);

	numMoves = test.genJumps(moveChoices,0);
	cout << "numMoves = " << numMoves << endl;
	printBoardArray(moveChoices, numMoves);


}