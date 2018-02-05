#ifndef GUI_INCLUDED_H
#define GUI_INCLUDED_H
#include <random>

int findCheckerMove(string newBoard, string oldBoard){
	for(unsigned int i=0; i<newBoard.size(); ++i){
		if(newBoard[i]!=oldBoard[i] and
			newBoard[i]!=' '){
			//index of where checker jumped/moved to
			return i;
		}
	}
}

//used alot
float tile_width = 62.5;

class Checker{
public:
	Checker(const float R, const int pos, const sf::Color col,
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

	void updateBoardPosition(const int posBoard, sf::Color col, 
		bool king){
		isKing = king; 
		color = col; 

		positionOnBoard = posBoard;
		convertBoardIndexIntoXYPositions(posBoard);
		piece.setFillColor(color); 
		piece.setPosition(x,y);
	}

	void convertBoardIndexIntoXYPositions(const int positionOnBoard){
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

	void draw()const{
		window->draw(piece);
	}

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
	checkerBoardGUI(){
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

	string run(){

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
						//turn this board into string
						stdBoard possibleBoards[30];
						int movesFound = b.genMoves(possibleBoards, 0);//black
						if(movesFound==0){
							string results;
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
						vector<int> validPositions;
						vector<int> indices;
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
						if(!valid) //print out to user
							error.setString("Error: Invalid Move.");

						//Random opponent turn
						if(waitForOpponent){
							//turn last selected piece back to normal color
							if(piece_selected and selected_piece->isKing)
								selected_piece->piece.setFillColor(sf::Color::Blue);
							else if(piece_selected)
								selected_piece->piece.setFillColor(sf::Color::Black);

							//get random number
							stdBoard possibleBoards[30];
							int moves = b.genMoves(possibleBoards,1); //red
							if(moves<=0){
								string results;
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

							int randMove;
							if(moves==1){
								randMove = 0;
							}
							else{
								std::mt19937 gen(time(0)); //seed
								std::uniform_int_distribution<int> dis(0,moves-1);
								randMove = dis(gen);
							}

							//choose move
							string move = possibleBoards[randMove].str();

							b.updateBoard(move);
							reDrawBoard(move);

							waitForOpponent=false;
							cout.flush();
							sf::sleep(sf::milliseconds(1000));
							turnNotificationText.setString("Turn: Player");
						}
					}

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

	//redraw checkers board based on string
	void reDrawBoard(string newBoard){
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

		//cout << "Delete pieces" << endl; 
		//delete pieces
		for(int i=redIndex+1; i<=red_pieces.size(); ++i){
		//	cout << "Delete red " << i << "red pieces: " << red_pieces.size();
		//	cout << endl;
			red_pieces.erase(red_pieces.end()-1);

		}
		for(int i=blackIndex+1; i<=black_pieces.size(); ++i){
		//	cout << "Delete black " << i << "black pieces: " << black_pieces.size();
		//	cout << endl;
			black_pieces.erase(black_pieces.end()-1); 
		}

		//cout << "Red pieces: " << red_pieces.size() << endl;
		//cout << "Black pieces: " << black_pieces.size() << endl; 

		//debugging
		// for(int j=0; j<red_pieces.size(); j++){
		// 	cout << "Red piece at " << red_pieces[j].positionOnBoard; 
		// 	cout << "( " << red_pieces[j].x << ", " << red_pieces[j].y;
		// 	cout << endl;  
		// }
		// for(int j=0; j<black_pieces.size(); j++){
		// 	cout << "Black piece at " << black_pieces[j].positionOnBoard; 
		// 	cout << "( " << black_pieces[j].x << ", " << black_pieces[j].y;
		// 	cout << endl;  
		// }
	}


	stdBoard b;
	bool waitForOpponent;
	Checker * selected_piece;
	bool clickedBefore;
	bool piece_selected;

	vector<sf::RectangleShape> red_tiles;
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
