//playgame.cpp
//3-12-18 Revised
//Play a game of NN's

#include <iostream>
#include <vector>
using std::vector;
#include "../neuralNetwork.h"
#include "../playgame.h"

int main() {
  vector<int> nodes{32, 40, 10, 1};
  //NN player2(nodes, "Blondie1");
  PieceCount player1;
  stdBoard board;
  stdBoard b;

//  board.str(" rrrrr rrr r   rb b  bb bb  bbbb");
//  board.str("Brr r       rb    rrbb    b   b ");
//  board.str("Br    r b    b   Rb    r        ");
  //board.str("        B RB  R                 ");
  //b.str("        BBBB                    ");
  //b.str("          r        B    R Br  R ");
  stdBoard moveList[MAXMOVES];
  board.draw();
  int numMoves = board.genMoves(moveList, 1);
  //If the board isn't in the unordered map, add it.
  for(int i = 0; i < numMoves; ++i) {
      if (monteMem.count(moveList[i])==0) {
        monteMem[moveList[i]][0] = 0;
        monteMem[moveList[i]][1] = 0;
        monteMem[moveList[i]][2] = 0;
      }
  }
  for(int j = 0; j < 10; ++j) {
    std::sort(moveList, moveList+numMoves,
        [&](stdBoard a, stdBoard b) {
          return (monteMemVal(a) > monteMemVal(b));
        });
    for(int i = 0;i < numMoves; ++i) {
      cout << moveList[i].str() << " " << monteMemVal(moveList[i]) << endl;
    }
    cout << endl;
    for (int i = 0; i < numMoves; ++i) {
      monte(moveList[i], false);
    }
  }


/*
  for (int i = 2; i < 5; ++i) {
    player1.searchDepth = i;
    cout << "searching depth " << i << endl << std::flush;
    b = player1.getMove(board,false);

    cout << "Ply " << i*2 << endl;
    cout << "gameBoards: " << (innerNodes+leafNodes) << endl;
    cout << "boardEvals: " << boardEvals << endl;
    cout << "innerNodes: " << innerNodes << endl;
    cout << "leafNodes: " << leafNodes << endl;

    gameBoards = 0;
    boardEvals = 0;
    innerNodes = 0;
    leafNodes = 0;

  }

*/
/*
  int nMoves = b.genMoves(moveList,1);
  for(int i = 0; i < nMoves; ++i) {
    moveList[i].draw();
  }
  cout << "Moves: " << nMoves << endl;
  //b = player1.getMove(board);
*/

}
