//
//  main.cpp
//  connect4
//
//  Created by Bilal Najar on 3/24/17.
//  Copyright © 2017 Bilal Najar. All rights reserved.
//
#include "connect4.h"

int main()
{
    
    int counter = 0;
    char human = 'O';
    char AI = 'X';
    char *move = NULL;
    char sign;
    int turn;
    
    cout << "Who has the first move? (Human=1/Computer=0)" << endl;
    cin >> turn;
    
    int moveCount = 0;
    
    int time;
    
    cout << "How much time does the computer have to make a move? (in seconds)" << endl;
    cin >> time;
    
    State state((double)time);
    state.printBoard();
    
    while (true) {
        
        if (turn == 1) {
            sign = human;
            move = new char[3];
            cout << "Enter your move: ";
            
            if (counter == 0 || counter == 1)
                cin.ignore();
            
            cin.getline(move, 3);
            
            if (!state.checkValidMove(move))
            {
                cout << "Invalid move, try again." << endl;
                counter = 2;
                continue;
            }
            
            state.enterMove(move, sign);
            state.printBoard();
           
            bool win = state.checkWin(sign);
            if (win) {
                cout << "\n\nYou Win" << endl;
                break;
            }
            moveCount++;
        }
        
        else
        {
            sign = AI;
            move = new char[3];
            
            state.alphaBetaPrune(sign, move);
            state.printBoard();
            
            bool win = state.checkWin(sign);
            
            if (win)
            {
                cout << "\n\nThe computer won" << endl;
                break;
            }
            counter = 2;
            
            if (moveCount == 0)
            {
                counter = 1;
                moveCount++;
            }
        }
        
        turn=abs(turn-1);
        
    }
    
    return 0;
    
}
    
    
