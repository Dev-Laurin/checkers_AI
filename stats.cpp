
#include <iostream>
#include <vector>
using std::vector; 
#include "neuralNetwork.h"

int readGameFile(std::string filename, 
	vector<string> &gameBoards){
 
	//Open the game file and store in a buffer 
	ifstream gameFile(filename); 

	if(!gameFile){
		cout << "Error game file could not be opened: "; 
		cout << line << endl; 
		return -1; 
	}

	//ignore extra file contents until we find first game board 
	stdBoard beginningBoard; 
	while(getline(gameFile, gameLine)){
		if(gameLine.find(beginningBoard.str())!=std::string::npos){
			break; 
		}
	}

	//start reading game boards
	gameBoards.push_back(beginningBoard.str()); 
	while(getline(gameFile, gameLine)){
		gameBoards.push_back(gameLine); 
	}
	
}

int main(){
	//Recreate an AI game to get stats out of 
	vector<int> nodes{32, 40, 10, 1};
    NN blondie1(nodes, "blondie");
    NN blondie2(nodes, "blondie");

    //Read in the game file 
    vector<string>gameBoards; 
    readGameFile("game001.txt", gameBoards); 

    //Get stats on boards we used - we used these boards (indices)
    vector<int>boardsUsed = {0, 12, 39, 56, 95}; 

    //For each ply 
    vector<int>ply = {2, 3, 4}; //is actually 4, 6, 8 ply
//    for(auto &j: ply){
    	
    	for(auto &i : boardsUsed){
	    	//calculate the board 
	    	blondie1.getMove(gameBoards[i]); 
	    	//print the stats 
	    	blondie1.prntstats(); 	


    	}
//    }


    

}