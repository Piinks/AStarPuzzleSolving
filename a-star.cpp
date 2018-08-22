// Kate Lovett
// a-star.cpp
// September 2017
// This program reads in the current state of an 8-piece puzzle board from stdin.
// It then utilizes the a-star algorithm to solve the puzzle, based on the
// goal state 0,1,2,3,4,5,6,7,8. The 0 tile represents an empty space, which
// allows the other tiles to move.
// This program takes one command line argument, which determines the heuristic.
// Once the goal has been found, the program will print the total # of nodes
// visited, the max # of nodes stored in memory, the depth of the optimal solution,
// the approximate effective branching factor, and the board's states from the
// start to the goal.

#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stack>
#include <string>
#include <set>
using namespace std;

struct node{
  int idNum;
  string boardConfig;
  string parentConfig;
  int gN;
  int hN;
  int fN;
  char searchType;

// These two operator functions serve as the method for the closed set
// to search and compare boards.
  bool operator<(const node& rhs) const{
	  return boardConfig < rhs.boardConfig;
  }

  bool operator==(const node& rhs) const{
	  return boardConfig == rhs.boardConfig;
  }
};

// This small class serves as the method for the frontier prioity
// queue to compare and organize itself.
class pqComp{
public:
    pqComp(){}
	bool operator() (const node& lhs, const node& rhs) const{
		if(lhs.fN != rhs.fN){
			return lhs.fN > rhs.fN;
		}
		else{
			return lhs.idNum < rhs.idNum;
		}
	}
};

int calcHN(int hMethod, string board);
string swap(int a, int b, string board);  //Non-destructive
bool costComp(node a, node b);
void printBoard(string boardConfig);

int main(int argc, char *argv[]){

	priority_queue<node, vector<node>, pqComp> frontier;
	set<node> closed;
	stack<string> solution;
	int hMethod = atoi(argv[1]);
    string goal = "012345678";
    int nodeCounterV = 1;
    int idCounter = 0;

    string temp;

    node currentNode;
    currentNode.parentConfig = "";
    currentNode.idNum = 0;
    currentNode.boardConfig = "";
    for(int i =0; i < 9; i++){
		cin >> temp;
		currentNode.boardConfig += temp;
    }
    currentNode.gN = 0;
    currentNode.hN = calcHN(hMethod, currentNode.boardConfig);
    currentNode.fN = currentNode.gN + currentNode.hN;

    frontier.push(currentNode);

	while(currentNode.boardConfig!=goal){ // This will detect the goal, leaving the currentBoard as the solution at exit of loop

		nodeCounterV += 1;

        // If the goal has not been found, find out where zero is
        int zeroIndex = currentNode.boardConfig.find("0");


        // Add current node to the closed list and remove from the frontier.
        closed.insert(frontier.top());

		frontier.pop();


        // Find next nodes + fn -> add to frontier after checking against the closed list
        // Testing for moves working clockwise around the current location of the empty tile.

        if(zeroIndex - 3 >= 0){                                         // Up
			node newNode;
			newNode.boardConfig = swap(zeroIndex, zeroIndex-3, currentNode.boardConfig);

			if(closed.find(newNode) == closed.end()){

				idCounter += 1;
				newNode.idNum = idCounter;

				newNode.parentConfig = currentNode.boardConfig;

				newNode.gN = currentNode.gN + 1;
				newNode.hN = calcHN(hMethod, newNode.boardConfig);
				newNode.fN = newNode.hN + newNode.gN;

				frontier.push(newNode);
			}
		}

        if(zeroIndex + 3 <= 8){                                         // Down
			node newNode;
			newNode.boardConfig = swap(zeroIndex, zeroIndex+3, currentNode.boardConfig);

			if(closed.find(newNode) == closed.end()){

				idCounter += 1;
				newNode.idNum = idCounter;

				newNode.parentConfig = currentNode.boardConfig;

				newNode.gN = currentNode.gN + 1;
				newNode.hN = calcHN(hMethod, newNode.boardConfig);
				newNode.fN = newNode.hN + newNode.gN;

				frontier.push(newNode);
			}
		}

		if((zeroIndex != 0)&&(zeroIndex!=3)&&(zeroIndex != 6)){
			node newNode;                                               // Left
			newNode.boardConfig = swap(zeroIndex, zeroIndex-1, currentNode.boardConfig);

			if(closed.find(newNode) == closed.end()){

				idCounter += 1;
				newNode.idNum = idCounter;

				newNode.parentConfig = currentNode.boardConfig;

				newNode.gN = currentNode.gN + 1;
				newNode.hN = calcHN(hMethod, newNode.boardConfig);
				newNode.fN = newNode.hN + newNode.gN;

				frontier.push(newNode);
			}
		}

		if((zeroIndex != 2)&&(zeroIndex!=5)&&(zeroIndex != 8)){
			node newNode;                                               // Right
			newNode.boardConfig = swap(zeroIndex, zeroIndex+1, currentNode.boardConfig);

			if(closed.find(newNode) == closed.end()){

				idCounter += 1;
				newNode.idNum = idCounter;

				newNode.parentConfig = currentNode.boardConfig;

				newNode.gN = currentNode.gN + 1;
				newNode.hN = calcHN(hMethod, newNode.boardConfig);
				newNode.fN = newNode.hN + newNode.gN;

				frontier.push(newNode);
			}
		}


		// Choose next move & continue to solve
		currentNode = frontier.top();

		// End of while loop
	}


		// Backtrack to find the solution path
		while(currentNode.parentConfig != ""){
			//printBoard(currentNode.boardConfig);
			//cout << endl;
			solution.push(currentNode.boardConfig);
			currentNode.boardConfig = currentNode.parentConfig;
			currentNode = *closed.find(currentNode);
		}
		// Push the final board (should be starting board) onto stack.
		solution.push(currentNode.boardConfig);

		unsigned long n = frontier.size() + closed.size();  // Calculates N and d.
		unsigned long d = solution.size()-1;

		cout << "V=" << nodeCounterV << endl;        // Outputs data followed by each state
		cout << "N=" << n << endl;                   // along the optimal path to the goal.
		cout << "d=" << d << endl;
		cout << "b=" << pow(n, 1.0/d) << endl << endl << endl;


		// cout << "Printing solution: " << endl << endl;
		for(int i = 0; i <= d; i ++){
			printBoard(solution.top());
			solution.pop();
			cout << endl << endl;
		}


	return 0;
}

string swap(int a, int b, string board){
  char temp = board[a];
  board[a] = board[b];
  board[b] = temp;
  return board;
}

int calcHN(int hMethod, string board){
  int cost1 = 0;
  int cost2 = 0;
  int initialX, initialY, targetX, targetY;

  for(int i = 0 ; i < 9; i++){

    int tile = board[i] - '0';
    if((tile!=0)&&(tile != i)){

      // for hN == 1
      cost1 += 1;

      // for hN == 2
      int targetY = (int)tile/3;
      int initialY = (int)i/3;
      int diffY = abs(targetY-initialY);

      if((i == 0)||(i == 3)||(i == 6)){
        initialX = 0;
      }
      else if((i == 1)||(i == 4)||(i == 7)){
        initialX = 1;
      }
      else{
        initialX = 2;
      }

      if((tile == 0)||(tile == 3)||(tile == 6)){
        targetX = 0;
      }
      else if((tile == 1)||(tile == 4)||(tile == 7)){
        targetX = 1;
      }
      else{
        targetX = 2;
      }

      int diffX = abs(targetX-initialX);

      cost2 += diffY + diffX;
    }
  }

  if(!hMethod)          { return 0; }
  else if(hMethod == 1) { return cost1; }
  else if(hMethod == 2) { return cost2; }
  else                  { return ((cost2 + cost1)/2); }
}

void printBoard(string boardConfig){

  for(int i = 0; i < 9; i+=3){
    cout << boardConfig[i] << ' ' << boardConfig[i+1] << ' ' << boardConfig[i+2] << endl;
  }
}
