#include <iostream>

using std::cout;
using std::endl;
#include "board.h"
#include "old_movegenerator.h"
#include "testBoard.h"
#include <random>
#include <chrono>

int maxBranch = 0;

//print boards horizontally
void printBoardArray(stdBoard boardList[], int numBoards) {
	int BoardsPerRow = 7;
	string boards[numBoards];
	for (int i = 0; i < numBoards; ++i) {
		boards[i] = boardList[i].str();
	}
	for (int i = 0; i < numBoards; i += BoardsPerRow) {
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

//Not going to be very efficient, but for testing boards is fine.
bool testBoard(stdBoard &board, int side) {
    //declare the data structures for the different generators to move things into
    stdBoard moves1[32];
    int numMoves1 = 0;
    char moves2[32][32];
    string m2string[32];
    int numMoves2 = 0;
    string bstr = board.str();
    char bd[32];
    for (int i = 0;i<32;++i) {
        bd[i] = bstr[i];
    }

    numMoves1 = board.genMoves(moves1,side);
    numMoves2 = altCheckers::MoveGenerator(bd, moves2, side);

    //Check that move counts match.  If not, failure.
    if (numMoves1 != numMoves2) {
        cout << "Test failed; Number of moves do not match" << endl;
        cout << "Board: " << numMoves1 << " OldGen: " << numMoves2 << endl;
        return false;
    }
    //Convert char arrays to strings
    bool matchTest;
    bool matched[32] = {false};
    for(int i = 0; i < numMoves1; ++i) {
        m2string[i] = string(moves2[i],32);
        matchTest = false;
        //find in stdBoard
        for(int j = 0; j < numMoves1; ++j) {
            if(m2string[i] == moves1[j].str()) {
                if (matched[j] == true) {
                    matchTest = false;
                    cout << "Duplicate Board found" << endl;
                    return false;
                } else {
                    matched[j] = true;
                    matchTest = true;
                    break;
                }
            }
        }
        if (matchTest==false) {
            cout << "No Match Found" << endl;
            cout << m2string[i] << endl;
            return false;
        }
    }
	return true;
}

//overloaded function to remove having to keep typing an if statement
bool testBoard(stdBoard &tBoard, int &numTests, int &numPassed) {
        ++numTests;
        bool testResult = testBoard(tBoard, 0);
        if (testResult) {
            //cout << "Board passed test: " << tBoard.str() << endl;
        } else {
            cout << "Board failed test: " << tBoard.str() << endl;
            return false;
        }
        testResult = testBoard(tBoard, 1);
        if (testResult) {
            //cout << "Board passed test: " << tBoard.str() << endl;
            ++numPassed;
        } else {
            cout << "Board failed test: " << tBoard.str() << endl;
            return false;
        }
        return testResult;
}

int main() {

	 int testsCorrect = 0;
	 int numOfTests = 0;

	cout << "Constructor Testing functions" << endl;

	//Test 0
	stdBoard test00;
	numOfTests++;
	if (test00.str() != "rrrrrrrrrrrr        bbbbbbbbbbbb") {
		cout << "Default constructor returned unexpected value: " << test00.str() << endl;
	}
	else {
		//cout << "Default constructor passed, board is " << test00.str() << endl;
		testsCorrect++;
	}

	//Test 1
	numOfTests++;
	stdBoard test01("RrRrRrBbBbBb        BrBrBrRbRbRb");
	if (test01.str() != "RrRrRrBbBbBb        BrBrBrRbRbRb") {
		cout << "Default string constructor returned unexpected value: " << test01.str() << endl;
	}
	else {
		//cout << "Default string constructor passed, board is " << test01.str() << endl;
		testsCorrect++;
	}

	//Test 2
	numOfTests++;
	stdBoard test02("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
	if (test02.str() != "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB") {
		cout << "Default string constructor returned unexpected value: " << test02.str() << endl;
	}
	else {
		//cout << "Default string constructor passed, board is " << test02.str() << endl;
		testsCorrect++;
	}

	cout << "	Tests Correct: " << testsCorrect << "/";
	cout << numOfTests << endl;


/*
	//Test Draw function.
	cout << "Printing test board, should be starting board." << endl;
	test00.draw();




	stdBoard test(0,0,0,0);
	stdBoard moveChoices[32];  //try a vector version later.
	int numMoves;
*/
	cout << "Testing Move Generator" << endl;
	//Testing regular moves.
	numOfTests = 0;
	testsCorrect = 0;
	//Test specific boards
	{
	  stdBoard b(" B       R       BR  r          ");
	  testBoard(b, numOfTests, testsCorrect);
	  b.updateBoard("          r        B    R Br  R ");
	  testBoard(b, numOfTests, testsCorrect);
    b.str(" R       B       RB  b          ");
	  testBoard(b, numOfTests, testsCorrect);
	  b.str("          b        R    B Rb  B ");
    //Move Gen test - error found during gameplay
    b.str("r B rr r      rrr  r bbbb b bbb ") ;
    testBoard(b, numOfTests, testsCorrect);
  }

  //Test individual moves.
  {
      stdBoard tBoard;
      stdBoard tTracker[4] = {stdBoard(0,0,0,0)};
      for(int j = 0; j < 4;++j) {
          tBoard.pieces[0] = 0;
          tBoard.pieces[1] = 0;
          tBoard.pieces[2] = 0;
          tBoard.pieces[3] = 0;
          for(int i = 0; i < 8;++i) {
              tBoard.pieces[j] = 0xF << 4*i;
              tBoard.pieces[j & 1] = tBoard.pieces[j];
              testBoard(tBoard, numOfTests, testsCorrect);
              tTracker[j].pieces[j] = tTracker[j].pieces[j] | tBoard.pieces[j];
          }
      }
      cout << "Showing individual moves tested piece locations" << endl;
      printBoardArray(tTracker,4);
  }




  //Test jumps.
  {
      stdBoard tBoard;
      stdBoard tTracker[4] = {stdBoard(0,0,0,0)};
      for(int j = 0; j < 4;++j) {
          tBoard.pieces[0] = 0;
          tBoard.pieces[1] = 0;
          tBoard.pieces[2] = 0;
          tBoard.pieces[3] = 0;
          for(int i = 0; i < 8;++i) {

              //Set up row.
              tBoard.pieces[j] = 0xF << 4*i;
              tBoard.pieces[j & 1] = tBoard.pieces[j];
              tTracker[j].pieces[j] = tTracker[j].pieces[j] | tBoard.pieces[j];

              //Set up opposite rows.
              tBoard.pieces[(j^1) & 1] = tBoard.pieces[j] >> 4 | tBoard.pieces[j] << 4;

              //Test
              testBoard(tBoard, numOfTests, testsCorrect);
          }
      }
      cout << "Showing individual moves jump piece locations" << endl;
      printBoardArray(tTracker,4);
  }

  //Test Blocking.
  {
      stdBoard tBoard;
      stdBoard tTracker[4] = {stdBoard(0,0,0,0),stdBoard(0,0,0,0),stdBoard(0,0,0,0),stdBoard(0,0,0,0)};
      for(int j = 0; j < 4;++j) {
          tBoard.pieces[0] = 0;
          tBoard.pieces[1] = 0;
          tBoard.pieces[2] = 0;
          tBoard.pieces[3] = 0;
          for(int i = 0; i < 8;++i) {

              //Set up row.
              tBoard.pieces[j] = 0xFF << 4*i;
              tBoard.pieces[j & 1] = tBoard.pieces[j];
              tTracker[j].pieces[j] = tTracker[j].pieces[j] | tBoard.pieces[j];

              //Set up opposite rows.
              tBoard.pieces[(j^1) & 1] = tBoard.pieces[j] >> 8 | tBoard.pieces[j] << 8;

              //Test
              testBoard(tBoard, numOfTests, testsCorrect);
          }
      }
      cout << "Showing individual moves jump piece locations" << endl;
      printBoardArray(tTracker,4);
  }

  //Test a specific board.
/*
  {
    stdBoard tBoard("                rrrrBBBBrrrr    ");
    tBoard.draw();
    cout << endl;
    stdBoard moveList[32];
    int mvCount = tBoard.genMoves(moveList,0);
    printBoardArray(moveList,mvCount);
    testBoard(tBoard, numOfTests, testsCorrect);
  }
*/
/*
	//Test Move Generator
	stdBoard bb;
	cout << "Beginning Board: " << bb.str() << endl;
	cout << "Flipped Board: " << bb.flipBoard() << endl;
	stdBoard possbileMoves[30];
	int moves = bb.genMoves(possbileMoves, 1); //red
	for(int i=0; i<moves; ++i){
		if(possbileMoves[i].flipBoard().at(20)==' '){
			cout << "Possible moves at location 20 checker" << endl;
				cout << possbileMoves[i].flipBoard() << endl;
	cout << "New index: ";
	cout << findCheckerMove(possbileMoves[i].flipBoard(), bb.flipBoard());
	cout << endl;
		}

	}

	bb.updateBoard(possbileMoves[3].str());
	cout << "New board: " << bb.str() << endl;
	cout << "Flipped: " << bb.flipBoard() << endl;


	stdBoard pMoves[30];
	moves = bb.genMoves(pMoves, 1);
	for(int i=0; i<moves; ++i){
				if(pMoves[i].flipBoard().at(16)==' '){
			cout << "Possible moves at location 16 checker" << endl;
				cout << pMoves[i].flipBoard() << endl;
	cout << "New index: ";
	cout << findCheckerMove(pMoves[i].flipBoard(), bb.flipBoard());
	cout << endl;
		}
	} */

    // Tests finished, print results
    cout << "Move Generator tests done" << endl;
    cout << testsCorrect << "/" << numOfTests << " Correct" <<endl;
    if (testsCorrect == numOfTests) {
        cout << "All tests passed" << endl;
    } else {
        cout << "Not all tests passed!" << endl;
    }
    numOfTests = 0;
    testsCorrect = 0;

	//Jump Test 0
	numOfTests++;
	cout << "	If there are jumps available, return jumps moves only (priority).";
	cout << endl;
	stdBoard jumpPriority("rrrrrrrrrr r  r  b  bb bbbbbbbbB");
	stdBoard possibleMoves[30];
	int moves = jumpPriority.genMoves(possibleMoves, 0);
	if(possibleMoves[0].str()=="rrrrrrrrrrbr        bb bbbbbbbbB"){
		testsCorrect++;
		cout << "		Successful" << endl;
	}
	else{
		cout << "	Error Test 0. More than 1 move when there is 1 jump." << endl;
	}

	//Jump Test 1
	numOfTests++;
	cout << "	Check if there is a 2-jump." << endl;
	jumpPriority.updateBoard("Rrrrr    r       r    b bbbbbbbb");
	stdBoard possibleMoves2[30];
	moves = jumpPriority.genMoves(possibleMoves2, 0);
	for(int i=0; i<moves; ++i){
		if(possibleMoves2[i].str()=="Rrrrr b                 bbbbbbbb"){
			testsCorrect++;
			cout << "		Successful" << endl;
			break;
		}
	}

	//Jump Test 2
	numOfTests++;
	cout << "	Check if there is a 3-jump." << endl;
	jumpPriority.updateBoard("     r        r       r  b      ");
	moves = jumpPriority.genMoves(possibleMoves, 0);
	if(possibleMoves[0].str()=="B                               "){
		testsCorrect++;
		cout << "		Successful" << endl;
	}
	else{
		cout << "Failed 3-jump Test 2. " << endl;
		cout << possibleMoves[0].str() << " vs ";
		cout <<  "B                               " << endl;
	}

	//Jump Test 3
	numOfTests++;
	cout << "	Check jumping off right edge." << endl;
	jumpPriority.updateBoard("           r   b                ");
	moves = jumpPriority.genMoves(possibleMoves, 0);
	if(moves==1 and possibleMoves[0].str()=="          br                    "){
		testsCorrect++;
		cout << "		Successful" << endl;
	}
	else{
		cout << "Failed Jump Test 3. Jumping off right edge." << endl;
	}

	//Jump Test 4
	numOfTests++;
	cout << "	Check jumping off top edge." << endl;
	jumpPriority.updateBoard("   r   b                        ");
	moves = jumpPriority.genMoves(possibleMoves, 0);
	if(moves==1 and possibleMoves[0].str()=="  Br                            "){
		testsCorrect++;
		cout << "		Successful" << endl;
	}
	else{
		cout << "Failed Jump Test 4. Jumping off top edge." << endl;
		cout << possibleMoves[0].str() << endl;
		cout << "Moves: " << moves << endl;
	}



	//Jump Test 5
	numOfTests++;
	cout << "	Check jumping up center." << endl;
	jumpPriority.updateBoard("           r       b            ");
	moves = jumpPriority.genMoves(possibleMoves, 0);
	if(moves==1 and possibleMoves[0].str()=="           r   b                "){
		testsCorrect++;
		cout << "		Successful" << endl;
	}
	else{
		cout << "Failed Jump Test 5" << endl;
		cout << possibleMoves[0].str() << endl;
		cout << "Moves: " << moves << endl;
	}




	cout << "	Tests Correct: " << testsCorrect << "/" << numOfTests;
	cout << endl;

    cout << "Benchmarking performance" << endl;
    {
        int maxBranch = 0;
//        std::mt19936 gen(time(0));
//        std::uniform_int_distribution<int> dis(0,3);
        stdBoard iBoard;
        //Board Returns.
        stdBoard Br[10][32];
        int i[10] = {0};
        int numMoves[10];
        int boardsGenerated[10] = {0};
        auto tStart = std::chrono::high_resolution_clock::now();
        numMoves[0] = iBoard.genMoves(Br[0],0);
        boardsGenerated[0] += numMoves[0];
        maxBranch = max(maxBranch,numMoves[0]);
        for(int j = 0; j < 100; ++j) {
          numMoves[0] = iBoard.genMoves(Br[0],0);
          boardsGenerated[0] += numMoves[0];
          maxBranch = max(maxBranch,numMoves[0]);
          for(i[0] = 0;i[0] < numMoves[0];++i[0]) {
              numMoves[1] = Br[0][i[0]].genMoves(Br[1],1);
              boardsGenerated[1] += numMoves[1];
              maxBranch = max(maxBranch,numMoves[1]);
              for(i[1]=0;i[1] < numMoves[1];++i[1]) {
                  numMoves[2] = Br[1][i[1]].genMoves(Br[2],0);
                  boardsGenerated[2] += numMoves[2];
                  maxBranch = max(maxBranch,numMoves[2]);
                  for(i[2]=0;i[2] < numMoves[2];++i[2]) {
                      numMoves[3] = Br[2][i[2]].genMoves(Br[2],1);
                      boardsGenerated[3] += numMoves[3];
                      maxBranch = max(maxBranch,numMoves[3]);
                      for(i[3]=0;i[3] < numMoves[3];++i[3]) {
                          numMoves[4] = Br[3][i[3]].genMoves(Br[3],0);
                          boardsGenerated[4] += numMoves[4];
                          maxBranch = max(maxBranch,numMoves[4]);
                          for(i[4]=0;i[4] < numMoves[4];++i[4]) {
                              numMoves[5] = Br[4][i[4]].genMoves(Br[4],1);
                              boardsGenerated[5] += numMoves[5];
                              maxBranch = max(maxBranch,numMoves[5]);
                              for(i[5]=0;i[5] < numMoves[5];++i[5]) {
                                  numMoves[6] = Br[5][i[5]].genMoves(Br[5],0);
                                  boardsGenerated[6] += numMoves[6];
                                  maxBranch = max(maxBranch,numMoves[6]);
                                  for(i[6]=0;i[6] < numMoves[6];++i[6]) {
                                      numMoves[7] = Br[6][i[6]].genMoves(Br[6],1);
                                      boardsGenerated[7] += numMoves[7];
                                      maxBranch = max(maxBranch,numMoves[7]);
                                      for(i[7]=0;i[7] < numMoves[7];++i[7]) {
                                          numMoves[8] = Br[7][i[7]].genMoves(Br[7],0);
                                          boardsGenerated[8] += numMoves[8];
                                          maxBranch = max(maxBranch,numMoves[8]);
                                          for(i[8]=0;i[8] < numMoves[8];++i[8]) {
                                              numMoves[9] = Br[8][i[8]].genMoves(Br[8],0);
                                              boardsGenerated[9] += numMoves[9];
                                              maxBranch = max(maxBranch,numMoves[9]);
                                          }
                                      }
                                  }
                              }
                          }
                      }
                  }
              }
          }
        }
        auto tStop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = tStop - tStart;

        cout << "boards generated: ";
        int totalBoards = 0;
        for(int j = 0; j < 10; ++j) {
                cout << boardsGenerated[j] << " ";
                totalBoards += boardsGenerated[j];
        }
        cout << endl;
        cout << totalBoards << " boards in " << elapsed.count() << " seconds, ";
        cout << "boards per second: " << int(totalBoards / elapsed.count()) << endl;
        cout << "Max branch factor: " << maxBranch << endl;
    }

}
