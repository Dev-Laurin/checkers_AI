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
  NN player1(nodes, "Blondie1");
  vector<int> nodes2{128, 128, 40, 1};
  NN2 player2(nodes2, "Bigg");
  //RandomPlayer player2;
  //PieceCount player1;
  vector<stdBoard> gameBoards(200);
  cout << "Starting Game:" << endl;
  int winner = playGame(player1, player2, gameBoards);
  cout << "Winner!" << winner << endl;
}
