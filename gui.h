#ifndef GUI_INCLUDED_H
#define GUI_INCLUDED_H
#include <random>
#include <string> 
#include "board.h"
#include <SFML/Graphics.hpp>
#include <vector> 

//helper functions 
int findCheckerMove(std::string newBoard, std::string oldBoard);

void highlightMoves(std::vector<sf::RectangleShape>&tiles, stdBoard boards[], 
	int moves, stdBoard oldBoard);

void unhighlightMoves(std::vector<sf::RectangleShape>&tiles);

class Checker{
public:
	Checker(const float R, const int pos, const sf::Color col,
		sf::RenderWindow * win, bool king);

	void updateBoardPosition(const int posBoard, sf::Color col, 
		bool king);

	void convertBoardIndexIntoXYPositions(const int positionOnBoard);

	void draw()const;

	//variables 
	sf::CircleShape piece;
	sf::Color color;
	sf::RenderWindow * window;
	int x;
	int y;
	float radius;
	int positionOnBoard;

	bool isKing;
};

class checkerBoardGUI{
public:
	checkerBoardGUI();
	std::string run();
	//redraw checkers board based on string
	void reDrawBoard(std::string newBoard);

	//variables 
	stdBoard b;
	bool waitForOpponent;
	Checker * selected_piece;
	bool clickedBefore;
	bool piece_selected;

	std::vector<sf::RectangleShape> red_tiles;
	std::vector<Checker> black_pieces;
	std::vector<Checker> red_pieces;

	sf::RenderWindow window;
	sf::RectangleShape board;
	sf::RectangleShape debugWindow;

	sf::Text error;
	sf::Text turnNotificationText;
	sf::Text debugText;

	sf::Font ubuntuFont;

};

#endif /* GUI_INCLUDED_H */
