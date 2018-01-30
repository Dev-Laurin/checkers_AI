#ifndef GUI_INCLUDED_H
#define GUI_INCLUDED_H

void getBoardFromString(vector<sf::CircleShape> black, 
	vector<sf::CircleShape> red, vector<sf::RectangleShape> tiles, 
	string board){
	
} 

int findCheckerMove(string newBoard, string oldBoard){
	for(int i=0; i<newBoard.size(); ++i){
		if(newBoard[i]!=oldBoard[i] and
			newBoard[i]!=' '){
			//index of where checker jumped/moved to
			return i; 
		}
	}
}

#endif /* GUI_INCLUDED_H */ 