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
	checkerBoardGUI gui; 
	gui.run(); 
	return 0; 
}