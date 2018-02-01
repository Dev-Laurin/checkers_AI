#include <iostream>
using std::cout; 
using std::endl; 
#include "board.h"
#include <SFML/Graphics.hpp>
#include <vector>
using std::vector; 
#include "gui.h"
#include <random> 

int main(){
	
	char input;  
	bool firstTime = true; 
	while(true){
		checkerBoardGUI gui;
		string results; 

		if(firstTime){
			results = gui.run(); 
			cout << "Game ended. Results: " << results << endl;
			firstTime = false; 
		}
		else{
			std::cin >> input;
			switch(input){
				case 'p': { //Play another game
					results = gui.run(); 
					cout << "Game ended. Results: " << results << endl;
				}
				break; 
				case 'e': { //Exit
					return 0; 
				}
				break;
				default: {
					cout << "Enter 'p' to play, 'e' to exit." << endl;
				}
			} 
		}

		if(results=="Exited game."){
			return 0; 
		}

		 
	}

	return 0; 
}