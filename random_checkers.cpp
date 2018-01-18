#include "random_checkers.h"

randomCheckers::randomCheckers(){
	//Create the initial board 8 X 8 
		//-1 is a non-usable space (like black or red)
		//0 is empty space
		//1 is our pon
		//2 is enemy pon
	vector<vector<int> > board(8, 
		vector<int>(8, 0)); //default value is 0

	//place our pons in the starting positions
	vector<int> even = {1,-1,1,-1,1,-1,1,-1}; 
	vector<int> odd = {-1,1,-1,1,-1,1,-1,1}; 
	board[7] = odd; 
	board[6] = even; 
	board[5] = odd; 

	//empty spaces in between 
	vector<int> empty1 = {0,-1,0,-1,0,-1,0,-1}; 
	vector<int> empty2 = {-1,0,-1,0,-1,0,-1,0}; 
	board[4] = empty1; 
	board[3] = empty2; 

	//place opponent pons
	vector<int> opp_even = {-1,2,-1,2,-1,2,-1,2}; 
	vector<int> opp_odd = {2,-1,2,-1,2,-1,2,-1}; 
	board[2] = opp_odd; 
	board[1] = opp_even; 
	board[0] = opp_odd; 


	//keep track of the pons themselves
	/*
	for(int i=0; i<4; ++i){
		ourPons.push_back()
	}
	ourPons = {}; 
*/ 
}