all: guiMain.cpp
	g++ -std=c++17 guiMain.cpp gui.cpp -lsfml-graphics -lsfml-window -lsfml-system -pthread
	./a.out 
testBoard: Test/testBoard.cpp old_movegenerator.cpp
	g++ -std=c++17 Test/testBoard.cpp old_movegenerator.cpp
	./a.out
testNN: testNN.cpp neuralNetwork.cpp
	g++ -std=c++17 testNN.cpp neuralNetwork.cpp -O2 -lboost_system -lboost_filesystem -lboost_serialization -o nnTest.o
	./nnTest.o 
train: training.cpp
	g++ -std=c++17 training.cpp -o train.o
	./train.o 
playgame: playgame.cpp
	g++ -std=c++17 playgame.cpp neuralNetwork.cpp -o playgame.o
	./playgame.o 
blondie: blondieExperiment1SpringBreak.cpp
	g++ -std=c++17 blondieExperiment1SpringBreak.cpp neuralNetwork.cpp playgame.cpp -lboost_system -lboost_filesystem -o blondie.o
	./blondie.o 