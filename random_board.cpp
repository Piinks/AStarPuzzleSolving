// Kate Lovett
// random_board.cpp
// September 2017
// This program reads in the goal state of the 8-piece puzzle board from stdin.
// It then initiates random moves (# specified as a command line
// argument), and outputs the final scrambled board to stdout.

#include <iostream>
#include <stdlib.h>
#include <cstdio>
//#include <string>

using namespace std;

void moveTiles(int &zeroIndex, int move, int board[]);

int main(int argc, char *argv[]){

    int board[9];
    int zeroIndex = 0;
    int move;

    // Read in the board.
    for(int i = 0; i < 9; i++){
        cin >> board[i];
    }

    // Initialize random seed from command-line argument.
    srand(atoi(argv[1]));


    // Make the number of moves specified by the command-line argument.
    for(int i = 0; i < atoi(argv[2]); i++){

        if(zeroIndex == 4){
            move = rand() % 4;      // 4 moves available at this position.
        }
        else if(zeroIndex == 0 || zeroIndex == 2 || zeroIndex == 6 || zeroIndex == 8){
            move = rand() % 2;      // 2 moves available at these positions.
        }
        else{
            move = rand() % 3;      // 3 moves available at these positions.
        }

        moveTiles(zeroIndex, move, board);
    }

    // Print final configuration to stdout
    for(int i = 0; i < 9; i+=3){
        printf("%d %d %d\n", board[i], board[i+1], board[i+2]);
    }

    return 0;
}

// Function to interpret and complete the chosen move
// Parameters: Current index of the empty tile - by reference - (zeroIndex),
//              the randomly selected move (move),
//              the board array - by reference.
void moveTiles(int &zeroIndex, int move, int board[]){

    int temp = board[zeroIndex];
    int swapIndex;

    switch(zeroIndex){                      // Empty tile moves:
        case 0 :
            if(!move)       { swapIndex = 1; }     // right
            else            { swapIndex = 3;}      // down
            break;

        case 1 :
            if(!move)       { swapIndex = 0; }      // left
            else if(move==1){ swapIndex = 2; }      // right
            else            { swapIndex = 4; }      // down
            break;

        case 2 :
            if(!move)       { swapIndex = 1; }      // left
            else            { swapIndex = 5; }      // down
            break;

        case 3 :
            if(!move)       { swapIndex = 0; }      // up
            else if(move==1){ swapIndex = 4; }      // right
            else            { swapIndex = 6; }      // down
            break;

        case 4 :
            if(!move)       { swapIndex = 1; }      // up
            else if(move==1){ swapIndex = 3; }      // left
            else if(move==2){ swapIndex = 5; }      // right
            else            { swapIndex = 7; }      // down
            break;

        case 5 :
            if(!move)       { swapIndex = 2; }      // up
            else if(move==1){ swapIndex = 4; }      // left
            else            { swapIndex = 8; }      // down
            break;

        case 6 :
            if(!move)       { swapIndex = 3; }      // up
            else            { swapIndex = 7; }      // right
            break;

        case 7 :
            if(!move)       { swapIndex = 6; }      // left
            else if(move==1){ swapIndex = 4; }      // up
            else            { swapIndex = 8; }      // right
            break;

        case 8 :
            if(!move)       { swapIndex = 5; }      // up
            else            { swapIndex = 7; }      // left
            break;

        default : break;
    }

    // Simple swap
    board[zeroIndex] = board[swapIndex];
    board[swapIndex] = temp;
    // Update location of the empty tile
    zeroIndex = swapIndex;
}
