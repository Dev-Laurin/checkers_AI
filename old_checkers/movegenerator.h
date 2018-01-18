#ifndef MOVEGENERATOR
#define MOVEGENERATOR

#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>

#define MAXBOARDS 20
#define BOARDSIZE 32
#define REDPAWN 'r'
#define BLACKPAWN 'b'
#define REDKING 'R'
#define BLACKKING 'B'
#define EMPTYCELL '_'
#define VALIDMOVEWIDTH 8

void PrintBoard(char Board[32]);
void PrintBoardLine(char Board[32]);

void kingMaker(char Boardret[]);

bool jumpcheck(const char ValidMoves[], const char BoardStr[],int x, int pieceValue, char Boardret[MAXBOARDS][BOARDSIZE], int & numMoves);

int MoveGenerator (char BoardStr[], char Boardret[MAXBOARDS][BOARDSIZE], bool isBlack = true);

void flipBoard(char BoardStr[]);
void flipBoard(std::string & Board);

#endif