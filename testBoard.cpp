#include <iostream>

using std::cout; 
using std::endl; 
#include "random_checkers.h"

int main(){
cout << "testing functions" << endl;
stdBoard test00;
if (test00.str() != "rrrrrrrrrrrr        bbbbbbbbbbbb") {
	cout << "Default constructor returned unexpected value: " << test00.str() << endl;
} else {
	cout << "Default constructor passed, board is " << test00.str() << endl;
}
stdBoard test01 ("RrRrRrBbBbBb        BrBrBrRbRbRb");
if (test01.str() != "RrRrRrBbBbBb        BrBrBrRbRbRb") {
	cout << "Default string constructor returned unexpected value: " << test01.str() << endl;
} else {
	cout << "Default string constructor passed, board is " << test01.str() << endl;
}
stdBoard test02 ("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
if (test02.str() != "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB") {
	cout << "Default string constructor returned unexpected value: " << test02.str() << endl;
} else {
	cout << "Default string constructor passed, board is " << test02.str() << endl;
}
}

//Start testing move generator

stdBoard moveChoices[32]  //try a vector version later.
int numMoves;
numMoves = test00.genMoves(moveChoices);


