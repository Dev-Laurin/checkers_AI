all: guiMain.cpp
	g++ -std=c++14 guiMain.cpp -lsfml-graphics -lsfml-window -lsfml-system
	./a.out 
test: Test/testBoard.cpp old_checkers/old_movegenerator.cpp
	g++ -std=c++14 Test/testBoard.cpp old_checkers/old_movegenerator.cpp
	./a.out
