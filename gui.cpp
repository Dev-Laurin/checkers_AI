#include "gui.h"

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

	if(posBoard>=0){
		positionOnBoard = posBoard;
		convertBoardIndexIntoXYPositions(posBoard);
		piece.setFillColor(color);
		piece.setPosition(x,y);		
	}
	else{
		//position is negative, piece still exists but not in the game, hide it
		positionOnBoard = posBoard; 
		piece.setFillColor(color); 
		piece.setPosition(-100 + -tile_width, -100 + -tile_width); 
	}


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

	redIndex = red_pieces.size(); 
	blackIndex = black_pieces.size(); 

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

	//Show game results 
	gameName.setFont(ubuntuFont); 
	gameName.setCharacterSize(15); 
	gameName.setColor(sf::Color::Black); 
	gameName.setPosition(520, 180); 

	gameWinnerText.setFont(ubuntuFont); 
	gameWinnerText.setCharacterSize(15); 
	gameWinnerText.setColor(sf::Color::Black); 
	gameWinnerText.setPosition(520, 200); 

	gameLoserText.setFont(ubuntuFont); 
	gameLoserText.setCharacterSize(15); 
	gameLoserText.setColor(sf::Color::Black); 
	gameLoserText.setPosition(520, 220); 

	gameBoardText.setFont(ubuntuFont); 
	gameBoardText.setCharacterSize(15); 
	gameBoardText.setColor(sf::Color::Black); 
	gameBoardText.setPosition(520, 240); 

	piece_selected = 0;
	waitForOpponent = false; //player goes first

	//Setup arrows for looking through games  
	rightArrowRect.setSize(sf::Vector2f(20, 5));  
	rightArrowRect.setFillColor(sf::Color::Red); 
	rightArrowRect.setPosition(650, 300); 


	leftArrowRect.setSize(sf::Vector2f(20, 5)); 
	leftArrowRect.setFillColor(sf::Color::Red); 
	leftArrowRect.setPosition(530, 300);  

 
	rightArrowTriangle.setRadius(10);
	rightArrowTriangle.setPointCount(3);
	rightArrowTriangle.setFillColor(sf::Color::Red); 
	rightArrowTriangle.setRotation(90); 
	rightArrowTriangle.setPosition(650 + 30, 292); 


	leftArrowTriangle.setRadius(10); 
	leftArrowTriangle.setPointCount(3); 
	leftArrowTriangle.setFillColor(sf::Color::Red); 
	leftArrowTriangle.setRotation(-90); 
	leftArrowTriangle.setPosition(520, 312); 

	gameWinner = "none"; 
	gameLoser = "none"; 
	isTie = false; 
}

std::string checkerBoardGUI::run(){

	//Draw
	int gameBoardIndex = 0; 
	while(window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
				return "Exited game.";
			}

			if(sf::Event::MouseButtonReleased){
				//change back 
				rightArrowTriangle.setFillColor(sf::Color::Red);
				rightArrowRect.setFillColor(sf::Color::Red); 

				leftArrowRect.setFillColor(sf::Color::Red); 
				leftArrowTriangle.setFillColor(sf::Color::Red); 
				
			}

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				sf::Vector2i position = sf::Mouse::getPosition(window);
				error.setString(""); //reset error message

				if(mode=='g'){

					//We are clicking through the game
					//Left or right arrow clicked? 
					unhighlightMoves(red_tiles); 
					
					//Right Arrow Clicked
					if(position.x <= 690 and position.x >= 650 and
						position.y <= 330 and position.y >= 295){
						//Clicked right arrow
						rightArrowTriangle.setFillColor(sf::Color::Magenta); 
						rightArrowRect.setFillColor(sf::Color::Magenta);

						//make next game appear
						if(gameBoardIndex + 1<gameBoards.size()){
							reDrawBoard(gameBoards[++gameBoardIndex]); 
						}

					}
					else if(position.x >= 500 and position.x <= 550 and
						position.y <= 330 and position.y >= 280){

						leftArrowTriangle.setFillColor(sf::Color::Magenta);
						leftArrowRect.setFillColor(sf::Color::Magenta);  

						//make last game appear
						if(gameBoardIndex - 1>=0){
							reDrawBoard(gameBoards[--gameBoardIndex]);
						}
						 
					}
					//highlight the possible moves from a board generator
					stdBoard temp; 
					temp.updateBoard(gameBoards[gameBoardIndex]); 
					//Get the possible moves 
					stdBoard pBoards[30]; 
					// 0 1 2 3 4 
					// B R B R B
					int side = gameBoardIndex%2; 
					if(gameBoardIndex%2!=0){
						side = 1; 
					}
					int moves = temp.genMoves(pBoards, side); 
					stdBoard thisBoard; 
					thisBoard.updateBoard(gameBoards[gameBoardIndex]); 
					highlightMoves(red_tiles, pBoards, moves, thisBoard); 

					gameBoardText.setString("Board: " + to_string(gameBoardIndex)); 

				}
				else if(mode=='n'){ //we are playing a neural network 
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
							stdBoard move;

							//atomic flag to determine if thread is finished
							std::atomic<bool> done(false);

							stdBoard bCopy = b;
							//thread to compute enemy move while main draws
							std::thread enemy([&done, &bCopy,
								&move]{

								//AI Player 
								RandomPlayer opp; 
								move = opp.getMove(bCopy, true);
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

							b.updateBoard(move.str());
							reDrawBoard(move.str());

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
		}

		window.clear();
		//draw background (black)
		window.draw(board);


		//draw red tile spaces
		for(unsigned int i=0; i<red_tiles.size(); ++i){
			window.draw(red_tiles[i]);
		}

		//draw pieces
		for(unsigned int i=0; i<redIndex; ++i){
			red_pieces[i].draw();
		}

		for(unsigned int i=0; i<blackIndex; ++i){
			black_pieces[i].draw();
		}

		window.draw(debugWindow);

		window.draw(debugText);
		window.draw(turnNotificationText);
		window.draw(error);

		//If in game mode 
		//if(mode=='g'){ //removed due to performance
			window.draw(rightArrowRect); 
			window.draw(rightArrowTriangle); 
			window.draw(leftArrowTriangle); 
			window.draw(leftArrowRect);			
		//}
		window.draw(gameWinnerText); 
		window.draw(gameLoserText); 
		window.draw(gameName);
		window.draw(gameBoardText); 
 
		window.display();				 
	}
}

//redraw checkers board based on std::string
void checkerBoardGUI::reDrawBoard(std::string newBoard){
	//start over
	blackIndex = 0;
	redIndex = 0;
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
}


//Read in the config file
	//Are we watching a previous game? 
	//Or which opponent NN are we playing? 
int checkerBoardGUI::readConfigFile(std::string filename){
	ifstream configFile(filename); 

	if(!configFile){
		cout << "Error opening configuration file: "; 
		cout << filename << endl; 
		return -1; 
	}

	string line; 

	//comments
	getline(configFile, line); 
	getline(configFile, line);
	//reading a game
	getline(configFile, line); 

	if(line=="none"){
		//we are not reading a game 
		mode = 'n'; 
		getline(configFile, line);
		if(line=="none"){
			cout << "Invalid configuration file. Neither "; 
			cout << "reading a game nor playing a neural network "; 
			cout << "is selected." << endl; 
			return -1; 
		}
		//Read the NN in 
		loadFromFile(opponentNN, line); 
	}
	else{
		//We are reading in a game 
		mode = 'g'; 
		//Open the game file and store in a buffer 
		ifstream gameFile(line); 

		if(!gameFile){
			cout << "Error game file could not be opened: "; 
			cout << line << endl; 
			return -1; 
		}

		gameName.setString("File: " + line); 

		//The winner or a tie 
		string gameLine; 
		getline(gameFile, gameLine);
		gameWinner = gameLine; 
		gameWinnerText.setString(gameWinner); 

		//Loser or a tie 
		getline(gameFile, gameLine);
		gameLoser = gameLine; 
		gameLoserText.setString(gameLoser); 

		//Is it actually a tie? 
		isTie = gameLine.find("Tie")!= std::string::npos; 


		//ignore extra file contents until we find first game board 
		stdBoard beginningBoard; 
		while(getline(gameFile, gameLine)){
			if(gameLine.find(beginningBoard.str())!=std::string::npos){
				break; 
			}
		}

		//start reading game boards
		gameBoards.push_back(beginningBoard.str()); 
		while(getline(gameFile, gameLine)){
			gameBoards.push_back(gameLine); 
		}
	}
}