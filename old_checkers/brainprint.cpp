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
		Brain[0].Print("Brain0.txt");
		Brain[0].Save("Brain0.NN");
		Brain[0].Evolve();
		Brain[0].Print("Brain1.txt");
		Brain[0].Save("Brain1.NN");
		
		cout << "Finished saving brains" << endl;
}
