#include "neuralNetwork.h"

stdBoard AIPlayer::getMove(stdBoard & board, bool side) {
      stdBoard possibleBoards[32];
      if(side) {
        board = board.flip();
      }
      int moves = board.genMoves(possibleBoards,0);
      int selectMove = 0;
      sNN moveVal = beta(possibleBoards[0],7);
      sNN tempMove;
      for (int i = 1;i<moves;++i) {
                tempMove = beta(possibleBoards[i],7);
                //cout << "moveVal: " << moveVal << " tempMove: " << tempMove << endl;
                //cout << "Highest: " << HIGHEST << " Lowest: " << LOWEST << endl;
                if (tempMove > moveVal) {
                    //better move found
                    moveVal = tempMove;
                    selectMove = i;
                }
      }
      if (side) {
          possibleBoards[selectMove].flip();
      }
      return possibleBoards[selectMove];
}

/*
  //Alpha is the MAX value selector.
  //Always assumed to be black.
  //Flip the board to computer run red.
  //Because a neural network may be good at selecting maximums
  //but not minimums.

  //Beta is the MINIMUM value selector.
  //Always assumed to be red.
*/
sNN AIPlayer::alpha(stdBoard & board, int depth) {
  stdBoard moveList[MAXMOVES];
  int moveCount;
  moveCount = board.genMoves(moveList,0);
  if (moveCount) {
      sNN rVal=LOWEST;
      if (depth>1) {
        for (int i = 0;i<moveCount;++i) {
          rVal = max(rVal,beta(moveList[i],depth-1));
        }
      } else {
        for (int i = 0;i<moveCount;++i) {
            rVal = max(rVal,calculateBoard(moveList[i]));
        }
      }
      //Take the highest value move.
      return rVal;
  } else {
    //No moves available.  We lose!
    //We don't like this, obviously.
    return LOWEST;
  }

}


sNN AIPlayer::beta(stdBoard & board, int depth) {
  stdBoard moveList[MAXMOVES];
  int moveCount;
  moveCount = board.genMoves(moveList,1);
  if (moveCount) {
      sNN rVal=HIGHEST;
      for (int i = 0;i<moveCount;++i) {
        rVal = min(rVal,alpha(moveList[i],depth-1));
      }
      return rVal;
  } else {
    //No moves available.  We win!
    //We like this, obviously.
    return HIGHEST;
  }
}

sNN AIPlayer::alphabeta(stdBoard board, unsigned int side) {
  stdBoard boardA[MAXMOVES];
  stdBoard boardB[MAXMOVES];
  unsigned int movesA, movesB;
  sNN moveValA[MAXMOVES];
  sNN moveValB[MAXMOVES];

  movesA = board.genMoves(boardA,1-side);
  for(unsigned int i=0;i<movesA;++i) {
    movesB = boardA[i].genMoves(boardB,side);
    for(unsigned int j=0;j<movesB;++j) {
      moveValB[j] = calculateBoard(boardB[j]);
    }
    moveValA[i] = *std::max_element(moveValB,moveValB+movesB);
  }
  return *std::min_element(moveValA,moveValA+movesA);
}


//The NN 
//Set weights to random values
NN::NN(std::vector<int>& nS, string familyname)
{
  //Save the family name (to tell apart/organize)
  familyName = familyname;

    nodeSizes = nS;
  //Save the node configuration
  nodes.resize(nodeSizes.size());
  for(unsigned int i=0; i<nodeSizes.size(); ++i){
    nodes[i].resize(nodeSizes[i], 0);
  }
  //Random number generator
  gen.seed(time(0));
  //Each weight is now between -0.2 and 0.2 (see proj3)
    std::uniform_real_distribution<double> dis(-0.2,0.2);

  //create how many layers there will be (4,32,40,10,1)
  network.resize(nodeSizes.size()-1);
  for(unsigned int j=0; j<network.size(); ++j){
          network[j].resize(nodeSizes[j]*nodeSizes[j+1]);
          for(unsigned int k=0; k<network[j].size(); ++k){
              //put random weights in
              network[j][k] = dis(gen);
    }
  }
  //Generate a random king value U(1.0, 3.0)
  std::uniform_real_distribution<double> kingDis(1.0, 3.0);
  kingVal = kingDis(gen);

  //Save the beg sigmas of the weights
  sigmas.resize(network.size());
  for(size_t i=0; i<network.size(); ++i){
    sigmas[i].resize(network[i].size(), sigma);
  }
}

//Given a board, calculate the output of the NN
double NN::calculateBoard(stdBoard & board){
  for(size_t i = 0;i < nodes.size();++i) {
          std::fill(nodes[i].begin(),nodes[i].end(),0.0);
  }
  getBoardInput(board);
  for (unsigned int i = 0; i < network.size(); ++ i) { //Step through network layers
         for (int j = 0; j < nodeSizes[i]; ++j) { //Step through first layer nodes
              for (int k = 0; k < nodeSizes[i+1]; ++k) {//Step through second layer nodes
                  nodes[i+1][k] += nodes[i][j] * network[i][j*nodeSizes[i+1] + k];
              }
          }
          //layer done, apply sigmoid

          for (unsigned int j = 0; j < nodes[i+1].size(); ++j) {
              sigmoid(nodes[i+1][j]);
          }
  }
  //Add other modifiers
  return nodes[nodes.size()-1][0];
  //nodes[nodes.size()-1][0] + boardCount(board)* pieceWeight;
}

//Puts board input into nodes vector in user specified index
void NN::getBoardInput(stdBoard & board){

  for(int i=0; i<32; ++i){
          nodes[0][i] =
              (int)(board.pieces[0][i] - board.pieces[1][i]) +
              (int)(board.pieces[2][i] - board.pieces[3][i]) * kingVal;
  }
}

double NN::boardCount(stdBoard board) {
    double count =
      (int)(board.pieces[0].count() - board.pieces[1].count()) +
      (int)(board.pieces[2].count() - board.pieces[3].count()) *
      kingVal;
    return count;
  }

//Given a number, calculate a sigmoid function output
void NN::sigmoid(double & num){
  num = num/(1+std::abs(num));
}

//Save this NN to a file
int NN::saveToFile(){

  ofstream file("NeuralNetworkFiles/" + familyName + "_NN_" + to_string(generation) + ".bin", 
    std::ios::binary);

  if(!file){
    cout << "Unable to open file." << endl;
    return -1;
  }

  //Write generation number 
  file.write((char*)&generation, sizeof(generation));

  //write the king value 
  file.write((char*)&kingVal, sizeof(kingVal)); 
 
  //write the weight vector sizes
  int size = network.size(); 
  file.write((char*)&size, sizeof(int)); 

  //write the multiple vector sizes
  for(int i=0; i<network.size(); ++i){
    int size = network[i].size(); 
    file.write((char*)&size, sizeof(int)); 
  }

  //Write weights to file
  for(int i=0; i<network.size(); ++i){
    for(int j=0; j<network[i].size(); ++j){
      file.write((char*)&network[i][j], sizeof(double)); 
    }
  }

  //Write sigmas 
  for(int i=0; i<sigmas.size(); ++i){
    for(int j=0; j<sigmas[i].size(); ++j){
      file.write((char*)&sigmas[i][j], sizeof(double)); 
    }
  }

  file.close();
  return 0; //successful
}

//Load NN from file
int NN::loadFromFile(string filename){
  ifstream file("NeuralNetworkFiles/" + filename + ".bin", std::ios::binary);

  if(!file){
    cout << "Error opening file.";
    cout << " Filename: " << filename << endl;
    return -1;
  }

  //read in the generation number 
  file.read(reinterpret_cast<char *>(&generation), sizeof(generation)); 

  //read in the kingvalue 
  file.read(reinterpret_cast<char *>(&kingVal), sizeof(kingVal));

  //read in the overall size of the weights
  int weightSize; 
  file.read(reinterpret_cast<char *>(&weightSize), sizeof(int)); 

  network.resize(weightSize);
  sigmas.resize(weightSize); 

  //read in size of individual vectors (2D)
  for(int i=0; i<weightSize; ++i){
    int size; 
    file.read(reinterpret_cast<char *>(&size), sizeof(int));
    network[i].resize(size); 
    sigmas[i].resize(size); 
  }

  //read in the actual weights 
  for(int i=0; i<network.size(); ++i){
    file.read((char *)network[i].data(), network[i].size()*sizeof(double)); 
  }

  //read in sigmas 
  for(int i=0; i<sigmas.size(); ++i){
    file.read((char *)sigmas[i].data(), sigmas[i].size()*sizeof(double)); 
  }
  
  file.close();

  //update nodeSizes
  for(int i=0; i<sigmas.size(); ++i){
    nodeSizes[i] = sigmas[i].size(); 
  }
  return 0;
}

//generate offspring, randomize this NN
  //(assuming this is a copy of the original)
void NN::becomeOffspring(){

  //change generation #
  ++generation;

  //Randomize king value
  std::uniform_real_distribution<double> kingDis(-0.1,0.1);
  kingVal = kingVal + kingDis(gen);

  //randomize the sigmas
  //(mean, standard deviation)
  //Our numbers will generate bet -1 and 1 
  std::normal_distribution<double> nDis(0.0, 1.0);

  for(size_t i=0; i<sigmas.size(); ++i){
    for(size_t j=0; j<sigmas[i].size(); ++j){

      //The normal distribution returns numbers outside
        //our range, keep getting a rand number until
        //we get one we can use in our range 0-1
      double rand = nDis(gen);
      while(rand<-1.0 or rand>1.0){
        rand = nDis(gen);
      }

      //newSig = oldsig^(1/sqrt(2*sqrt(n)) * random)

      //Compute our new sigma
      sigmas[i][j] = pow(sigmas[i][j], (1.0/sqrt(2.0*sqrt(sigmas[i].size())))*rand);

      //randomize the weights using new sigmas
      rand = nDis(gen);
      while(rand<-1.0 or rand>1.0){
        rand = nDis(gen);
      }
      //newWeight = oldWeight + newSig * random normal num
      network[i][j] = network[i][j] + sigmas[i][j] * rand;
    }
  }
}

//check if NN is the same or not 
bool operator!=(const NN & lhs, const NN & rhs){

  //Check the weights 
  if(lhs.network.size()!=rhs.network.size())
    return false; 

    for(int i=0; i<lhs.network.size(); ++i){

      if(lhs.network[i].size()!=rhs.network[i].size())
        return false; 

        for(int j=0; j<lhs.network[i].size(); ++j){
          if(lhs.network[i][j]!=rhs.network[i][j])
            return false; 
        }        
    } 

  //Check the sigmas 
  if(lhs.sigmas.size()!=rhs.sigmas.size())
    return false; 

  for(int i=0; i<lhs.sigmas.size(); ++i){

    if(lhs.sigmas[i].size()!=rhs.sigmas[i].size())
      return false; 

      for(int j=0; j<lhs.sigmas[i].size(); ++j){
        if(lhs.sigmas[i][j]!=rhs.sigmas[i][j])
          return false; 
      }        
  } 

  //family name
  if(lhs.familyName!=rhs.familyName)
    return false; 

  //King value 
  if(lhs.kingVal!=rhs.kingVal)
    return false; 

  return true; 
}