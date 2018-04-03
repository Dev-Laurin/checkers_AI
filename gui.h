#ifndef GUI_INCLUDED_H
#define GUI_INCLUDED_H

#include <string>
#include "board.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include <iostream> 
#include <fstream> 
#include "neuralNetwork.h"
using std::getline; 
using std::cout; 
using std::endl; 
using std::string; 

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
	//Runs the program 
	std::string run();
	//redraw checkers board based on string
	void reDrawBoard(std::string newBoard);
	//read in the config file
	int readConfigFile(std::string filename); 


	//variables
	stdBoard b;
	bool waitForOpponent;
	Checker * selected_piece;
	bool clickedBefore;
	bool piece_selected;

	std::vector<sf::RectangleShape> red_tiles;
	std::vector<Checker> black_pieces;
	std::vector<Checker> red_pieces;

	int redIndex; 
	int blackIndex; 

	sf::RenderWindow window;
	sf::RectangleShape board;
	sf::RectangleShape debugWindow;

	sf::Text error;
	sf::Text turnNotificationText;
	sf::Text debugText;
	sf::Text gameWinnerText; 
	sf::Text gameLoserText; 
	sf::Text gameName; 
	sf::Text gameBoardText; 

	sf::Font ubuntuFont;

	//Arrows for looking through games
	sf::RectangleShape rightArrowRect; 
	sf::RectangleShape leftArrowRect; 

	sf::CircleShape rightArrowTriangle; 
	sf::CircleShape leftArrowTriangle; 

	//Config file vars 
	std::string inputFilename; 
	char mode; //either 'g' or 'n' for game vs neural network play

	//Reading in a game 
	vector<string>gameBoards; 
	string gameWinner; 
	string gameLoser; 
	bool isTie; 

	//blondie005 3 20 
	//005 : 5 generation
	//3: evolved 3 times since original 
	//20 : NN filename 

	//Playing a NN 
	NN opponentNN; 
};

#endif /* GUI_INCLUDED_H */
