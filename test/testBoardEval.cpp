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

  b = player1.getMove(board);


}
