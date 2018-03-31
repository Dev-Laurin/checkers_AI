all: guiMain.cpp neuralNetwork.cpp playgame.cpp gui.cpp
	g++ -std=c++17 guiMain.cpp neuralNetwork.cpp playgame.cpp endgame.cpp gui.cpp -lsfml-graphics -lsfml-window -lsfml-system -lboost_system -lboost_filesystem -lboost_serialization -pthread -o gui.o
	./gui.o 
testBoard: testBoard.cpp old_movegenerator.cpp
	g++ -std=c++17 testBoard.cpp old_movegenerator.cpp
	./a.out
testNN: test/testNN.cpp neuralNetwork.cpp
	g++ -std=c++17 test/testNN.cpp neuralNetwork.cpp -O2 -lboost_system -lboost_filesystem -lboost_serialization -o nnTest.o
	./nnTest.o 
train: training.cpp
	g++ -std=c++17 training.cpp -o train.o
	./train.o 
playgame: playgame.cpp
	g++ -std=c++17 playgame.cpp neuralNetwork.cpp -O2 -lboost_system -lboost_filesystem -lboost_serialization -o playgame.o
	./playgame.o 
blondie: blondieExperiment1SpringBreak.cpp
	g++ -std=c++17 blondieExperiment1SpringBreak.cpp neuralNetwork.cpp playgame.cpp -lboost_system -lboost_filesystem -o blondie.o
	./blondie.o 
tournament: tournament.cpp neuralNetwork.cpp
	g++ -std=c++17 tournament.cpp neuralNetwork.cpp -O2 -lboost_system -lboost_filesystem -lboost_serialization -o tournament.o
	./tournament.o 
playtestgame: test/testplaygame.cpp playgame.cpp neuralNetwork.cpp  
	g++ -std=c++17 test/testplaygame.cpp playgame.cpp neuralNetwork.cpp -O2 -lboost_system -lboost_filesystem -lboost_serialization -o testgame.o 
	./testgame.o