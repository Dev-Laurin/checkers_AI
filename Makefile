all: guiMain.cpp
	g++ -std=c++17 guiMain.cpp gui.cpp -lsfml-graphics -lsfml-window -lsfml-system -pthread
	./a.out 
testBoard: Test/testBoard.cpp old_movegenerator.cpp
	g++ -std=c++17 Test/testBoard.cpp old_movegenerator.cpp
	./a.out
testNN: testNN.cpp
	g++ -std=c++17 testNN.cpp -O2 -o nnTest.o
	./nnTest.o 
train: training.cpp
	g++ -std=c++17 training.cpp -o train.o
	./train.o 