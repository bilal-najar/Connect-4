//
//  connect4.h
//  connect4
//
//  Created by Bilal Najar on 3/24/17.
//  Copyright © 2017 Bilal Najar. All rights reserved.
//

#ifndef connect4_h
#define connect4_h

#include<iostream>
#include<stdio.h>
#include<vector>
#include<string>
#include<ctime>
#include<cmath>


using namespace std;

class State {
    
private:
    const int ROW = 8;
    const int COL = 8;
    int bestRow = 0;
    int bestCol = 0;
    char **board = NULL;
    char *upperLetters = NULL;
    char *lowerLetters = NULL;
    vector<vector<int> > threeHori;
    vector<vector<int> > threeVert;
    int threeVertRow= 0;
    int threeHoriRow = 0;
    clock_t startTime;
    double timeAllowed;
    
public:
    State(double);
    ~State();
    void enterMove(char[], char);
    void printBoard();
    bool checkWin(char);
    bool checkValidMove(char[]);
    void alphaBetaPrune(char, char[]);
    int maxSearch(int, int, char, int, int);
    int minSearch(int, int, char, int, int);
    int evaluation();
    bool checkAlone(int, int);
    int checkTwoInRow(char);
    int checkThreeInRow(char);
    
};


#endif
