#include <iostream>
using std::cout; 
using std::endl; 
#include "board.h"
#include <SFML/Graphics.hpp>
#include <vector>
using std::vector; 

int main(){

	float tile_width = 62.5; 
	float tile_position; 

	float windowY = 500;
	float windowX = 700;  

	sf::RenderWindow window(sf::VideoMode(windowX,windowY), "Checkers"); 
	sf::RectangleShape board(sf::Vector2f(500.f, 500.f)); 
	sf::RectangleShape debugWindow(sf::Vector2f(190.f, 490.f)); 
 
	board.setFillColor(sf::Color::Black); 

	debugWindow.setFillColor(sf::Color::White); 
	debugWindow.setPosition(505,5); 

	bool row = true; //1 = odd, 0 = even 
	float x_pos = 0;  
	float y_pos = 0;  
	vector<sf::RectangleShape> red_tiles; 
	std::vector<sf::CircleShape> pieces;

	for(int y=0; y<8; ++y){
		x_pos=0; 
		if(row){ //odd row, black tile first on left
			x_pos=tile_width;  
		}
		for(int x=0; x<4; ++x){
			//Create Red Tiles
			sf::RectangleShape red_tile(sf::Vector2f(62.5f, 62.5f)); 
			red_tile.setPosition(x_pos,y_pos); 
			red_tile.setFillColor(sf::Color::Red);
			red_tiles.push_back(red_tile); 
			x_pos+=(tile_width*2);
		}
		y_pos+=tile_width; 
		row = !row;
	}

	//Create Black pieces
	y_pos = 0; 
	for(int y=0; y<3; ++y){
		x_pos=0; 
		if(row){
			x_pos=tile_width; 
		}
		for(int x=0; x<4; ++x){
			sf::CircleShape piece(22.f); 
			piece.setFillColor(sf::Color::Black); 
			piece.setPosition( x_pos + (tile_width-piece.getRadius())/4, 
		y_pos + (tile_width-piece.getRadius())/4);
			pieces.push_back(piece);
			x_pos+=(tile_width*2); 
		}
		y_pos+=tile_width; 
		row = !row; 
	}

	//Create Red pieces
	y_pos = 5*tile_width; 
	for(int y=0; y<3; ++y){
		x_pos=0; 
		if(row){
			x_pos=tile_width; 
		}
		for(int x=0; x<4; ++x){
			sf::CircleShape piece(22.f); 
			piece.setFillColor(sf::Color::White); 
			piece.setPosition( x_pos + (tile_width-piece.getRadius())/4, 
		y_pos + (tile_width-piece.getRadius())/4);
			pieces.push_back(piece);
			x_pos+=(tile_width*2); 
		}
		y_pos+=tile_width; 
		row = !row; 
	}
			  
	//Debug Window font 
	

	//Draw 
	while(window.isOpen())
	{
		sf::Event event; 
		while (window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close(); 
		}

		window.clear(); 

		//draw background (black)
		window.draw(board); 
		//draw red tile spaces 
		for(int i=0; i<red_tiles.size(); ++i){
			window.draw(red_tiles[i]); 
		}
		//draw black pieces
		for(int i=0; i<pieces.size(); ++i){
			window.draw(pieces[i]); 
		}
		window.draw(debugWindow);  
		window.display(); 
	}
	
	return 0; 
}