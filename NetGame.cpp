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

#include <ctime>
#include <sstream>
#include "neuralNetwork.h"
#include "board.h"
#include <json/json.hpp>
//#include <sys/select.h>
#include <curl/curl.h>
#include <thread>
#include <chrono>


using std::cout;
using std::endl;
using json = nlohmann::json;

#define MAXGAMEMOVES 200
#define WINVALUE 12
#define LOSEVALUE -12
#define DEPTH 4
#define BOARDSIZE 32

/*
** Arguments:
** Brain to use
** Depth to go to
** Game name
** 1 = red, else black.
*/

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//Arguments:
//brain depth gamename side

int main(int argc, char * argv[]) {

// Initial variables
//	int NumMoves;
//	int curPlayer = 0;
//	int round = 0;
//	bool gamelost = false;
	std::string srvAddr = "http://skynet.cs.uaf.edu";
	std::string strRequest;
	std::string gameName;
	std::string svrResponse;  //Holds responses from the server.
	std::string sendMoveInfo;
	std::string getGameInfo;
	CURLcode curlR;
	AIPlayer * Brain;
	stdBoard board;

// Set variables.
  std::string AI = argv[1];
  if (AI=="piececount") {
      Brain = new PieceCount;
  } else if (true) {
      Brain = new NN;
      Brain->loadFromFile(AI);
  } else {
      Brain = new NN2;
      Brain->loadFromFile(AI);
  }


//	Brain.depth = atoi(argv[2]);
	gameName = argv[3];
	int side = atoi(argv[4]); // 1 = red
	getGameInfo = "{\"name\":\"" + gameName + "\"}";
// Initialize curl
	curl_global_init(CURL_GLOBAL_ALL);
	CURL *curl;
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, & WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, & svrResponse);
//	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	while(true) {
		json jGameInfo;
		std::stringstream buffer;
		svrResponse = "";
	// Get the game
		if(curl) {
			strRequest = srvAddr + "/?info_game=true";
//			cout << srvAddr + "?info_game=true" << endl;
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, getGameInfo.c_str());
			curl_easy_setopt(curl, CURLOPT_URL, strRequest.c_str());
			curlR = curl_easy_perform(curl);
			if(curlR) {
				cout << "Error in request!" << endl;
				return 1;
			}
			buffer << svrResponse;
			buffer >> jGameInfo; // deserialize
			cout << "Status: " << jGameInfo["status"] << endl;
			board.str(jGameInfo["boards"].back());
			cout << "board: " << board.strNet() << endl;
			if(jGameInfo["status"] == "red_turn") {
				cout << "Red Turn" << endl;
				if(side == 0) {
						std::this_thread::sleep_for(std::chrono::seconds(1));
						continue;
				}
			} else if (jGameInfo["status"] == "black_turn") {
				cout << "Black Turn" << endl;
				if(side==1){
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
			} else {
				cout << "Invalid Game?" << endl;
				cout << svrResponse << endl;
				return 1;
			}
		}
	// Set the board code

		//timing code
		time_t time_start1;
		time_t time_end1;
		time(&time_start1);
		//end timing code
		board = Brain->getMove(board, side);
		Brain->prntStats();
//		PlayMove(Brain, board, depth);
		time(&time_end1);
		std::cout << "It took " << difftime(time_end1,time_start1) << " seconds for the move" << endl;
		cout<< "Sending board: " << board.strNet() << endl;
	//return the move
		strRequest = srvAddr + "/?play_game=true";
		sendMoveInfo = "{\"name\":\"" + gameName + "\",\"board\":\"" + board.strNet() + "\"}";
		cout << strRequest << endl;
		cout << sendMoveInfo << endl;

		svrResponse = "";
		if(curl) {
			curl_easy_setopt(curl, CURLOPT_URL, strRequest.c_str());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sendMoveInfo.c_str());
			curlR = curl_easy_perform(curl);
			if (svrResponse == "{}") {
				cout << "Move successfully sent!" << endl;
			} else {
				cout << "Send return buffer: " << svrResponse << endl;
			}
		}
	}
	return 0;
}
