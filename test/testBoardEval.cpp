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

  board.str(" rrrrr rrr r   rb b  bb bb  bbbb");
  board.str("Brr r       rb    rrbb    b   b ");
  board.str("Br    r b    b   Rb    r        ");
  //board.str("        B RB  R                 ");
  //b.str("        BBBB                    ");
  //b.str("          r        B    R Br  R ");
  stdBoard moveList[MAXMOVES];
  board.draw();

  for (int i = 2; i < 5; ++i) {
    player1.searchDepth = i;
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


/*
  int nMoves = b.genMoves(moveList,1);
  for(int i = 0; i < nMoves; ++i) {
    moveList[i].draw();
  }
  cout << "Moves: " << nMoves << endl;
  //b = player1.getMove(board);
*/

}
