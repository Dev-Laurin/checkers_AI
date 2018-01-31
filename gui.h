#ifndef GUI_INCLUDED_H
#define GUI_INCLUDED_H

int findCheckerMove(string newBoard, string oldBoard){
	for(int i=0; i<newBoard.size(); ++i){
		if(newBoard[i]!=oldBoard[i] and
			newBoard[i]!=' '){
			//index of where checker jumped/moved to
			return i; 
		}
	}
}

int findOldCheckerSpace(string newBoard, string oldBoard){
	for(int i=0; i<newBoard.size(); ++i){
		if(newBoard[i]!=oldBoard[i] and newBoard[i]==' '){
			//index of where checker jumped from 
			return i; 
		}
	}
}

//used alot
float tile_width = 62.5; 

class Checker{
public: 
	Checker(const float R, const int pos, const sf::Color col,
		sf::RenderWindow * win){
		radius = R; 
		positionOnBoard = pos; 
		color = col; 
		window = win; 

        //make the checker for drawing
        piece.setRadius(radius);
        piece.setFillColor(color);

        //find x y coordinates of position on board
		updateBoardPosition(positionOnBoard);  
	}

	void updateBoardPosition(const int posBoard){
		positionOnBoard = posBoard;
		convertBoardIndexIntoXYPositions(posBoard);
		piece.setPosition(x,y); 
	}

	void convertBoardIndexIntoXYPositions(const int positionOnBoard){
		cout << "CONVERTING FROM I TO X Y " << endl; 
		cout << "Board Index given: " << positionOnBoard << endl;
		int tileCount = 0; 
        for(int row=0; row<8; ++row){
        	for(int col=0; col<4; ++col){
        		if(tileCount==positionOnBoard){
        			y = row * tile_width;
        			cout << "Row" << endl; 
        			cout << row << endl; 
        			if(row%2==0){ //if even row
        				x = col*tile_width*2 + tile_width; 
        				cout << "Even x = " << x << endl; 
        			} 
        			else{
        				x = col*tile_width*2;
        				cout << "Odd x = " << x << endl; 
        			}
        		}
        		tileCount++; 
        	}
        }

        cout << "X before = " << x << endl; 
        cout << "Y before = " << y << endl; 
        cout << "Tile width = " << tile_width << endl; 
        cout << "Radius = " << radius << endl; 
		x+=((tile_width-radius)/4); 
		y+=((tile_width-radius)/4); 
		cout << "X = " << x << endl; 
		cout << "Y = " << y << endl; 
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
		for(int i=20; i<b.str().size(); ++i){
			Checker r(22.f, i, sf::Color(139,0,0,255), &window);
			red_pieces.push_back(r);
			Checker bl(22.f, i-20, sf::Color::Black, &window);
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

	void run(){
		
			//Draw 
		while(window.isOpen())
		{
			sf::Event event; 
			while (window.pollEvent(event)){
				if(event.type == sf::Event::Closed)
					window.close(); 
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					sf::Vector2i position = sf::Mouse::getPosition(window);
					error.setString(""); //reset error message

					//move checker?
					if(piece_selected){

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
							//cout << "Pos on board: " << selected_piece->positionOnBoard << endl;
							if(possibleBoards[i].flipBoard().at(selected_piece->positionOnBoard) == ' '){
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
								//cout << "Clicked Index: " << clickedIndex << endl;
								//cout << "ValidPosition[i]" << validPositions[i] << endl;
								//is a valid position, move the piece
								selected_piece->updateBoardPosition(clickedIndex);
								valid=true; 
								//update board
								b.updateBoard(possibleBoards[indices[i]].str()); 
								reDrawBoard(b.flipBoard()); 
								cout << "after redraw " << endl; 
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
					if(piece_selected) 
						selected_piece->piece.setFillColor(sf::Color(139,0,0,255));

					//See which red piece was selected
					for(int i=0; i<red_pieces.size(); ++i){
						int yArea = red_pieces[i].y + 
						red_pieces[i].radius*2;
						int xArea = red_pieces[i].x + 
						red_pieces[i].radius*2; 

						if(red_pieces[i].y < position.y and 
							position.y < yArea and 
							red_pieces[i].x < position.x and
							position.x < xArea){
								//this piece was clicked change its color
							selected_piece = &red_pieces[i]; 
							red_pieces[i].piece.setFillColor(sf::Color::White); 
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
			for(int i=0; i<red_pieces.size(); ++i){
				//window.draw(black_pieces[i]); 
				red_pieces[i].draw(); 
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
		cout << "Updating pieces" << endl; 
		cout << newBoard << endl; 
		//start over 
		black_pieces.clear(); 
		red_pieces.clear(); 
		for(int i=0; i<newBoard.size(); ++i){
			if(newBoard[i]=='b'){
				//draw a black checker there
				Checker n(22.f, i, sf::Color::Black, &window);
				black_pieces.push_back(n); 

				//black_pieces[i].updateBoardPosition(i);

			}
			else if(newBoard[i]=='r'){
				Checker n(22.f, i, sf::Color(139,0,0,255), &window);
				red_pieces.push_back(n); 
				//red_pieces[i].radius = 22.f;
				//red_pieces[i].updateBoardPosition(i); 
				

			}
			else if(newBoard[i]=='B'){
				//Checker n(22.f, i, sf::Color::Blue, &window);
				//black_pieces[i] = n;
			}
			else if(newBoard[i]=='R'){
				//Checker n(22.f, i, sf::Color::Green, &window);
				//red_pieces[i] = n;				
			}
			else{
				//blank space
			}
		}
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