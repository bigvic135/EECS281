//
//  PuzzleSolver.hpp
//  Project1
//
//  Created by BigVic on 9/13/17.
//  Copyright © 2017 mac. All rights reserved.
//

#ifndef PuzzleSolver_h
#define PuzzleSolver_h
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <deque>
#include <cstring>
#include <sstream>
#include <getopt.h>
#include <algorithm>
using namespace std;

struct State {
    char color;
    int row;
    int col;
};


typedef vector<vector<char> > TwoDMap;
typedef vector<vector<vector<char> > > ThreeDMap;

// 2d map and 3d retraceable and backtracing map


string getOptions(int argc, char *argv[]);
bool validInput(char symbol);
bool validLocation(ThreeDMap &threeDmap, int index, State &temp);
vector<vector<char> > makeMap();
void mapOutput(ThreeDMap &threeDmap, TwoDMap &twoDmap);
void listOutput(ThreeDMap &threeDmap, TwoDMap &twoDmap);
void findEscape(ThreeDMap &threeDmap, TwoDMap &twoDmap);
bool reachablePoint(TwoDMap &twoDmap, int direction, State &temp);
void noSolution(ThreeDMap &threeDmap, TwoDMap &twoDmap);
char changefloortoColor(int floor);

// need a class for player (location)
// need a class for the whole map
// station put the first three variables
#endif /* PuzzleSolver_hpp */
