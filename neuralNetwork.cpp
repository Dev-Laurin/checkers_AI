#include "neuralNetwork.h"


//declare random number generator
std::mt19937_64 gen(time(0));
//Initialize the database

int gameBoards = 0;
int boardEvals = 0;
int innerNodes = 0;
int leafNodes = 0;



//Quick and dirty monty carlo calc.
stdBoard AIPlayer::getMontyMove(stdBoard & board, bool side) {
  //0: Initial, 1: Red, 2: Black
  stdBoard moveList[MAXMOVES];
  int numMoves;
  vector<MontyBoard> moves;
  numMoves = board.genMoves(moveList,side);
return board;
}



stdBoard AIPlayer::getMove(stdBoard & board, bool side) {

  //place to store the possible moves
    stdBoard moveList[MAXMOVES];
    int selectMove;
    std::vector<std::thread> threads;

  //For timing
    std::time(&timeStart);
    struct tm * timeInfo;
    struct tm * timeInfoHalf;
    timeInfo = localtime(&timeStart);
    timeInfo->tm_sec += MOVETIME;
    timeLimit = mktime(timeInfo);
    timeInfoHalf->tm_sec += MOVETIME/2;
    timeHalf = mktime(timeInfoHalf);
    timeExceeded = false;

  //For keeping track of trimming stat
    trimTotal = 0;
    depthReached = 2; //start at 2.

  //Flip the board if needed (red vs black)
    if(side) {
        board = board.flip();
    }
  //Get the possible board moves

    int moves = board.genMoves(moveList,0);
    numMoves = moves; //save for printing stats
    numBoards = 0;
    numBoardEvals = 0;
    if (moves == 0) {
        return stdBoard(0,0,0,0);
    } else { //For report.
//      ++innerNodes;
    }
  //If there is more than one move, check cache and place in
    //cache if there is a miss (speeds up most used boards)
    if (moves > 1) {
      for(int i = 0; i < moves; ++i) {
        if (boardMem.count(moveList[i])) {
//            ++cacheHit;
        } else {
            boardMem.emplace(moveList[i],calculateBoard(moveList[i]));
//            ++cacheMiss;
        }
      }
      //Find out how board scores.
      //Based on Chinook's database
/*      bool endValid = board.endGameCheck();
      if (endValid && false) {
        cout << "Valid board for endgame" <<endl;
        //TIE     0
        //WIN     1
        //LOSS    2
        //UNKNOWN 3
        int score = DBLookup(board);
        if (score==1) {
          cout << "Winning board" << endl;
        } else if (score == 2) {
          cout << "Losing board" << endl;
        } else if (score == 0) {
          cout << "Tie" << endl;
        } else {
        cout << "Not a valid board for endgame" << endl;
        }
      }
*/
      //iterative deepening search
      selectMove = 0;
      sNN moveVal[MAXMOVES];

      //If more than half of the time has been consumed, don't try to go deeper.
      while(time(0) < timeHalf && depthReached <= searchDepth) {
//      while(time(0) < timeLimit && depthReached <= searchDepth) {
        //Sort the boards so we expand the best ones first
        std::sort(moveList, moveList+moves,
                  [&](stdBoard a, stdBoard b) {
                    return (boardMem.at(a) > boardMem.at(b));
                  });

        //expand the first move
        for (int i = 0; i < moves; ++i) {
            moveVal[i] = beta(moveList[i], 0, depthReached, LOWEST, HIGHEST);
            //cout << "moveVal: " << moveVal << " tempMove: " << tempMove << endl;
            //cout << "Highest: " << HIGHEST << " Lowest: " << LOWEST << endl;
        }
        if(!timeExceeded) {
          ++depthReached;
        }
      }
      for(int i = 1; i < moves; ++i) {
          if(moveVal[i] > moveVal[selectMove]) {
            selectMove = i;
          }
      }
    } else { // Only one move, no need to calc!
      selectMove = 0;
      depthReached = 0;
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


sNN AIPlayer::alpha(stdBoard & board, int depth, int maxDepth, sNN a, sNN b) {
  stdBoard moveList[MAXMOVES];
  int moves;
  moves = board.genMoves(moveList,0);
  if (moves) {
//      ++innerNodes;
    for(int i = 0; i < moves; ++i) {
//      ++leafNodes;
      if (boardMem.count(moveList[i])) {
//          ++cacheHit;
      } else {
        /*
          bool endValid = board.endGameCheck();
          int v = DBLookup(board);
          */
          boardMem.emplace(moveList[i],calculateBoard(moveList[i]));
//          ++cacheMiss;
      }
    }
    std::sort(moveList, moveList+moves,
          [&](stdBoard a, stdBoard b) {
            return (boardMem.at(a) > boardMem.at(b));
          });
    if (time(nullptr) > timeLimit) {
      timeExceeded = true;
      return boardMem.at(moveList[0]);
    }
    if (depth >= maxDepth) { //Out of depth, return!)
      return boardMem.at(moveList[0]);
    }
    sNN rVal = beta(moveList[0],depth+1, maxDepth, a, b);
    for (int i = 1;i < moves ; ++i) {
      a = std::max(rVal, a);
      if (b <= a) {  //I can force a better move than the worse the opponent can, so he won't pick this.
//        ++trimTotal;
        return rVal;
      }
      rVal = max(rVal,beta(moveList[i],depth+1, maxDepth, a, b));
    }
    //Take the highest value move.
    return rVal;
  } else {
//    ++leafNodes;
    //No moves available.  We lose!
    //We don't like this, obviously.
    return LOWEST + depth; //less depth means we are losing quicker, avoid.
  }

}


sNN AIPlayer::beta(stdBoard & board, int depth, int maxDepth, sNN a, sNN b) {

 //Get possible moves with current board
  stdBoard moveList[MAXMOVES];
  int moveCount;
  moveCount = board.genMoves(moveList,1);

  //if there are not 0 moves
  if (moveCount) {
//      ++innerNodes;

      //get the maximum value of this board (alpha)
//    numBoards++;
      sNN rVal = alpha(moveList[0],depth, maxDepth, a, b);
      //get the minimum of alpha's board value and the highest possible value
      for (int i = 1;i<moveCount;++i) {
        b = std::min(rVal, b);
        if (b <= a) { //I can force a worse move than the best found, stop looking.
//        ++trimTotal;
          return rVal;
        }
//        numBoards++;
        //set the value to the minimum between last value and next alpha value
        rVal = min(rVal,alpha(moveList[i],depth, maxDepth, a, b));
      }
      return rVal;
  } else {
//    ++leafNodes;
    //No moves available.  We win!
    //We like this, obviously.
    return HIGHEST-depth; //less depth means it is losing quicker, prefer.
  }
}

double AIPlayer::clmp(double x, double a, double b)
{
    return x < a ? a : (x > b ? b : x);
}
void AIPlayer::prntStats() {
  cout << "Cache Effectiveness: " << cacheHit << "/" << cacheHit+cacheMiss;
  cout << " Cache size: " << boardMem.size() << endl;
  cout << "Pruning Trims: " << trimTotal << " Depth Reached: " << depthReached << endl;


  cout << "Boards expanded per move: " << (numBoardEvals-numMoves)/numMoves << endl;
  cout << "Board Evaluation Functions called: " << numBoardEvals << endl;
  cout << "Inner nodes: " << innerNodes << endl;
  cout << "Leaf nodes: " << numBoardEvals << endl;
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

NN::NN() {
  std::uniform_real_distribution<double> dis(-0.2,0.2);
  std::uniform_real_distribution<double> kingDis(0.0, 1.0);
}


//Given a board, calculate the output of the NN
sNN NN::calculateBoard(stdBoard & board){
//  ++boardEvals;
//  numBoardEvals++;
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
}

//Puts board input into nodes vector in user specified index
void NN::getBoardInput(stdBoard & board){
  for(int i=0; i<32; ++i){
          nodes[0][i] =
              ((double)board.pieces[0][i] - (double)board.pieces[1][i]) *
              (1 + kingVal*((double)board.pieces[2][i]));
  }
}


void NN2::getBoardInput(stdBoard & board) {
  for(int i=0; i<32; ++i){
          nodes[0][i] = board.pieces[0][i];
          nodes[0][i+32] = board.pieces[1][i];
          nodes[0][i+64] = board.pieces[2][i] && board.pieces[0][i];
          nodes[0][i+96] = board.pieces[2][i] && board.pieces[1][i];
  }
}

double NN::boardCount(stdBoard & board) {
  double count =
    (double)board.pieces[0].count() - (double)board.pieces[1].count() +
    ((double)(board.pieces[0] & board.pieces[2]).count() -
     (double)(board.pieces[1] & board.pieces[2]).count()) * kingVal;
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
  boardMem.clear(); //Becoming a new network, previous cached values invalid.

  //Randomize king, piececount, and monty values
  std::normal_distribution<double> kingDis(0.0,0.1);
  kingVal = clmp(kingVal + kingDis(gen),0.0,1.0);
  pieceWeight = clmp(pieceWeight + kingDis(gen),0.0,2.0);
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
      network[i][j] = clmp(network[i][j] + sigmas[i][j] * rand,-1,1);
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
    //cout << "Hehe, I'm Random!" << endl;
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
  std::ofstream ofs(filename, std::ios::binary);
  if(!ofs){
    cout << "Error opening file for NN saving: " << filename;
    cout << endl;
    return -1;
  }
  boost::archive::binary_oarchive oa(ofs);
  oa << *this;
  ofs.close();

  return 0; //successful
}

int NN::saveToTextFile(string filename){
  //Boost file saving
  std::ofstream ofs(filename, std::ios::binary);
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
  ifstream file(filename, std::ios::binary);
  if(!file){
    cout << "Error opening NN file: " << filename << endl;
    return -1;
  }
  boost::archive::binary_iarchive ia(file);
  ia >> *this;
  file.close();

  return 0;
}

int saveToFile(const NN & nn, string filename){
    //Boost file saving
  std::ofstream ofs(filename, std::ios::binary);
  if(!ofs){
    cout << "Error opening file for NN saving: " << filename;
    cout << endl;
    return -1;
  }
  boost::archive::binary_oarchive oa(ofs);
  oa << nn;
  ofs.close();

  return 0; //successful
}

int loadFromFile(NN& nn, string filename){
  ifstream file(filename, std::ios::binary);
  if(!file){
    cout << "Error opening NN file: " << filename << endl;
    return -1;
  }
  boost::archive::binary_iarchive ia(file);
  ia >> nn;
  file.close();

  return 0;
}
