/*
NeuralNetTest.cpp
Jason Bright
Neural Net Test program
20160215

*/
/*
gen wrong moves/jumps
Bad iterator
Switch maximum

*/


#include "neuralnet.h"
#include "movegenerator.h"

using std::cout;
using std::endl;

#define MAXGAMEMOVES 200
#define NUMBRAINS 50
#define WINVALUE 12
#define LOSEVALUE -12

int main(int argc, char * argv[]) {
		NeuralNet Brain[NUMBRAINS];
		for (int i = 0; i < NUMBRAINS; ++i) {
			std::stringstream fileName;
			fileName << argv[1] << "/brain" << i << ".NN";
			Brain[i].Load(fileName.str());
		}
		for(int i = 0;i < NUMBRAINS;++i) {
			std::stringstream fileName;
			fileName << argv[1] << "/brain" << i << ".NN";
			Brain[i].Save(fileName.str());
		}
		cout << "Finished saving brains" << endl;
}
