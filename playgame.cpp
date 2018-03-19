//playgame.cpp
//3-12-18 Revised
//Play a game of NN's

#include <iostream>
#include <vector>
using std::vector;
#include "board.h"
#include "neuralNetwork.h"

//Play a game using tournament rules with 2 AIPlayers
int playGame(AIPlayer & player1, AIPlayer & player2, vector<stdBoard> & gameBoards){
    //start a new game
    stdBoard b;
    vector<stdBoard> playerMoves[2];


    //First 3 ply are random
    RandomPlayer RPlayer;

    //200 moves max
    int i = 0;
    gameBoards[i] = b;
//    b.draw();
    b = RPlayer.getMove(b,false);
    gameBoards[++i] = b;
//    b.draw();
    b = RPlayer.getMove(b,true);
    gameBoards[++i] = b;
//    b.draw();
    b = RPlayer.getMove(b,false);
    gameBoards[++i] = b;
    cout << "The starting random board: " << endl;
    b.draw();

    //To tell who lost, and if the game is over
    stdBoard loser(0,0,0,0);

    auto tStart = std::chrono::high_resolution_clock::now();
    auto tStop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed;

    while(i < 199){
        tStart = std::chrono::high_resolution_clock::now();
        b = player1.getMove(b,true);
        if (b == loser) {
            ++player1.wins;
            ++player2.losses;
            return 1;
        }
        tStop = std::chrono::high_resolution_clock::now();
        elapsed = tStop - tStart;
        cout << "CPlayer 1 Move " << i << ", in " << int(elapsed.count()) << " seconds, board is:" << endl;
        if (player1.timeExceeded) {
          cout << "Move search was time limited" << endl;
        }
        b.draw();
        cout << "Cache Effectiveness: " << player1.cacheHit << "/" << player1.cacheHit+player1.cacheMiss;
        cout << " Cache size: " << player1.boardMem.size() << endl;
        if (std::find(playerMoves[0].begin(),playerMoves[0].end(),b) < playerMoves[0].end()) {
          cout << "Cycle Detected, draw!" << endl;
          break; //cycle detected, tie!
        }
        gameBoards[++i] = b;
        playerMoves[0].push_back(b);

        tStart = std::chrono::high_resolution_clock::now();
        b = player2.getMove(b,false);
        if (b == loser) {
            ++player2.wins;
            ++player1.losses;
            return -1;
        }
        tStop = std::chrono::high_resolution_clock::now();

        elapsed = tStop - tStart;
        cout << "CPlayer 2 Move " << i << ", in " << int(elapsed.count()) << " seconds, board is:" << endl;
        if (player2.timeExceeded) {
          cout << "Move search was time limited" << endl;
        }
        b.draw();
        cout << "Cache Effectiveness: " << player2.cacheHit << "/" << player2.cacheHit+player2.cacheMiss;
        cout << " Cache size: " << player2.boardMem.size() << endl;

        if (std::find(playerMoves[1].begin(),playerMoves[1].end(),b) < playerMoves[1].end()) {
          cout << "Cycle Detected, draw!" << endl;
          break; //cycle detected, tie!
        }
        gameBoards[++i] = b;
        playerMoves[1].push_back(b);

    }

    //a draw
    ++player1.draws;
    ++player2.draws;
    return 0;

}
