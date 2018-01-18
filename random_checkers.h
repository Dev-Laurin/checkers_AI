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
	vector<vector<int> > board; 
	vector<pon> ourPons; 
	vector<pon> enemyPons; 
}; 

#endif /* RANDOM_CHECKERS_INCLUDED */ 