//
//  connect4.cpp
//  connect4
//
//  Created by Bilal Najar on 3/24/17.
//  Copyright © 2017 Bilal Najar. All rights reserved.
//


#include "connect4.h"

State::State(double time)
{
    //Time(seconds) allowed per move
    timeAllowed = time;
    char temp;
    
    //allocate game board space
    board = new char * [ROW];
    for (int i = 0; i < ROW; i++)
    {
        *(board + i) = new char[COL];
    }
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++)
        {
            *(*(board + i) + j) = '.';
        }
    }
    
    //create letter arrays
    upperLetters = new char[ROW];
    for (int i = 65; i < (65 + ROW); i++)
    {
        temp = i;
        *(upperLetters + (i - 65)) = temp;
    }
    lowerLetters = new char[ROW];
    for (int i = 97; i < (97 + ROW); i++)
    {
        temp = i;
        *(lowerLetters + (i - 97)) = temp;
    }
}


State::~State()
{
    //delete game board
    for (int i = 0; i < ROW; i++)
    {
        delete[] * (board + i);
    }
    delete[] board;
    board = NULL;
    
    //delete letter arrays;
    delete[] upperLetters;
    upperLetters = NULL;
    delete[] lowerLetters;
    lowerLetters = NULL;
}

//Takes sign and move position and inserts it into the board
void State::enterMove(char move[], char sign)
{
    int row = 0;
    char letter = *(move + 0);
    char temp = *(move + 1);
    int col = (temp - '0') - 1;
    
    for (int i = 0; i < ROW; i++)
    {
        if (*(upperLetters + i) == letter || *(lowerLetters + i) == letter)
        {
            row = i;
            break;
        }
    }
    
    *(*(board + row) + col) = sign;
}

//Print the game board's current state
void State::printBoard()
{
    for (int i = 0; i < ROW; i++)
    {
        if (i == 0)
        {
            cout << "  ";
        }
        cout << (i + 1) << " ";
    }
    cout << endl;
    
    for (int i = 0; i < ROW; i++)
    {
        cout << *(upperLetters + i) << " ";
        
        for (int j = 0; j < COL; j++)
        {
            cout << *(*(board + i) + j) << " ";
        }
        cout << endl;
    }
}

//Checks to see if the sign played creates a row of four to determine a winner
bool State::checkWin(char sign)
{
    bool win = false;
    
    //check horizontal
    for (int i = 0; i < ROW; i++)
    {
        int count = 0;
        
        for (int j = 0; j < COL; j++)
        {
            if (*(*(board + i) + j) == sign)
            {
                count++;
            }
            else
            {
                count = 0;
            }
            if (count >= 4)
            {
                win = true;
                return win;
            }
        }
    }
    
    //check vertical
    for (int j = 0; j < COL; j++)
    {
        int count = 0;
        for (int i = 0; i < ROW; i++)
        {
            if (*(*(board + i) + j) == sign)
            {
                count++;
            }
            else
            {
                count = 0;
            }
            if (count >= 4)
            {
                win = true;
                return win;
            }
        }
    }
    
    return win;
}

//Makes sure the move is on the board and the space is empty
bool State::checkValidMove(char move[])
{
    
    bool valid = true;
    char letter = *(move + 0);
    char temp = *(move + 1);
    int col = (temp - '0') - 1;
    
    if (col < 0 || col > 7)
    {
        return false;
    }
    int row = 0;
    for (int i = 0; i < ROW; i++)
    {
        if (*(upperLetters + i) == letter || *(lowerLetters + i) == letter)
        {
            row = i;
            valid = true;
            break;
        }
    }
    if (*(*(board + row) + col) != '.')
    {
        return false;
    }
    return valid;
}


//Alpha-Beta Pruning function that recursively calls min
void State::alphaBetaPrune(char sign, char move[])
{
    
    //These are class variables that hold the positions of the best found move
    //made by the maximizer method.
    bestRow = 0;
    bestCol = 0;
    
    int a = -1000000;
    int b = 1000000;
    int depth = 5;
    
    startTime = 0;
    startTime = clock();
    
    int v = maxSearch(a, b, sign, depth, depth);

    for (int i = 0; i < ROW; i++)
    {
        if (i == bestRow){
            *(move + 0) = *(upperLetters + i);
        }
    }
    bestCol++;
    *(move + 1) = bestCol + '0';
    
    cout << "Computer has made the move: " << *(move + 0) << *(move + 1) << endl;

    enterMove(move, sign);
    
    
}

//Max method for alpha beta pruning
int State::maxSearch(int alpha, int beta, char sign, int depth, int original_depth)
{
    //Returns score to the evaluation function
    if (checkWin('X'))
        return 100000;
    else if (checkWin('O'))
        return -100000;
    //Return leaf node score to evaluation function
    if (depth == 0) {
        return evaluation();
    }
    
    int v = -1000000;
    //Timer to keep track of allowed time
    double time = ((double)(clock() - startTime)) / CLOCKS_PER_SEC;
    
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (time > timeAllowed) {
                if (v == -1000000)
                    v = -v;
                return v;
            }
            if (checkAlone(i, j))
                continue;
            
            //Add the current players sign to create successor states
            if (*(*(board + i) + j) == '.') {
                *(*(board + i) + j) = sign;
                //Change sign before minimizer method
                if (sign == 'O')
                    sign = 'X';
                else if (sign == 'X')
                    sign = 'O';
                int old_v = v;
                v = max(v, minSearch(alpha, beta, sign, depth - 1, original_depth));
                if (v > old_v && depth == original_depth) {
                    bestRow = i;
                    bestCol = j;
                }
                //Remove previous move
                *(*(board + i) + j) = '.';
                //Return to original sign after minimizer method.
                if (sign == 'O')
                    sign = 'X';
                else if (sign == 'X')
                    sign = 'O';
                if (v >= beta) {
                    return v;
                }
                alpha = max(alpha, v);
            }
        }
    }
    return v;
    
}

//Min method for alpha beta pruning
int State::minSearch(int alpha, int beta, char sign, int depth, int original_depth)
{
    //Returns score to the evaluation function
    if (checkWin('X'))
        return 100000;
    else if (checkWin('O'))
        return -100000;
    
    //Return leaf node score to evaluation function
    if (depth == 0) {
        return evaluation();
    }
    int v = 1000000;
    //Timer to keep track of allowed time
    double time = ((double)(clock() - startTime)) / CLOCKS_PER_SEC;
    
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (time > timeAllowed) {
                if (v == 1000000)
                    v = -v;
                return v;
            }
            
            if (checkAlone(i, j))
                continue;
            //Add the current players sign to create successor states
            if (*(*(board + i) + j) == '.') {
                *(*(board + i) + j) = sign;
                //Change sign before maximizer method
                if (sign == 'O')
                    sign = 'X';
                else if (sign == 'X')
                    sign = 'O';

                v = min(v, maxSearch(alpha, beta, sign, depth - 1, original_depth));
                //Remove previous move
                *(*(board + i) + j) = '.';
                //Return to original sign after maximizer method.
                if (sign == 'O')
                    sign = 'X';
                else if (sign == 'X')
                    sign = 'O';
                if (v <= alpha) {
                    return v;
                }
                beta = min(beta, v);
                
            }
        }
    }
    return v;
}

//Evaluation Function and its weights
int State::evaluation()
{
     int total = 0;
    
    int goodThrees = checkThreeInRow('X');
    int goodTwos = checkTwoInRow('X');
    int badThrees = checkThreeInRow('O');
    int badTwos = checkTwoInRow('O');
    
    total += (goodThrees * 1000);
    total += (goodTwos * 500);
    total -= (badThrees * 1500);
    total -= (badTwos * 800);
    
    return total;
}

//Checks to see if position is touching any other pieces for alpha-beta pruning evaluation
bool State::checkAlone(int i, int j)
{
    bool alone = false;
    
    if (i == 0 )
    {
        if (j == 0)
        {
            if (*(*(board + (i + 1)) + j) == '.' && *(*(board + i) + (j + 1)) == '.')
            {
                alone = true;
            }
        }
        else if (j == 7)
        {
            if (*(*(board + (i + 1)) + j) == '.' && *(*(board + i) + (j - 1)) == '.')
            {
                alone = true;
            }
        }
        else if (*(*(board + (i + 1)) + j) == '.' && *(*(board + i) + (j - 1)) == '.' && *(*(board + i) + (j + 1)) == '.')
        {
            alone = true;
        }
    }
    else if (j == 0)
    {
        if (i == 7)
        {
            if (*(*(board + (i - 1)) + j) == '.' && *(*(board + i) + (j + 1)) == '.')
            {
                alone = true;
            }
        }
        else if (*(*(board + (i - 1)) + j) == '.' && *(*(board + (i + 1)) + j) == '.' && *(*(board + i) + (j + 1)) == '.')
        {
            alone = true;
        }
    }
    else if (i == 7) {
        if (j == 7) {
            if (*(*(board + (i - 1)) + j) == '.' && *(*(board + i) + (j - 1)) == '.')
            {
                alone = true;
            }
        }
        else if (*(*(board + (i - 1)) + j) == '.' && *(*(board + i) + (j - 1)) == '.' && *(*(board + i) + (j + 1)) == '.')
        {
            alone = true;
        }
    }
    else if (j == 7) {
        if (*(*(board + (i - 1)) + j) == '.' && *(*(board + (i + 1)) + j) == '.' && *(*(board + i) + (j - 1)) == '.')
        {
            alone = true;
        }
    }
    else if (*(*(board + (i - 1)) + j) == '.' && *(*(board + (i + 1)) + j) == '.' && *(*(board + i) + (j + 1)) == '.' && *(*(board + i) + (j - 1)) == '.')
    {
        alone = true;
    }
    
    return alone;
    
}


//Checks to see if there is two in a row
int State::checkTwoInRow(char sign)
{
    char opponent;
    int start=0;
    
    int space = 0;
    int spacePos = 0;
    int twos = 0;

    
    if (sign == 'O')
        opponent = 'X';
    
    else if (sign == 'X')
        opponent = 'O';
    
    
    
    
    //check the horizontal row
    for (int i = 0; i < ROW; i++)
    {
        int count = 0;
        
        for (int j = 0; j < COL; j++)
        {
            int exists = false;
            
            //Checks to see if a two in a row is a part of a three in a row
            for (int k = 0; k < threeHori.size(); k++)
            {
                if (threeHori[k].size() > 0)
                {
                    if (threeHori[k][0] == i && (threeHori[k][1] == j || (threeHori[k][1] + 1) == j))
                    {
                        count = 0;
                        space = 0;
                        exists = true;
                        break;
                    }
                }
            }
            if (exists)
                break;
            if (*(*(board + i) + j) == sign && space < 2)
            {
                if (count == 0) {
                    start = j;
                }
                count++;
                
            }
            else if (*(*(board + i) + j) != sign && space >= 1)
            {
                count = 0;
                space = 0;
                spacePos = 0;
            }
            else if (*(*(board + i) + j) != sign && space < 2)
            {
                space++;
                spacePos = j;
            }
            if (count >= 2)
            {
                if (space == 1)
                {
                    if (*(*(board + i) + (spacePos)) == '.')
                        twos += 10;
                    else if (opponent == *(*(board + i) + (spacePos)))
                        twos -= 10;
                }
                else
                {
                    if (start == 0)
                    {
                        if (opponent == *(*(board + i) + (j + 1)))
                        {
                            twos -= 10;
                        }
                        else if (*(*(board + i) + (j + 1)) == '.')
                        {
                            twos += 10;
                        }
                    }
                    else if (start == 6)
                    {
                        if (opponent == *(*(board + i) + (start - 1)))
                        {
                            twos -= 5;
                        }
                        else if (*(*(board + i) + (start - 1)) == '.')
                        {
                            twos += 5;
                        }
                    }
                    else {
                        if (opponent == *(*(board + i) + (start - 1)) && opponent == *(*(board + i) + (j + 1)))
                        {
                            twos -= 8;
                        }
                        else if (opponent == *(*(board + i) + (start - 1)))
                        {
                            twos -= 6;
                        }
                        else if (opponent == *(*(board + i) + (j + 1)))
                        {
                            twos -= 6;
                        }
                        else if ((*(*(board + i) + (start - 1)) == '.' && *(*(board + i) + (j + 1)) == '.'))
                        {
                            twos += 7;
                        }
                    }
                }
                
                space = 0;
                spacePos = 0;
                count = 0;
            }
        }
    }
    
    //check vertical
    for (int j = 0; j < COL; j++)
    {
        int count = 0;
        for (int i = 0; i < ROW; i++)
        {
            bool exists = false;
            
            for (int k = 0; k < threeVert.size(); k++)
            {
                if (threeVert[k].size() > 0)
                {
                    if ((threeVert[k][0] == i || (threeVert[k][0] + 1) == i) && threeVert[k][1] == j)
                    {
                        count = 0;
                        space = 0;
                        exists = true;
                        break;
                    }
                }
            }
           
            if (exists)
                break;
            
            if (*(*(board + i) + j) == sign && space < 2)
            {
                if (count == 0)
                {
                    start = i;
                }
                count++;
            }
            else if (opponent == *(*(board + i) + j))
            {
                count = 0;
                space = 0;
            }
            else if (*(*(board + i) + j) == '.' && space >= 1)
            {
                count = 0;
                space = 0;
            }
            else if (*(*(board + i) + j) == '.' && space < 2)
            {
                space++;
            }
            if (count >= 2)
            {
                if (space == 1)
                {
                    if (*(*(board + (spacePos)) + j) == '.')
                        twos += 10;
                    else if (opponent == *(*(board + (spacePos)) + j))
                        twos -= 10;
                }
                else
                {
                    if (start == 0)
                    {
                        if (opponent == *(*(board + (i + 1)) + j))
                        {
                            twos -= 10;
                        }
                        else if (*(*(board + (i + 1)) + j) == '.')
                        {
                            twos += 10;
                        }
                    }
                    else if (start == 6)
                    {
                        if (opponent == *(*(board + (start - 1)) + j))
                        {
                            twos -= 10;
                        }
                        else if (*(*(board + (start - 1)) + j) == '.')
                        {
                            twos += 10;
                        }
                    }
                    else
                    {
                        if (opponent == *(*(board + (start - 1)) + j) && opponent == *(*(board + (i + 1)) + j))
                        {
                            twos -= 8;
                        }
                        else if (opponent == *(*(board + (start - 1)) + j))
                        {
                            twos -= 6;
                        }
                        else if (opponent == *(*(board + (i + 1)) + j))
                        {
                            twos -= 6;
                        }
                        else if ((*(*(board + (start - 1)) + j) == '.' && *(*(board + (i + 1)) + j) == '.'))
                        {
                            twos += 7;
                        }
                    }
                }
                space = 0;
                spacePos = 0;
                count = 0;
            }
        }
    }
    
    
    //delete the vectors used
    for (int i = 0; i < 10; i++)
    {
        threeVert.erase(threeVert.end() - 1);
        threeHori.erase(threeHori.end() - 1);
    }
    
    threeVertRow = 0;
    threeHoriRow = 0;
    
    return twos;
    
}

//Checks if there is a three in a row
int State::checkThreeInRow(char sign)
{
    
   //Allocate space for the positions of three in a rows
    for (int i = 0; i < 10; i++)
    {
        threeHori.push_back(vector<int>());
        threeVert.push_back(vector<int>());
    }
    
    char opponent;
    int start = 0;
    int space = 0;
    int spacePos = 0;
    int threes = 0;
    
    if (sign == 'O')
        opponent = 'X';
    else if (sign == 'X')
        opponent = 'O';
    
    
    
    //check horizontal
    for (int i = 0; i < ROW; i++)
    {
        int count = 0;
        
        for (int j = 0; j < COL; j++)
        {
            if (*(*(board + i) + j) == sign && space < 2)
            {
                if (count == 0)
                {
                    start = j;
                }
                count++;
                
            }
            else if (*(*(board + i) + j) != sign && space >= 1)
            {
                count = 0;
                space = 0;
                spacePos = 0;
            }
            else if (*(*(board + i) + j) != sign && space < 2)
            {
                spacePos = j;
                space++;
            }
            if (count >= 3)
            {
                if (space == 1)
                {
                    if (*(*(board + i) + (spacePos)) == '.')
                        threes += 10;
                    
                    else if (opponent == *(*(board + i) + (spacePos)))
                        threes -= 10;
                }
                else
                {
                    if (start == 0) {
                        if (opponent == *(*(board + i) + (j + 1)))
                        {
                            threes -= 10;
                        }
                        
                        else if (*(*(board + i) + (j + 1)) == '.')
                        {
                            threes += 10;
                        }
                    }
                    else if (start == 5)
                    {
                        if (opponent == *(*(board + i) + (start - 1)))
                        {
                            threes -= 10;
                        }
                        
                        else if (*(*(board + i) + (start - 1)) == '.')
                        {
                            threes += 10;
                        }
                    }
                    else
                    {
                        if (opponent == *(*(board + i) + (start - 1)) && opponent == *(*(board + i) + (j + 1)))
                        {
                            threes -= 11;
                        }
                        else if (opponent == *(*(board + i) + (start - 1)))
                        {
                            threes += 10;
                        }
                        else if (opponent == *(*(board + i) + (j + 1)))
                        {
                            threes += 10;
                        }
                        else if ((*(*(board + i) + (start - 1)) == '.' && *(*(board + i) + (j + 1)) == '.'))
                        {
                            threes += 15;
                        }
                    }
                }
            
                space = 0;
                spacePos = 0;
                count = 0;
                
                //Add the position of the three in a row
                threeHori[threeHoriRow].push_back(i);
                threeHori[threeHoriRow].push_back(start);
                threeHoriRow++;
                
            }
        }
    }
    
    //check vertical
    for (int j = 0; j < COL; j++)
    {
        int count = 0;
        
        for (int i = 0; i < ROW; i++)
        {
            if (*(*(board + i) + j) == sign && space < 2)
            {
                if (count == 0)
                {
                    start = i;
                }
                count++;
            }
            else if (*(*(board + i) + j) != sign && space >= 1)
            {
                count = 0;
                space = 0;
                spacePos = 0;
            }
            else if (*(*(board + i) + j) != sign && space < 2)
            {
                space++;
                spacePos = i;
            }
            if (count >= 3)
            {
                if (space == 1)
                {
                    if (*(*(board + (spacePos)) + j) == '.')
                        threes += 10;
                    else if (opponent == *(*(board + (spacePos)) + j))
                        threes -= 10;
                }
                else
                {
                    if (start == 0)
                    {
                        if (opponent == *(*(board + (i + 1)) + j))
                        {
                            threes -= 10;
                        }
                        else if (*(*(board + (i + 1)) + j) == '.')
                        {
                            threes += 10;
                        }
                    }
                    else if (start == 5)
                    {
                        if (opponent == *(*(board + (start - 1)) + j))
                        {
                            threes -= 10;
                        }
                        else if (*(*(board + (start - 1)) + j) == '.')
                        {
                            threes += 10;
                        }
                    }
                    else
                    {
                        if (opponent == *(*(board + (start - 1)) + j) && opponent == *(*(board + (i + 1)) + j))
                        {
                            threes -= 11;
                        }
                        else if (opponent == *(*(board + (start - 1)) + j))
                        {
                            threes += 10;
                        }
                        else if (opponent == *(*(board + (i + 1)) + j))
                        {
                            threes += 10;
                        }
                        else if ((*(*(board + (start - 1)) + j) == '.' && *(*(board + (i + 1)) + j) == '.'))
                        {
                            threes += 15;
                        }
                    }
                }
                
                space = 0;
                spacePos = 0;
                count = 0;
                
                threeVert[threeVertRow].push_back(start);
                threeVert[threeVertRow].push_back(j);
                threeVertRow++;
            }
        }
    }
    return threes;
}


