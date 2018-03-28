#ifndef ALPHABETA_H_INCLUDED
#define ALPHABETA_H_INCLUDED

#include <bitset>
#include <random>
#include <algorithm>

using std::max;
using std::min;

#include <limits>

#include "board.h"

#ifndef CONSTS
#define CONSTS
unsigned const static int MAXMOVES = 32;
typedef double sNN;
constexpr sNN LOWEST = -10000;
constexpr sNN HIGHEST = 10000;
#endif
std::mt19937 generator0();
std::uniform_real_distribution<sNN> distro(0.0,1.0);

//A simple piececount AI
sNN boardCount(stdBoard board) {

  sNN count = (int)(
        board.pieces[0].count() +
        board.pieces[2].count() -
        board.pieces[1].count() -
        board.pieces[3].count()) +
        distro(generator0);
  return count;
}

sNN beta(stdBoard board, int depth);
//Alpha is the MAX value selector.
//Always assumed to be black.
//Flip the board to computer run red.
//Because a neural network may be good at selecting maximums
//but not minimums.
sNN alpha(stdBoard board, int depth) {
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
            rVal = max(rVal,boardCount(moveList[i]));
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

//Beta is the MINIMUM value selector.
//Always assumed to be red.
sNN beta(stdBoard board, int depth) {
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

sNN alphabeta(stdBoard board, unsigned int side = 0) {
  stdBoard boardA[MAXMOVES];
  stdBoard boardB[MAXMOVES];
  unsigned int movesA, movesB;
  sNN moveValA[MAXMOVES];
  sNN moveValB[MAXMOVES];

  movesA = board.genMoves(boardA,1-side);
  for(unsigned int i=0;i<movesA;++i) {
    movesB = boardA[i].genMoves(boardB,side);
    for(unsigned int j=0;j<movesB;++j) {
      moveValB[j] = boardCount(boardB[j]);
    }
    moveValA[i] = *std::max_element(moveValB,moveValB+movesB);
  }
  return *std::min_element(moveValA,moveValA+movesA);
}



#endif // ALPHABETA_H_INCLUDED
