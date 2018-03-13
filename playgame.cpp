//playgame.cpp
//3-12-18 Revised
//Play a game of NN's

#include <iostream>
#include <vector>
using std::vector;
#include "board.h"
#include "neuralNetwork.h"

//Play a game using tournament rules with 2 AIPlayers
int playGame(AIPlayer & player1, AIPlayer & player2, vector<stdBoard> & boards){

    //start a new game
    stdBoard b;

    //First 3 ply are random
    RandomPlayer RPlayer;

    //200 moves max
    int i = 0;
//    b.draw();
    b = RPlayer.getMove(b,false);
//    b.draw();
    b = RPlayer.getMove(b,true);
//    b.draw();
    b = RPlayer.getMove(b,false);
//    b.draw();
    i += 3;

    int index = 0; 
    //To tell who lost, and if the game is over
    stdBoard loser(0,0,0,0);

    while(i < 100){
        b = player1.getMove(b,true);
        boards[index] = b;  
        ++index; 
        if (b == loser) {
            ++player1.wins; 
            ++player2.losses; 
            return 1;
        }

        b = player2.getMove(b,false);
        boards[index] = b; 
        ++index; 
        if (b == loser) {
            ++player2.wins; 
            ++player1.losses;
            return -1;
        }
    }

    //a draw 
    ++player1.draws; 
    ++player2.draws; 
    return 0; 

}
