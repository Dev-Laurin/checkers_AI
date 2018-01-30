#ifndef GUI_INCLUDED_H
#define GUI_INCLUDED_H

int findCheckerMove(string newBoard, string oldBoard){
	for(int i=0; i<newBoard.size(); ++i){
		if(newBoard[i]!=oldBoard[i] and
			newBoard[i]!=' '){
			//index of where checker jumped/moved to
			return i; 
		}
	}
}

int findOldCheckerSpace(string newBoard, string oldBoard){
	for(int i=0; i<newBoard.size(); ++i){
		if(newBoard[i]!=oldBoard[i] and newBoard[i]==' '){
			//index of where checker jumped from 
			return i; 
		}
	}
}

class checkerBoardGUI{
public: 
	checkerBoardGUI(){

	}
};

#endif /* GUI_INCLUDED_H */ 