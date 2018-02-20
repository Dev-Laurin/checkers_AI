all: guiMain.cpp
	g++ -std=c++14 guiMain.cpp gui.cpp -lsfml-graphics -lsfml-window -lsfml-system -pthread
	./a.out 
testBoard: Test/testBoard.cpp old_movegenerator.cpp
	g++ -std=c++14 Test/testBoard.cpp old_movegenerator.cpp
	./a.out
testNN: testNN.cpp
	g++ -std=c++14 testNN.cpp -o nnTest.o
	./nnTest.o 
train: training.cpp
	g++ -std=c++14 training.cpp -o train.o
	./train.o 