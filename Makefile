all: main.cpp
	g++ -std=c++14 main.cpp -lsfml-graphics -lsfml-window -lsfml-system
	./a.out 
test: testBoard.cpp
	g++ -std=c++14 testBoard.cpp
	./a.out