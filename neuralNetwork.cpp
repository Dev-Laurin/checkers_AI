#include "neuralNetwork.h"

stdBoard AIPlayer::getMove(stdBoard & board, bool side) {
      stdBoard possibleBoards[32];
      if(side) {
        board = board.flip();
      }
      int moves = board.genMoves(possibleBoards,0);
      int selectMove = 0;
      sNN moveVal = beta(possibleBoards[0],7);
      sNN tempMove;
      for (int i = 1;i<moves;++i) {
                tempMove = beta(possibleBoards[i],7);
                //cout << "moveVal: " << moveVal << " tempMove: " << tempMove << endl;
                //cout << "Highest: " << HIGHEST << " Lowest: " << LOWEST << endl;
                if (tempMove > moveVal) {
                    //better move found
                    moveVal = tempMove;
                    selectMove = i;
                }
      }
      if (side) {
          possibleBoards[selectMove].flip();
      }
      return possibleBoards[selectMove];
}

/*
  //Alpha is the MAX value selector.
  //Always assumed to be black.
  //Flip the board to computer run red.
  //Because a neural network may be good at selecting maximums
  //but not minimums.

  //Beta is the MINIMUM value selector.
  //Always assumed to be red.
*/
sNN AIPlayer::alpha(stdBoard & board, int depth) {
  stdBoard moveList[MAXMOVES];
  int moveCount;
  moveCount = board.genMoves(moveList,0);
  if (moveCount) {
      sNN rVal=LOWEST;
      if (depth>1) {
        for (int i = 0;i<moveCount;++i) {
          rVal = max(rVal,beta(moveList[i],depth-1));
        }
      } else {
        for (int i = 0;i<moveCount;++i) {
            rVal = max(rVal,calculateBoard(moveList[i],0));
        }
      }
      //Take the highest value move.
      return rVal;
  } else {
    //No moves available.  We lose!
    //We don't like this, obviously.
    return LOWEST;
  }

}


sNN AIPlayer::beta(stdBoard & board, int depth) {
  stdBoard moveList[MAXMOVES];
  int moveCount;
  moveCount = board.genMoves(moveList,1);
  if (moveCount) {
      sNN rVal=HIGHEST;
      for (int i = 0;i<moveCount;++i) {
        rVal = min(rVal,alpha(moveList[i],depth-1));
      }
      return rVal;
  } else {
    //No moves available.  We win!
    //We like this, obviously.
    return HIGHEST;
  }
}

sNN AIPlayer::alphabeta(stdBoard board, unsigned int side) {
  stdBoard boardA[MAXMOVES];
  stdBoard boardB[MAXMOVES];
  unsigned int movesA, movesB;
  sNN moveValA[MAXMOVES];
  sNN moveValB[MAXMOVES];

  movesA = board.genMoves(boardA,1-side);
  for(unsigned int i=0;i<movesA;++i) {
    movesB = boardA[i].genMoves(boardB,side);
    for(unsigned int j=0;j<movesB;++j) {
      moveValB[j] = calculateBoard(boardB[j],0);
    }
    moveValA[i] = *std::max_element(moveValB,moveValB+movesB);
  }
  return *std::min_element(moveValA,moveValA+movesA);
}
