//playgame.cpp
//3-6-18 Revised
//Play a game of NN's

#include <iostream>
#include <vector>
using std::vector;
#include "board.h"
//#include "opponent_AI.h"
#include "neuralNetwork.h"

int main(){
    vector<int> nodes{32, 40, 10, 1};
    NN blondie24(nodes, "blondie24");

    vector<int> nodes2{32, 91, 50, 1};
    NN other(nodes2, "other");

    //start a new game
    stdBoard b;

    //First 3 ply are random
    RandomPlayer RPlayer;

    //200 moves max
    int i = 0;
    b.draw();
    b = RPlayer.getMove(b,false);
    b.draw();
    b = RPlayer.getMove(b,true);
    b.draw();
    b = RPlayer.getMove(b,false);
    b.draw();
    i += 3;
    stdBoard loser(0,0,0,0);
    auto tStart = std::chrono::high_resolution_clock::now();
    auto tStop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed;
    while(i < 200){
      tStart = std::chrono::high_resolution_clock::now();
      b = RPlayer.getMove(b,true);
      if (b == loser) {
          cout << "Player 1, Random, wins in " << i << " moves!" << endl;
          return 1;
      }
      tStop = std::chrono::high_resolution_clock::now();
      elapsed = tStop - tStart;
      cout << "Move " << ++i << ", in " << int(elapsed.count()) << " seconds, Random is:" << endl;
      b.draw();

      tStart = std::chrono::high_resolution_clock::now();
      b = blondie24.getMove(b,false);
          if (b == loser) {
          cout << "Player 2, Blondie, wins in " << i << " moves!" << endl;
          return -1;
      }
      tStop = std::chrono::high_resolution_clock::now();
      elapsed = tStop - tStart;
      cout << "Move " << ++i << ", in " << int(elapsed.count()) << " seconds, Blondie is:" << endl;
      b.draw();
    }
    cout << "It turned out a draw!" << endl;
	return 0;
}
