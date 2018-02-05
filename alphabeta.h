#ifndef ALPHABETA_H_INCLUDED
#define ALPHABETA_H_INCLUDED

#include <bitset>
#include <random>
#include <algorithm>
#include "board.h"

std::default_random_engine gen;
std::uniform_real_distribution<double> distro(0.0,1.0);

unsigned const static int MAXMOVES = 32;

//A simple piececount AI
double boardCount(stdBoard board) {
  double count = board.pieces[0].count() +
      board.pieces[2].count() -
      board.pieces[1].count() -
      board.pieces[3].count() +
      distro(gen); // random number between 0 and 1 to randomize selection of equal values.
  return count;
}

double alphabeta(stdBoard board, unsigned int side = 0) {
  stdBoard boardA[MAXMOVES];
  stdBoard boardB[MAXMOVES];
  unsigned int movesA, movesB;
  double moveValA[MAXMOVES];
  double moveValB[MAXMOVES];

  movesA = board.genMoves(boardA,1-side);
  for(unsigned int i=0;i<movesA;++i) {
    movesB = boardA[i].genMoves(boardB,side);
    for(unsigned int j=0;j<movesB;++j) {
      moveValB[j] = boardCount(boardB[j]);
    }
    moveValA[i] = std::max_element(moveValB,moveValB+movesB);
  }
  return std::min_element(moveValA,moveValA+movesA);
}



#endif // ALPHABETA_H_INCLUDED
