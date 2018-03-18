#include "gui.h"
#include "opponent_AI.h"
#include <thread>
#include <future>
#include <chrono>
//using namespace std::chrono_literals;

//used alot
float tile_width = 62.5;

int findCheckerMove(std::string newBoard, std::string oldBoard){
	for(unsigned int i=0; i<newBoard.size(); ++i){
		if(newBoard[i]!=oldBoard[i] and
			newBoard[i]!=' '){
			//index of where checker jumped/moved to
			return i;
		}
	}
}

void highlightMoves(std::vector<sf::RectangleShape>&tiles, stdBoard boards[],
	int moves, stdBoard oldBoard){

	int newBoardPos;
	for(int i=0; i<moves; ++i){
		newBoardPos = findCheckerMove(boards[i].str(), oldBoard.str());
		tiles[newBoardPos].setFillColor(sf::Color::Magenta);
	}
}

void unhighlightMoves(std::vector<sf::RectangleShape>&tiles){

	for(unsigned int i=0; i<tiles.size(); ++i){
		tiles[i].setFillColor(sf::Color::Red);
	}
}


Checker::Checker(const float R, const int pos, const sf::Color col,
	sf::RenderWindow * win, bool king){
	radius = R;
	positionOnBoard = pos;
	color = col;
	window = win;
	isKing = king;

    //make the checker for drawing
    piece.setRadius(radius);
    piece.setFillColor(color);

    //find x y coordinates of position on board
	updateBoardPosition(positionOnBoard, col, king);
}

void Checker::updateBoardPosition(const int posBoard, sf::Color col,
	bool king){
	isKing = king;
	color = col;

	positionOnBoard = posBoard;
	convertBoardIndexIntoXYPositions(posBoard);
	piece.setFillColor(color);
	piece.setPosition(x,y);
}

void Checker::convertBoardIndexIntoXYPositions(const int positionOnBoard){
	int tileCount = 0;
    for(int row=0; row<8; ++row){
    	for(int col=0; col<4; ++col){
    		if(tileCount==positionOnBoard){
    			y = row * tile_width;
    			if(row%2==0){ //if even row
    				x = col*tile_width*2 + tile_width;
    			}
    			else{
    				x = col*tile_width*2;
    			}
    		}
    		tileCount++;
    	}
    }

	x+=((tile_width-radius)/4);
	y+=((tile_width-radius)/4);
}

void Checker::draw()const{
	window->draw(piece);
}

checkerBoardGUI::checkerBoardGUI(){
	//create/draw blank board
	float windowY = 500;
	float windowX = 700;

	window.create(sf::VideoMode(windowX,windowY), "Checkers");
	board.setSize(sf::Vector2f(500.f, 500.f));
	debugWindow.setSize(sf::Vector2f(190.f, 490.f));

	board.setFillColor(sf::Color::Black);

	debugWindow.setFillColor(sf::Color::White);
	debugWindow.setPosition(505,5);

	bool row = true; //1 = odd, 0 = even
	float x_pos = 0;
	float y_pos = 0;


	//draw tiles
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

	//Create pawns
	for(unsigned int i=20; i<b.str().size(); ++i){
		Checker r(22.f, i-20, sf::Color(139,0,0,255), &window, false);
		red_pieces.push_back(r);
		Checker bl(22.f, i, sf::Color::Black, &window, false);
		black_pieces.push_back(bl);
	}

	//Debug Window font
	if(!ubuntuFont.loadFromFile("NotoSansCJK-Medium.ttc"))
		cout << "Error loading font file." << endl;

	//debug text
	debugText.setFont(ubuntuFont);
	debugText.setString("Welcome to Checkers!");
	debugText.setCharacterSize(15);
	debugText.setColor(sf::Color::Black);
	debugText.setPosition(520,20);

	turnNotificationText.setFont(ubuntuFont);
	turnNotificationText.setString("Turn: player.");
	turnNotificationText.setCharacterSize(15);
	turnNotificationText.setColor(sf::Color::Green);
	turnNotificationText.setPosition(520, 50);

	error.setFont(ubuntuFont);
	error.setString("");
	error.setCharacterSize(15);
	error.setColor(sf::Color::Red);
	error.setPosition(520, 100);

	piece_selected = 0;
	waitForOpponent = false; //player goes first
}

std::string checkerBoardGUI::run(){

		//Draw
	while(window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
				return "Exited game.";
			}

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				sf::Vector2i position = sf::Mouse::getPosition(window);
				error.setString(""); //reset error message



				//move checker?
				if(piece_selected){

					//unhighlight enemy postions
					unhighlightMoves(red_tiles);

					//find tile that user clicked
					int clickedIndex = 9000;
					for(unsigned int i=0; i<red_tiles.size(); ++i){
						if(red_tiles[i].getPosition().x <= position.x and
							position.x <= red_tiles[i].getPosition().x + tile_width and
							red_tiles[i].getPosition().y <= position.y and
							position.y <= red_tiles[i].getPosition().y + tile_width ){
							//tile is found
							clickedIndex = i;
							break;
						}
					}

					//get valid moves from this board
					//turn this board into std::string
					stdBoard possibleBoards[30];
					int movesFound = b.genMoves(possibleBoards, 0);//black

					if(movesFound<=0){
						std::string results;
						//there are no moves, count pieces to see who won
						if(black_pieces.size()>red_pieces.size()){
							//red won
							results = "GG you won!";
						}
						else if(black_pieces.size()<red_pieces.size()){
							results = "You lost, GG.";
						}
						else{
							//same amount of pieces
							results = "We tied.";

						}
						window.close();
						return results;
					}
					std::vector<int> validPositions;
					std::vector<int> indices;
					//loop through boards to find valid moves
					for(int i=0; i<movesFound; ++i){
					//	"rrrrrrrrrrrr        bbbbbbbbbbbb"
						if(possibleBoards[i].str().at(selected_piece->positionOnBoard) == ' '){
							//this board has our checker involved
							validPositions.push_back(findCheckerMove(possibleBoards[i].str(),
								b.str()));

							indices.push_back(i); //keep track of position in possible boards for updating later
						}
					}

					bool valid = false;
					//see if user clicked a valid position
					for(unsigned int i=0; i<validPositions.size(); ++i){
						if(clickedIndex == validPositions[i]){
							//is a valid position
							valid=true;
							//update board
							b.updateBoard(possibleBoards[indices[i]].str());
							reDrawBoard(b.str());
							//we need to wait for our opponent now
							waitForOpponent = true;
							turnNotificationText.setString("Turn: Opponent");
							break;
						}

					}
					if(!valid){ //print out to user
						error.setString("Error: Invalid Move.");

						highlightMoves(red_tiles, possibleBoards, movesFound, b);

						if(piece_selected and selected_piece->isKing)
							selected_piece->piece.setFillColor(sf::Color::Blue);
						else if(piece_selected)
							selected_piece->piece.setFillColor(sf::Color::Black);
						piece_selected = false;

						//check if it was another piece that was selected instead
						//See which piece was selected
						for(unsigned int i=0; i<black_pieces.size(); ++i){
							int yArea = black_pieces[i].y +
							black_pieces[i].radius*2;
							int xArea = black_pieces[i].x +
							black_pieces[i].radius*2;

							if(black_pieces[i].y < position.y and
								position.y < yArea and
								black_pieces[i].x < position.x and
								position.x < xArea){
									//this piece was clicked change its color
								selected_piece = &black_pieces[i];
								black_pieces[i].piece.setFillColor(sf::Color::White);
								piece_selected = true;
								clickedBefore = true;
								break;
							}
							piece_selected = false;
						}
					}

					//Random opponent turn
					if(waitForOpponent){
						//unhighlight player moves
						unhighlightMoves(red_tiles);

						//turn last selected piece back to normal color
						piece_selected = true;
						if(piece_selected and selected_piece->isKing)
							selected_piece->piece.setFillColor(sf::Color::Blue);
						else if(piece_selected)
							selected_piece->piece.setFillColor(sf::Color::Black);

						//check if there are moves left
						stdBoard possibleBoards[30];
						int moves = b.genMoves(possibleBoards,1); //red
						if(moves<=0){
							std::string results;
							//there are no moves, count pieces to see who won
							if(black_pieces.size()>red_pieces.size()){
								//red won
								results = "GG you won!";
							}
							else if(black_pieces.size()<red_pieces.size()){
								results = "You lost, GG.";
							}
							else{
								//same amount of pieces
								results = "We tied.";

							}
							window.close();
							return results;
						}

						//parameters
						char opponentAIType = 'g';
						std::string move;

						//atomic flag to determine if thread is finished
						std::atomic<bool> done(false);

						stdBoard bCopy = b;
						//thread to compute enemy move while main draws
						std::thread enemy([&done, &bCopy,
							&move, &opponentAIType]{

							Opponent opp(opponentAIType);
							move = opp.getMove(bCopy);
							done = true;
						});

						//if main thread, keep drawing until enemy thread joins
						while(!done and window.isOpen()){
							sf::Event event;
							while (window.pollEvent(event) and !done){
								if(event.type == sf::Event::Closed){
									window.close();
									return "Exited game.";
								}
								window.clear();
								//draw background (black)
								window.draw(board);


								//draw red tile spaces
								for(unsigned int i=0; i<red_tiles.size(); ++i){
									window.draw(red_tiles[i]);
								}

								//draw pieces
								for(unsigned int i=0; i<red_pieces.size(); ++i){
									red_pieces[i].draw();
								}

								for(unsigned int i=0; i<black_pieces.size(); ++i){
									black_pieces[i].draw();
								}

								window.draw(debugWindow);

								window.draw(debugText);
								window.draw(turnNotificationText);
								window.draw(error);
								window.display();
							}
						}
						enemy.join();

						//Show possible moves from gui
						highlightMoves(red_tiles, possibleBoards, moves,
							b);

						b.updateBoard(move);
						reDrawBoard(move);

						waitForOpponent=false;
						turnNotificationText.setString("Turn: Player");

						piece_selected = false;
					}
				}
				else if(!waitForOpponent) {
					//unhighlight enemy moves
					unhighlightMoves(red_tiles);

					//turn last selected piece back to normal color
					if(piece_selected and selected_piece->isKing)
						selected_piece->piece.setFillColor(sf::Color::Blue);
					else if(piece_selected)
						selected_piece->piece.setFillColor(sf::Color::Black);

					//See which piece was selected
					for(unsigned int i=0; i<black_pieces.size(); ++i){
						int yArea = black_pieces[i].y +
						black_pieces[i].radius*2;
						int xArea = black_pieces[i].x +
						black_pieces[i].radius*2;

						if(black_pieces[i].y < position.y and
							position.y < yArea and
							black_pieces[i].x < position.x and
							position.x < xArea){
								//this piece was clicked change its color
							selected_piece = &black_pieces[i];
							black_pieces[i].piece.setFillColor(sf::Color::White);
							piece_selected = true;
							clickedBefore = true;
							break;
						}
						piece_selected = false;
					}

						stdBoard possibleBoards[30];
						int moves = b.genMoves(possibleBoards,0);

						if(moves<=0){
							std::string results;
							//there are no moves, count pieces to see who won
							if(black_pieces.size()>red_pieces.size()){
								//red won
								results = "GG you won!";
							}
							else if(black_pieces.size()<red_pieces.size()){
								results = "You lost, GG.";
							}
							else{
								//same amount of pieces
								results = "We tied.";

							}
							window.close();
							return results;
						}

						//highlight possible player moves
						highlightMoves(red_tiles, possibleBoards, moves, b);

				}


			}
		}

		window.clear();
		//draw background (black)
		window.draw(board);


		//draw red tile spaces
		for(unsigned int i=0; i<red_tiles.size(); ++i){
			window.draw(red_tiles[i]);
		}

		//draw pieces
		for(unsigned int i=0; i<red_pieces.size(); ++i){
			red_pieces[i].draw();
		}

		for(unsigned int i=0; i<black_pieces.size(); ++i){
			black_pieces[i].draw();
		}

		window.draw(debugWindow);

		window.draw(debugText);
		window.draw(turnNotificationText);
		window.draw(error);
		window.display();
	}
}

//redraw checkers board based on std::string
void checkerBoardGUI::reDrawBoard(std::string newBoard){
	//start over
	//black_pieces.clear();
	//red_pieces.clear();
	int blackIndex = 0;
	int redIndex = 0;
	for(unsigned int i=0; i<newBoard.size(); ++i){
		if(newBoard[i]=='b'){
			//draw a black checker there
			//Checker n(22.f, i, sf::Color::Black, &window, false);
			//black_pieces.push_back(n);
			black_pieces[blackIndex].updateBoardPosition(i, sf::Color::Black,
				false);
			blackIndex++;
		}
		else if(newBoard[i]=='r'){
			//Checker n(22.f, i, sf::Color(139,0,0,255), &window, false);
			//red_pieces.push_back(n);
			red_pieces[redIndex].updateBoardPosition(i, sf::Color(139,0,0,255),
				false);
			redIndex++;

		}
		else if(newBoard[i]=='B'){
			//Checker n(22.f, i, sf::Color::Blue, &window, true);
			//black_pieces.push_back(n);
			black_pieces[blackIndex].updateBoardPosition(i,
				sf::Color::Blue, true);
			blackIndex++;
		}
		else if(newBoard[i]=='R'){
			//Checker n(22.f, i, sf::Color::Green, &window, true);
			//red_pieces.push_back(n);
			red_pieces[redIndex].updateBoardPosition(i,
				sf::Color::Green, true);
			redIndex++;
		}
	}

	//delete pieces
	for(unsigned int i=redIndex+1; i<=red_pieces.size(); ++i){
		red_pieces.erase(red_pieces.end()-1);

	}
	for(unsigned int i=blackIndex+1; i<=black_pieces.size(); ++i){
		black_pieces.erase(black_pieces.end()-1);
	}
}
