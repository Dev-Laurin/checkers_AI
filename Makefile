all: main.cpp
	g++ -std=c++14 main.cpp random_checkers.cpp -lsfml-graphics -lsfml-window -lsfml-system
	./a.out 