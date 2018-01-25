#include <iostream>

using std::cout; 
using std::endl; 
#include "board.h"

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
	cout << moveChoices[0].str();
	numMoves = test.genMoves(moveChoices);
	for (int i = 0; i < numMoves; ++i) {
		cout << endl;
		moveChoices[i].draw();
	}

	test.black = 0xFF000000; //Bottom 2 rows all filled.
	numMoves = test.genMoves(moveChoices);
	if (numMoves != 7) {
		cout << "Unexpected number of moves found!" << endl;
	}
	test.draw();
	cout << moveChoices[0].str();
	numMoves = test.genMoves(moveChoices);
	for (int i = 0; i < numMoves; ++i) {
		cout << endl;
		moveChoices[i].draw();
	}
	test.black = 0xFF000000; //Bottom 2 rows all filled.
	test.blackK = 0xFF000000; //Bottom 2 rows all kings.
	numMoves = test.genMoves(moveChoices);
	if (numMoves != 7) {
		cout << "Unexpected number of moves found!" << endl;
	}
	test.draw();
	cout << moveChoices[0].str();
	numMoves = test.genMoves(moveChoices);
	for (int i = 0; i < numMoves; ++i) {
		cout << endl;
		moveChoices[i].draw();
	}


}