#ifndef RANDOM_CHECKERS_INCLUDED
#define RANDOM_CHECKERS_INCLUDED

#include <vector>
using std::vector; 

class pon{
public: 
	int x; //positions on board
	int y; 
}; 

class randomCheckers{
public: 
	randomCheckers(); 
	void play(); 
	/*firstPlayersMove*/ void secondPlayersMove(); 
private: 
	int bk; 
	int red; 
	int bkKing; 
	int redKing; 
}; 

#endif /* RANDOM_CHECKERS_INCLUDED */ 