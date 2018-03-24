#include "neuralNetwork.h"

//declare random number generator
std::mt19937_64 gen(time(0));


//Searches until it runs out of time.
stdBoard AIPlayer::IntDeepSearch(stdBoard & board, bool side) {
  stdBoard moveList[MAXMOVES];
  int selectMove = 0;
  std::time(&timeStart);
  if(side) {
    board = board.flip();
  }
  //Work first layer.
  int moves = board.genMoves(moveList,0);
  if (moves == 0) {
      return stdBoard(0,0,0,0);
  }
  //Score the list.
  for(int i = 0; i < moves; ++i) {
    moveList[i].score = calculateBoard(moveList[i]);
  }
  std::sort(moveList,moveList+moves);
  selectMove = moves -1;  //It's sorted, the best move is the last one.
  //Find opponent moves
  stdBoard oppMoveList[moves][MAXMOVES];
  for(int i = moves-1; i >= 0; --i) {
    int moveCount = moveList[i].genMoves(oppMoveList[i],1);
    if (moveCount == 0) { //Enemy has no moves, win.
      selectMove = i;
      i = -1;
    }
  }



  if(side) {
    board = board.flip();
    moveList[selectMove] = moveList[selectMove].flip();
  }
  return moveList[selectMove];
}


stdBoard AIPlayer::getMove(stdBoard & board, bool side) {
    stdBoard moveList[MAXMOVES];
    int selectMove;
    std::time(&timeStart);
    struct tm * timeInfo;
    timeInfo = localtime(&timeStart);
    timeInfo->tm_sec += MOVETIME;
    timeLimit = mktime(timeInfo);
    timeExceeded = false;
    trimTotal = 0;
    if(side) {
        board = board.flip();
    }
    int moves = board.genMoves(moveList,0);
    if (moves == 0) {
        return stdBoard(0,0,0,0);
    }
    if (moves > 1) {
      for(int i = 0; i < moves; ++i) {
        if (boardMem.count(moveList[i])) {
          moveList[i].score = boardMem.at(moveList[i]);
          ++cacheHit;
        } else {
          moveList[i].score = calculateBoard(moveList[i]);
          boardMem.insert({moveList[i],moveList[i].score});
          ++cacheMiss;
        }
      }
      std::sort(moveList,moveList+moves);
      selectMove = moves - 1;
      sNN moveVal = beta(moveList[moves-1],searchDepth, LOWEST*2, HIGHEST*2);
      sNN tempMove;
      for (int i = moves-2; i >= 0; --i) {
          tempMove = beta(moveList[i],searchDepth, LOWEST * 2, HIGHEST * 2);
          //cout << "moveVal: " << moveVal << " tempMove: " << tempMove << endl;
          //cout << "Highest: " << HIGHEST << " Lowest: " << LOWEST << endl;
          if (tempMove > moveVal) {
              //better move found
              moveVal = tempMove;
              selectMove = i;
          }
      }
    } else { // Only one move, no need to calc!
      selectMove = 0;
    }
    if (side) {
        board = board.flip();
        moveList[selectMove] = moveList[selectMove].flip();
    }
    return moveList[selectMove];
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


sNN AIPlayer::alpha(stdBoard & board, int depth, sNN a, sNN b) {
  stdBoard moveList[MAXMOVES];
  int moves;
  moves = board.genMoves(moveList,0);
  if (moves) {
    for(int i = 0; i < moves; ++i) {
      if (boardMem.count(moveList[i])) {
          moveList[i].score = boardMem.at(moveList[i]);
          ++cacheHit;
      } else {
          moveList[i].score = calculateBoard(moveList[i]);
          boardMem.insert({moveList[i],moveList[i].score});
          ++cacheMiss;
      }
    }
    std::sort(moveList,moveList+moves);
    if (time(nullptr) > timeLimit) {
      timeExceeded = true;
      return moveList[moves-1].score;
    }
    if (depth <= 1) { //Out of depth, return!)
      return moveList[moves-1].score;
    }
    sNN rVal = beta(moveList[moves-1],depth-1, a, b);
    for (int i = moves - 2;i >= 0; --i) {
      a = std::max(rVal, a);
      if (b <= a) {  //I can force a better move than the worse the opponent can, so he won't pick this.
        ++trimTotal;
        return rVal;
      }
      rVal = max(rVal,beta(moveList[i],depth-1, a, b));
    }
    //Take the highest value move.
    return rVal;
  } else {
    //No moves available.  We lose!
    //We don't like this, obviously.
    return LOWEST - depth; //more depth means we are losing quicker, avoid.
  }

}


sNN AIPlayer::beta(stdBoard & board, int depth, sNN a, sNN b) {
  stdBoard moveList[MAXMOVES];
  int moveCount;
  moveCount = board.genMoves(moveList,1);
  if (moveCount) {
      sNN rVal = alpha(moveList[0],depth, a, b);
      for (int i = 1;i<moveCount;++i) {
        b = std::min(rVal, b);
        if (b <= a) { //I can force a worse move than the best found, stop looking.
          ++trimTotal;
          return rVal;
        }
        rVal = min(rVal,alpha(moveList[i],depth, a, b));
      }
      return rVal;
  } else {
    //No moves available.  We win!
    //We like this, obviously.
    return HIGHEST+depth; //more depth means it is losing quicker, prefer.
  }
}

double AIPlayer::clmp(double x, double a, double b)
{
    return x < a ? a : (x > b ? b : x);
}
void AIPlayer::prntStats() {
  cout << "Cache Effectiveness: " << cacheHit << "/" << cacheHit+cacheMiss;
  cout << " Cache size: " << boardMem.size() << endl;
  cout << "Pruning Trims: " << trimTotal << endl;
}


/*
stdBoard AIPlayer::alphabeta(stdBoard board, unsigned int side) {

    unsigned int movesA, movesB;
    sNN moveValA[MAXMOVES];
    sNN moveValB[MAXMOVES];

    stdBoard Br[MAXMOVES];
    unsigned int i = 0;
    unsigned int numMoves;
    int boardsGenerated[10] = {0};
    unsigned int maxMove = 0;
    sNN maxMoveVal = 0;

    numMoves = board.genMoves(Br,side);
    if (numMoves == 0) {//We have lost!  Return a blank board to signal
    return stdboard(0,0,0,0);
    }
    for(i = 0;i<numMoves;++i) {
        maxMove = max()
    }
    return board;
}
*/

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
  std::uniform_real_distribution<double> kingDis(0.0, 1.0);
  kingVal = kingDis(gen);
  pieceWeight = kingDis(gen);
  montyWeight = kingDis(gen);

  //Save the beg sigmas of the weights
  sigmas.resize(network.size());
  for(size_t i=0; i<network.size(); ++i){
    sigmas[i].resize(network[i].size(), sigma);
  }
}

//Given a board, calculate the output of the NN
sNN NN::calculateBoard(stdBoard & board){
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
  return nodes[nodes.size()-1][0] + boardCount(board)* pieceWeight;
  //nodes[nodes.size()-1][0] + boardCount(board)* pieceWeight;
}

//Puts board input into nodes vector in user specified index
void NN::getBoardInput(stdBoard & board){
  for(int i=0; i<32; ++i){
          nodes[0][i] =
              (double)board.pieces[0][i] - (double)board.pieces[1][i] +
              ((double)board.pieces[2][i] - (double)board.pieces[3][i]) * kingVal;
  }
}


void NN2::getBoardInput(stdBoard & board) {
  for(int i=0; i<32; ++i){
          nodes[0][i] = board.pieces[0][i];
          nodes[0][i+32] = board.pieces[1][i];
          nodes[0][i+64] = board.pieces[2][i];
          nodes[0][i+96] = board.pieces[3][i];
  }
}

double NN::boardCount(stdBoard & board) {
  double count =
    (double)board.pieces[0].count() - (double)board.pieces[1].count() +
    ((double)board.pieces[2].count() - (double)board.pieces[3].count()) *
    kingVal;
  return count;
}

//Given a number, calculate a sigmoid function output
void AIPlayer::sigmoid(double & num){
  num = num/(.16667+std::abs(num));
}

//generate offspring, randomize this NN
  //(assuming this is a copy of the original)
void NN::becomeOffspring(){

  //change generation #
  ++generation;

  //Randomize king, piececount, and monty values
  std::normal_distribution<double> kingDis(0.0,0.1);
  kingVal = clmp(kingVal + kingDis(gen),0.0,1.0);
  pieceWeight = clmp(pieceWeight + kingDis(gen),0.0,1.0);
  montyWeight = clmp(montyWeight + kingDis(gen),0.0,1.0);
  //randomize the sigmas
  //(mean, standard deviation)
  //Our numbers will generate bet -1 and 1
  std::normal_distribution<double> nDis(0.0, 1.0);

 //get the total weights in this NN
  double totalWeights = 0;
  for(unsigned int i=0; i<network.size(); ++i){
    totalWeights+=network[i].size();
  }

  //Randomize the weights to become a child
  for(size_t i=0; i<sigmas.size(); ++i){
    for(size_t j=0; j<sigmas[i].size(); ++j){

      //The normal distribution returns numbers outside
        //our range, keep getting a rand number until
        //we get one we can use in our range 0-1
      double rand = nDis(gen);
      while(rand<-1.0 or rand>1.0){
        rand = nDis(gen);
      }

      //newSig = oldsig*e^(1/sqrt(2*sqrt(n)) * random)
      double tau = (1.0/sqrt(2.0*sqrt(totalWeights)));
     // cout << "computation: tau: " << tau << endl;
     // cout << "rand: " << rand << endl;
      //Compute our new sigma
      sigmas[i][j] = sigmas[i][j]*exp(tau*rand);

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
  if(lhs.network.size()!=rhs.network.size()) {
    return true;
  }

  for(unsigned int i=0; i<lhs.network.size(); ++i){

    if(lhs.network[i].size()!=rhs.network[i].size())
      return true;


    for(unsigned int j=0; j<lhs.network[i].size(); ++j){
      if(lhs.network[i][j]!=rhs.network[i][j])
        return true;
    }
  }

  //Check the sigmas
  if(lhs.sigmas.size()!=rhs.sigmas.size())
    return true;

  for(unsigned int i=0; i<lhs.sigmas.size(); ++i){

    if(lhs.sigmas[i].size()!=rhs.sigmas[i].size())
      return true;


    for(unsigned int j=0; j<lhs.sigmas[i].size(); ++j){
      if(lhs.sigmas[i][j]!=rhs.sigmas[i][j])
        return true;
    }
  }

  //family name
  if(lhs.familyName!=rhs.familyName)
    return true;

  return false;
}

bool operator==(const NN & lhs, const NN & rhs){
  return !(lhs!=rhs);
}

//check if NN2 is the same or not
bool operator!=(const NN2 & lhs, const NN2 & rhs){

  //Check the weights
  if(lhs.network.size()!=rhs.network.size()) {
    return true;
  }

  for(unsigned int i=0; i<lhs.network.size(); ++i){

    if(lhs.network[i].size()!=rhs.network[i].size())
      return true;


    for(unsigned int j=0; j<lhs.network[i].size(); ++j){
      if(lhs.network[i][j]!=rhs.network[i][j])
        return true;
    }
  }

  //Check the sigmas
  if(lhs.sigmas.size()!=rhs.sigmas.size())
    return true;

  for(unsigned int i=0; i<lhs.sigmas.size(); ++i){

    if(lhs.sigmas[i].size()!=rhs.sigmas[i].size())
      return true;


    for(unsigned int j=0; j<lhs.sigmas[i].size(); ++j){
      if(lhs.sigmas[i][j]!=rhs.sigmas[i][j])
        return true;
    }
  }

  //family name
  if(lhs.familyName!=rhs.familyName)
    return true;

  return false;
}

bool operator==(const NN2 & lhs, const NN2 & rhs){
  return !(lhs!=rhs);
}

stdBoard RandomPlayer::getMove(stdBoard & board, bool side) {
    stdBoard possibleBoards[MAXMOVES];
    if(side) {
      board = board.flip();
    }
    int moveCount = board.genMoves(possibleBoards,0);
    if (moveCount == 0) {
        return stdBoard(0,0,0,0);
    }
    int moveSelect = int(distro(gen)*moveCount);

    if(side) {
      board = board.flip();
      possibleBoards[moveSelect] = possibleBoards[moveSelect].flip();
    }
    cout << "Hehe, I'm Random!" << endl;
    return possibleBoards[moveSelect];
  }

// //Global Save to File Functions
// //Save this NN to a file
// int saveToFile(const NN & nn, string filename){

//   //Boost file saving
//   std::ofstream ofs(filename); 
//   if(!ofs){
//     cout << "Error opening file for NN saving: " << filename;
//     cout << endl;  
//     return -1; 
//   }
//   boost::archive::text_oarchive oa(ofs);  
//   oa << nn; 
//   ofs.close(); 

//   return 0; //successful
// }

// int loadFromFile(NN & nn, string filename){

//   ifstream file(filename); 
//   if(!file){
//     cout << "Error opening NN file: " << filename << endl;
//     return -1; 
//   }
//   boost::archive::text_iarchive ia(file);
//   ia >> nn; 
//   file.close(); 

//   return 0;
// }

int NN::saveToFile(string filename){
  //Boost file saving
  std::ofstream ofs(filename); 
  if(!ofs){
    cout << "Error opening file for NN saving: " << filename;
    cout << endl;  
    return -1; 
  }
  boost::archive::text_oarchive oa(ofs);  
  oa << *this; 
  ofs.close(); 

  return 0; //successful
}

int NN::loadFromFile(string filename){
  ifstream file(filename); 
  if(!file){
    cout << "Error opening NN file: " << filename << endl;
    return -1; 
  }
  boost::archive::text_iarchive ia(file);
  ia >> *this; 
  file.close(); 

  return 0; 
}

int saveToFile(const NN & nn, string filename){
    //Boost file saving
  std::ofstream ofs(filename); 
  if(!ofs){
    cout << "Error opening file for NN saving: " << filename;
    cout << endl;  
    return -1; 
  }
  boost::archive::text_oarchive oa(ofs);  
  oa << nn; 
  ofs.close(); 

  return 0; //successful
}

int loadFromFile(NN& nn, string filename){
  ifstream file(filename); 
  if(!file){
    cout << "Error opening NN file: " << filename << endl;
    return -1; 
  }
  boost::archive::text_iarchive ia(file);
  ia >> nn; 
  file.close(); 

  return 0; 
}