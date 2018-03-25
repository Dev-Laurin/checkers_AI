//blondieExperiment1SpringBreak
//To run the first blondie24 experiment over spring break
	//to get errors early rather than later with NN.
//3-12-18

#include <queue>
#include <algorithm>
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
using std::setw;
using std::setfill;
using std::stringstream;

#include <vector>
using std::vector;

#include "neuralNetwork.h"

#include <boost/config.hpp>
#include <boost/filesystem.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/collection_traits.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/queue.hpp>

using namespace boost::filesystem;

#include "playgame.h"


class gameList {
public:
    int population;
    std::queue<int> pl1;
    std::queue<int> pl2;
    vector<int> scores;
    vector<int> wins;
    vector<int> losses;
    vector<int> ties;
    int gameNum=0;
    int tournCount = 0;

//Constructor
    gameList() {
      population = 30;
      scores.resize(30,0);
      wins.resize(30,0);
      losses.resize(30,0);
      ties.resize(30,0);
    }
    gameList(int s) {
      population = s;
      scores.resize(s,0);
      wins.resize(s,0);
      losses.resize(s,0);
      ties.resize(s,0);
    }

//Accessors
    int size() {
        return pl1.size();
    }
    int p1() {
        return pl1.front();
    }
    int p2() {
        return pl2.front();
    }
// Add/remove games
    void addGame(int a, int b) {
        pl1.push(a);
        pl2.push(b);
    }

    void pop() {
        pl1.pop();
        pl2.pop();
    }

//Able to save
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & population;
        ar & pl1;
        ar & pl2;
        ar & scores;
        ar & wins;
        ar & losses;
        ar & ties;
        ar & gameNum;
        ar & tournCount;
    }
};


int main(){
  const int population = 30;
  string tournName = "bigg";
  vector<int> nodes{128, 128, 40, 10, 1};
  string tournPath = "tournaments/" + tournName;
  vector<stdBoard> gameBoards;
  gameList gL(population);
  std::uniform_int_distribution<int> distro(0, population-1);
  NN2 p1, p2;

  if (!is_directory(tournPath)) {
    cout << "directory doesn't exist!";
    create_directories(tournPath);
  }
  if (exists(tournPath + "/gamelist.txt")){  //gamelist exists, load it.
    std::ifstream ifs(tournPath + "/gamelist.txt");
    boost::archive::text_iarchive ia(ifs);
    ia >> gL;
    ifs.close();
  } else { //Gamelist doesn't exist, populate
    for (int i = 0; i < population; ++i ) {
      int p1,p2;
      do { //prevent games playing each other.
        p1 = distro(gen);
        p2 = distro(gen);
      } while (p1==p2);
      gL.addGame(p1,p2);
    }
    std::ofstream ofs(tournPath + "/gamelist.txt");
    boost::archive::text_oarchive oa(ofs);
    oa << gL;
    ofs.close();
  }
  for (int i = 0; i < population; ++i) {
    std::stringstream num;
    num << setfill('0') << setw(3) << i;
    if (!exists(tournPath + "/" + tournName + num.str() + ".nn2")) {
      cout << "Neural Network not found, creating!" << endl;
      std::ofstream ofs(tournPath + "/" + tournName + num.str() + ".nn2", std::ios::binary);
      boost::archive::binary_oarchive oa(ofs);
      oa << NN2(nodes, tournName + num.str());
      ofs.close();
    }
  }
  while(true) {
    while (gL.size() > 0) {
      //Load the relevant NNs
      cout << "Network " << gL.p1() << " vs " << gL.p2() << endl;
      std::stringstream num;
      num << setfill('0') << setw(3) << gL.p1();
      string player1 = tournPath + "/" + tournName + num.str() + ".nn2";
      p1.loadFromFile(player1);
      num.str("");
      num << setfill('0') << setw(3) << gL.p2();
      string player2 = tournPath + "/" + tournName + num.str() + ".nn2";
      p2.loadFromFile(player2);
      if (p1 == p2) {
        cout << "Error, Networks are the same!" << endl;
        return 1;
      }
      int gameResult = playGame(p1, p2, gameBoards);
      if (gameResult==0) { //tie
        ++gL.ties[gL.p1()];
        ++gL.ties[gL.p2()];
      } else if (gameResult==1) { //p1 wins
        ++gL.wins[gL.p1()];
        ++gL.losses[gL.p2()];
      } else {
        ++gL.losses[gL.p1()];
        ++gL.wins[gL.p2()];
      }
      gL.scores[gL.p1()] += gameResult;
      gL.scores[gL.p2()] -= gameResult;
      //Game has been completed and tallied.
      ++gL.gameNum;
      num.str("");
      num << setfill('0') << setw(3) << gL.tournCount;
      string tPath = tournPath + "/t" + num.str();
      create_directories(tPath);
      num.str("");
      num << setfill('0') << setw(3) << gL.gameNum;
      std::ofstream file(tPath + "/game" + num.str() + ".txt");
      if(!file){
        cout << "Game File Error." << endl;
        return -1;
      }
      if(boost::filesystem::is_directory(tPath)){
        if(gameResult > 0){
          //NeuralNets[i] won
          file << "winner: " << p1.familyName;
          file << " " << p1.generation;
          file << " " << gL.p1() << endl;

          file << "loser: " << p2.familyName;
          file << " " << p2.generation;
          file << " " << gL.p2() << endl;
        }
        else if(gameResult<0){
          //NeuralNets[randIndex]
          file << "winner: " << p2.familyName;
          file << " " << p2.generation;
          file << " " << gL.p2() << endl;

          file << "loser: " << p1.familyName;
          file << " " << p1.generation;
          file << " " << gL.p1() << endl;
        }
        else{
          //draw
          file << "Tie: " << p1.familyName;
          file << " " << p1.generation;
          file << " " << gL.p1() << endl;

          file << "Tie: " << p2.familyName;
          file << " " << p2.generation;
          file << " " << gL.p2() << endl;
        }

        //Save all the boards in the game into a file
        for(unsigned int k=0; k<gameBoards.size(); ++k){
          file << gameBoards[k].str() << endl;
        }
        gameBoards.clear();
        file.close();
      }
      gL.pop(); //Knock off the game we just did.
      //save the state.
      std::ofstream ofs(tournPath + "/gamelist.txt");
      boost::archive::text_oarchive oa(ofs);
      oa << gL;
      ofs.close();
    }
    //Playing games is over.  Evolve.
    vector<int> rankings;
    for(int i = 0;i<gL.population;++i) {
      rankings.push_back(i);
    }
    std::sort(rankings.begin(),rankings.end(),[&](int a, int b) {
              return gL.scores[a] > gL.scores[b];
              });
    for(int i = 0;i < gL.population; ++i) {
      cout << "NN2 " << rankings[i] << " with score " << gL.scores[rankings[i]]
       << " Wins: " << gL.wins[rankings[i]] << " Losses: " << gL.losses[rankings[i]]
       << " Ties: " << gL.ties[rankings[i]] << endl;
    }
    for (int i = 0; i < population/5; ++i) {
      NN2 evolve;
      stringstream num;
      num << setfill('0') << setw(3) << rankings[i];
      string fileName = tournPath + "/" + tournName + num.str() + ".nn2";
      evolve.loadFromFile(fileName);
      evolve.becomeOffspring();
      num.str("");
      num << setfill('0') << setw(3) << rankings[population-i-1];
      fileName = tournPath + "/" + tournName + num.str() + ".nn2";
      evolve.saveToFile(fileName);
    }

    //evolving done, reset.
      gL.scores.assign(population, 0);
      gL.wins.assign(population, 0);
      gL.losses.assign(population, 0);
      gL.ties.assign(population, 0);
      gL.gameNum = 0;
      ++gL.tournCount;

    //setup for more games
      for (int i = 0; i < population; ++i ) {
        int p1,p2;
        do { //prevent games playing each other.
          p1 = distro(gen);
          p2 = distro(gen);
        } while (p1==p2);
        gL.addGame(p1,p2);
      }
  }

  return 0;
}
/*

		//Tournament ended, choose the top 15 Neural Networks that
			//will generate children
		//Sort the vector by wins
		std::sort(NeuralNets.begin(), NeuralNets.end(),
			[](const NN & a, const NN & b) -> bool{
				return a.wins < b.wins;
			});

		//Last 15 will generate children (higher wins)
			//overtop the first 15
		//for(int i=NeuralNets.size()-1; i>NeuralNets.size()/2; --i){
		for(int i=NeuralNets.size()-1; i>5; --i){  //Only kill and replace 5.
			//save parent to file
			boost::filesystem::path totalPath = current_path;
			string path = parentDirectory + NeuralNets[i].familyName +
			"/" + "GEN" + to_string(NeuralNets[i].generation) +
			"/";
			string slash = "/" + path;
			totalPath += slash.c_str();
			boost::filesystem::path dir(totalPath);
			boost::filesystem::create_directories(dir);

			if(boost::filesystem::is_directory(dir)){
				path += "NN" + i;
				NeuralNets[i].saveToFile(path);
			}
			else{
				cout << "Path making failed." << endl;
				cout << path << endl;
				return -1;
			}

			//make loser network a child of this parent
			NeuralNets[i - NeuralNets.size()/2] = NeuralNets[i];
			NeuralNets[i - NeuralNets.size()/2].becomeOffspring();

		}

	}

	return 0;
}
*/
