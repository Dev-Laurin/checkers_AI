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
	std::vector<sf::CircleShape> black_pieces;
	std::vector<sf::CircleShape> red_pieces;

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
			black_pieces.push_back(piece);
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
			piece.setFillColor(sf::Color(139,0,0,255)); 
			piece.setPosition( x_pos + (tile_width-piece.getRadius())/4, 
		y_pos + (tile_width-piece.getRadius())/4);
			red_pieces.push_back(piece);
			x_pos+=(tile_width*2); 
		}
		y_pos+=tile_width; 
		row = !row; 
	}
			  
	//Debug Window font 
	sf::Font ubuntuFont; 
	if(!ubuntuFont.loadFromFile("NotoSansCJK-Medium.ttc"))
		cout << "Error loading font file." << endl;

	//debug text
	sf::Text debugText; 
	debugText.setFont(ubuntuFont); 
	debugText.setString("Welcome to Checkers!"); 
	debugText.setCharacterSize(15); 
	debugText.setColor(sf::Color::Black); 
	debugText.setPosition(520,20); 

	sf::Text turnNotificationText; 
	turnNotificationText.setFont(ubuntuFont); 
	turnNotificationText.setString("Turn: player.");
	turnNotificationText.setCharacterSize(15); 
	turnNotificationText.setColor(sf::Color::Green); 
	turnNotificationText.setPosition(520, 50); 

	sf::Text error; 
	error.setFont(ubuntuFont); 
	error.setString(""); 
	error.setCharacterSize(15);
	error.setColor(sf::Color::Red); 
	error.setPosition(520, 100); 

	//Mouse
	sf::Mouse user_mouse; 

	sf::CircleShape * selected_piece; 
	bool clickedBefore; 
	bool piece_selected = 0; 

	//make starting board
	stdBoard b;

	bool waitForOpponent = false; //player goes first  

	//Draw 
	while(window.isOpen())
	{
		sf::Event event; 
		while (window.pollEvent(event)){
			if(waitForOpponent){
				//opponent's move
				
				//get random number
				stdBoard possibleBoards[30]; 
				int moves = b.genMoves(possibleBoards,0); //black
				std::mt19937 gen(33); //seed
				std::uniform_int_distribution<int> dis(0,moves); 
				int randMove = dis(gen); 

				//choose move
				string move = possibleBoards[randMove].str(); 

				//move checker in gui
				//find the old blank space 
				int oldIndex = findOldCheckerSpace(move, b.flipBoard());
				//find that checker 
				int checkerIndex = 0; 
				for(int i=0; i<black_pieces.size(); ++i){
					if(black_pieces[i].getPosition().x >= red_tiles[oldIndex].getPosition().x and
					black_pieces[i].getPosition().x <= red_tiles[oldIndex].getPosition().x + tile_width and 
					black_pieces[i].getPosition().y >= red_tiles[oldIndex].getPosition().y and
					black_pieces[i].getPosition().y <= red_tiles[oldIndex].getPosition().y + tile_width){
						checkerIndex = i; 
					}
				}
				//find the new space to jump to  
				int newIndex = findCheckerMove(move, b.flipBoard());

				//move the checker 
				black_pieces[checkerIndex].setPosition(red_tiles[newIndex].getPosition().x + black_pieces[checkerIndex].getRadius()/4, 
				red_tiles[newIndex].getPosition().y + black_pieces[checkerIndex].getRadius()/4); 

				//update board 
				b.updateBoard(move); 
				
				waitForOpponent = false; //player's turn 
				cout.flush(); 
				sf::sleep(sf::milliseconds(1000)); 
				turnNotificationText.setString("Turn: Player's turn.");

			}
			if(event.type == sf::Event::Closed)
				window.close(); 
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				//left mouse was clicked
				sf::Vector2i position = sf::Mouse::getPosition(window);
				error.setString(""); //reset error message

				//if mouse was clicked and piece was selected, move piece?
				if(piece_selected){
					//find if valid tile was selected

					//tile piece is sitting on for reference
					int thisTileXPos = selected_piece->getPosition().x - 
					(tile_width - selected_piece->getRadius())/4;
					int thisTileYPos = selected_piece->getPosition().y - 
					(tile_width - selected_piece->getRadius())/4;

					int index = 0; 

					//find this position in regards to board string
					for(int i=0; i<red_tiles.size(); ++i){
						if(red_tiles[i].getPosition().x >= thisTileXPos and
							red_tiles[i].getPosition().x <= thisTileXPos + tile_width and 
							red_tiles[i].getPosition().y >= thisTileYPos and
							red_tiles[i].getPosition().y <= thisTileYPos + tile_width ){
							//tile is found, multicolor it
							index = i; 
							break; 
						}
					}

					//find tile that user clicked
					int clickedIndex = 0; 
					for(int i=0; i<red_tiles.size(); ++i){
						if(red_tiles[i].getPosition().x <= position.x and
							position.x <= red_tiles[i].getPosition().x + tile_width and 
							red_tiles[i].getPosition().y <= position.y and
							position.y <= red_tiles[i].getPosition().y + tile_width ){
							//tile is found, multicolor it
							clickedIndex = i; 
							break; 
						}
					}

					//get valid moves from this board
						//turn this board into string 
					stdBoard possibleBoards[30]; 
					int movesFound = b.genMoves(possibleBoards, 1);

					vector<int> validPositions; 
					vector<int> indices; 
					//loop through boards to find valid moves
					for(int i=0; i<movesFound; ++i){
					//	"rrrrrrrrrrrr        bbbbbbbbbbbb"
						if(possibleBoards[i].flipBoard().at(index) == ' '){
							//this board has our checker involved
							//cout << "Finding valid positions" << endl; 
							//cout << "PossibleBoard[i] = " << possibleBoards[i].flipBoard() << endl;
							//cout << "Our board flipped: " << b.flipBoard() << endl; 
							validPositions.push_back(findCheckerMove(possibleBoards[i].flipBoard(),
								b.flipBoard()));
							indices.push_back(i); //keep track of position in possible boards for updating later
							//cout << "Found the checker" << endl; 
						}
					} 

					bool valid = false; 
					//see if user clicked a valid position
					for(int i=0; i<validPositions.size(); ++i){
						if(clickedIndex == validPositions[i]){
							//is a valid position, move the piece
							selected_piece->setPosition(red_tiles[clickedIndex].getPosition().x + (tile_width-selected_piece->getRadius())/4,
								red_tiles[clickedIndex].getPosition().y + (tile_width-selected_piece->getRadius())/4);
							valid=true; 
							//update board
							b.updateBoard(possibleBoards[indices[i]].str()); 
							//we need to wait for our opponent now
							waitForOpponent = true; 
							turnNotificationText.setString("Turn: Opponent");
							break; 
						}
						
					}
					if(!valid) //print out to user
						error.setString("Error: Invalid Move.");

				} 

				//turn last selected piece back to normal color
				if(clickedBefore and piece_selected) 
					selected_piece->setFillColor(sf::Color(139,0,0,255));

				//Look to see which red piece was selected
				for(int i=0; i<red_pieces.size(); ++i){
					
					int yArea = red_pieces[i].getPosition().y + 
					red_pieces[i].getRadius()*2;
					int xArea = red_pieces[i].getPosition().x + 
					red_pieces[i].getRadius()*2; 

					if(red_pieces[i].getPosition().y < position.y and 
						position.y < yArea and 
						red_pieces[i].getPosition().x < position.x and
						position.x < xArea){
							//this piece was clicked change its color
						selected_piece = &red_pieces[i]; 
						red_pieces[i].setFillColor(sf::Color::White); 
						piece_selected = true; 
						clickedBefore = true; 
						break; 
					}
					piece_selected = false; 
				}
			}
		}

		window.clear(); 

		//draw background (black)
		window.draw(board); 

		//draw red tile spaces 
		for(int i=0; i<red_tiles.size(); ++i){
			window.draw(red_tiles[i]); 
		}
		//draw pieces
		for(int i=0; i<black_pieces.size(); ++i){
			window.draw(black_pieces[i]); 
			window.draw(red_pieces[i]); 
		}
		window.draw(debugWindow); 
		window.draw(debugText);
		window.draw(turnNotificationText); 
		window.draw(error); 
		window.display(); 
	}
	
	return 0; 
}