#include <iostream>
using std::cout; 
using std::endl; 
#include "board.h"
#include "gui.h"

int main(){
	
	char input;  
	bool firstTime = true; 
	while(true){
		string results; 

		if(firstTime){
			cout << "Press p to play again or e to exit." << endl;
			checkerBoardGUI gui;
			gui.readConfigFile("config.txt"); 
			results = gui.run(); 
			cout << "Game ended. Results: " << results << endl;
			firstTime = false; 
		}
		else{
			std::cin >> input;
			switch(input){
				case 'p': { //Play another game
					checkerBoardGUI gui;
					gui.readConfigFile("config.txt");
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