//Usage of NN class

//Test a bigger network
std::vector<int> nodes{32, 50, 50, 50, 100, 50, 1};
NN bigN(nodes, "bigN");

//Blondie24
std::vector<int> nodes{ 32, 40, 10, 1};
NN blondie(nodes, "blondie24");


//Timing
TEST_CASE("Timing Blondie24.", "{32, 40, 10, 1}"){
	
	//Timing
	stdBoard b; //Default
	stdBoard b1;
	b1.updateBoard("           r       b            ");
	stdBoard b2;
	b2.updateBoard("rrrrrrrrrr r  r  b  bb bbbbbbbbb");
	stdBoard b3;
	b3.updateBoard("rrrrr    r       r    b bbbbbbbb");
	std::vector<stdBoard> boards{b, b1, b2, b3};

	std::vector<int> nodes{ 32, 40, 10, 1};

	std::mt19937 gen(time(0));
	std::uniform_int_distribution<int> dis(0,boards.size()-1);

	clock_t start;
	double duration;
	start = clock();
	NN blondie(nodes, "blondie24");

	int runs = 1000;
	double outputTotal = 0; 
	for(int i=0; i<runs; ++i){
		outputTotal += blondie.calculateBoard(boards[dis(gen)]);
	}

	duration = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "To not compiler optimize: " << outputTotal << endl;
	cout << "Blondie24 Time Difference: " << duration << endl;

	double timePerBoard = duration/runs;
	cout << "Time per board: " << timePerBoard << endl;
	cout << "Board Evals per second: " << runs/duration << endl;
	cout << endl; 
	//We should have at least 12,000 boards per second
	REQUIRE(runs/duration > 10000); 
}

TEST_CASE("Timing for Big Neural Network.", 
	"{32, 50, 50, 50, 100, 50, 1}"){	

	stdBoard b; //Default
	stdBoard b1;
	b1.updateBoard("           r       b            ");
	stdBoard b2;
	b2.updateBoard("rrrrrrrrrr r  r  b  bb bbbbbbbbb");
	stdBoard b3;
	b3.updateBoard("rrrrr    r       r    b bbbbbbbb");
	std::vector<stdBoard> boards{b, b1, b2, b3};

	std::mt19937 gen(time(0));
	std::uniform_int_distribution<int> dis(0,boards.size()-1);

	//Start the timer
	clock_t start, stop;
	double duration;

	std::vector<int> nodes{32, 50, 50, 50, 100, 50, 1};
	NN loopTimedNN(nodes, "bigNTimed");

	int randIndex = 0;
	//For loop for timing purposes, testing 4 boards chosen randomly
	int runs = 1000;
  	double answers[1000];
	start = clock();

	for(int i=0; i<runs; ++i){
		randIndex = dis(gen);
    	answers[i] = loopTimedNN.calculateBoard(boards[randIndex]);
	}
	stop = clock();
	duration = (stop - start) / (double) CLOCKS_PER_SEC;

  	std::uniform_int_distribution<int> dis1k(0,1000);
  	cout << "Random Board Value: " << answers[dis1k(gen)] << endl;


	cout << "Time Difference: " << duration << endl;

	double timePerBoard = duration/runs;
	cout << "Time per board: " << timePerBoard << endl;
	cout << "Board Evals per second: " << runs/duration << endl;

	REQUIRE(runs/duration > 10000); //at least 10,000 boards per sec
}


//Results from Timing: 
/*
Timing Results: 
 BigNN:  
    Non optimizer flag : 1,500 - 2,000 boards/sec
    -O2 flag : 15,000 - 25,000 boards/sec 
    CPU : Intel Core i5-2410M (Dual Core) @ 2.30 GHz  
   
 Blondie24: 
    Non optimizer flag: 15,000 boards/sec
    -O2 flag: 150,000 boards/sec 
	CPU : Intel Core i5-2410M (Dual Core) @ 2.30 GHz  
*/ 
