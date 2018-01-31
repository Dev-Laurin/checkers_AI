all: guiMain.cpp
	g++ -std=c++14 guiMain.cpp -lsfml-graphics -lsfml-window -lsfml-system
	./a.out 
test: testBoard.cpp old_movegenerator.cpp
	g++ -std=c++14 testBoard.cpp
	./a.out
