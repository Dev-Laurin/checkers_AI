//playgame.cpp
//3-6-18 Revised
//Play a game of NN's

#include <iostream>
#include <vector>
using std::vector;
#include "board.h"
//#include "opponent_AI.h"
#include "neuralNetwork.h"

int playGame(AIPlayer & player1, AIPlayer & player2){
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

    //To tell who lost, and if the game is over
    stdBoard loser(0,0,0,0);

    while(i < 200){
        b = player1.getMove(b,true);
        if (b == loser) {
            cout << "Player 1 wins in " << i << " moves!" << endl;
            return 1;
        }

        b.draw();

        b = player2.getMove(b,false);
            if (b == loser) {
              cout << "Player 2 wins in " << i << " moves." << endl;
            return -1;
        }

        b.draw();
    }

    //Count up the remaining pieces to see who won
    cout << "draw" << endl;
    return 0; 

}

int main(){
    vector<int> nodes{32, 40, 10, 1};
    NN blondie24(nodes, "blondie24");

    vector<int> nodes2{32, 91, 50, 1};
    NN other(nodes2, "other");

    int results = playGame(blondie24, other); 
    if(results>0){
        cout << "Player 1 won" << endl;
    }
    else if(results<0){
        cout << "Player 2 won" << endl;
    }
    else{
        cout << "Tie" << endl;
    }

    cout << "It turned out a draw!" << endl;
	return 0;
}
