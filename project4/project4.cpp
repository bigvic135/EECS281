//
//  main.cpp
//  project4
//
//  Created by BigVic on 12/3/17.
//  Copyright © 2017 mac. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <getopt.h>
#include<vector>
#include<cmath>
#include<cstdlib>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip>
#include<cstdint>
#include<deque>
#include<stack>

using namespace std;

double upperBound = 0;
double currentTotalDist = 0;


struct Location
{
    double x;
    double y;
    int type; // tame = 0, wild = 1, border = 2
};

class adjMatrix
{
public:
    vector<Location> cageLocation; // the location information for each base
    vector<vector<double>> adjMatrix; // the adjacent matrix
    int numCages;
    string mode; // project output mode
};

double findDistance(const Location &l1, const Location &l2){
    
    double distX = l1.x - l2.x;
    double distY = l1.y - l2.y;
    double dist = distX * distX + distY * distY;
    dist = sqrt(dist);
    //double dist = hypot(distX, distY);
    return dist;
}

bool checkValidtour(const Location &l1, const Location &l2){
    // wild cages are connected to all wild and border cages
    // non wild cages are connected to all non wild and border cages
    // border cages are connected to all cages
    
    // tame
    if (l1.type == 0){
        if (l2.type == 0 || l2.type == 2){
            return true;
        }
        else {
            return false;
        }
    }
    // wild
    else if (l1.type == 1){
        if (l2.type == 1 || l2.type == 2){
            return true;
        }
        else {
            return false;
        }
    }
    // border
    else {
        return true;
    }
}

int findCage(vector<int> &visitied){
    int numCages = (int)visitied.size();
    int index = rand() % numCages;
    while (visitied[index] == 1){
        index = rand() % numCages;
    }
    return index;
}

int findMinInsertionCost(int newCage, vector<int> &subTour, adjMatrix &matrix){
    int index = 0;
    size_t numCagesVisited = subTour.size();
    double minDistance = __DBL_MAX__;
    double currentDistance;
    for (int i = 0; i < (int)numCagesVisited - 1; ++i){
        //currentDistance = matrix.adjMatrix[subTour[i]][newCage] + matrix.adjMatrix[newCage][subTour[i + 1]] - matrix.adjMatrix[subTour[i]][subTour[i + 1]];
        currentDistance = findDistance(matrix.cageLocation[subTour[i]], matrix.cageLocation[newCage]) + findDistance(matrix.cageLocation[newCage], matrix.cageLocation[subTour[i + 1]]) - findDistance(matrix.cageLocation[subTour[i]], matrix.cageLocation[subTour[i + 1]]);
        if (currentDistance < minDistance){
            minDistance = currentDistance;
            index = i + 1;
        }
    }
    return index;
}


vector<int> makeMST(adjMatrix &matrix){
    vector<int> kv; // visited
    vector<double> dv; // distance
    vector<int> pv; // preceding vertex
    int numCages = matrix.numCages;
    kv.assign(numCages, 0);
    dv.assign(numCages, __DBL_MAX__);
    pv.assign(numCages, 0);
    dv[0] = 0;
    kv[0] = 1;
    // find initial point
    for (int i = 1; i < numCages; ++i){
        if (checkValidtour(matrix.cageLocation[0], matrix.cageLocation[i])){
            dv[i] = findDistance(matrix.cageLocation[0], matrix.cageLocation[i]);
        }
    }
    
    for (int i = 1; i < numCages; ++i){
        int index = 0;
        double minDist = __DBL_MAX__;
        
        
        // finds index and value of min distance, revise this so you dont have to linear search for min distance
        for (int j = 0; j < numCages; ++j){
            if (kv[j] == 0 && dv[j] < minDist){
                index = j;
                minDist = dv[j];
            }
        }
        kv[index] = 1; // mark cage as visited
        
        
        for (int j = 0; j < numCages; ++j){
            // finding distances from new vantage point
            if (kv[j] == 0 && checkValidtour(matrix.cageLocation[index], matrix.cageLocation[j]) && findDistance(matrix.cageLocation[index], matrix.cageLocation[j]) < dv[j]){
                dv[j] = findDistance(matrix.cageLocation[index], matrix.cageLocation[j]);
                pv[j] = index;
            }
        }
        // print while doing algorithm, string stream
        //
        
    }
    return pv;
}

// upperbound for part c
vector<int> makeFASTTSP(adjMatrix &matrix){
    vector<int> kv; // visited
    vector<double> dv; // distance
    int numCages = matrix.numCages;
    kv.assign(numCages, 0);
    dv.assign(numCages, __DBL_MAX__);
    dv[0] = 0;
    kv[0] = 1;
    
    // record distances from origin
    for (int i = 1; i < numCages; ++i){
        //dv[i] = matrix.adjMatrix[0][i];
        dv[i] = findDistance(matrix.cageLocation[0], matrix.cageLocation[i]);
    }
    
    // find first city from origin
    int index = 0;
    double minDist = __DBL_MAX__;
    for (int i = 0; i < numCages; ++i){
        if (kv[i] == 0 && dv[i] < minDist){
            index = i;
            minDist = dv[i];
        }
    }
    kv[index] = 1;
    vector<int> subTour;
    subTour.push_back(0);
    subTour.push_back(index);
    
    int new_cage, index_new_cage;
    int numCagesVisited = 2;
    // loop till cycle is complete
    while (numCagesVisited != matrix.numCages){
        // selection
        new_cage = findCage(kv);
        
        // insertion
        index_new_cage = findMinInsertionCost(new_cage, subTour, matrix);
        subTour.insert(subTour.begin() + index_new_cage, new_cage);
        
        //  mark cage as visited
        kv[new_cage] = 1;
        ++numCagesVisited;
    }
    
    return subTour;
}



double findTotalDistance(vector<int> &tour, adjMatrix &matrix){
    double distance = 0;
    for (int i = 0; i < (int)tour.size() - 1; ++i){
        //distance += matrix.adjMatrix[tour[i]][tour[i + 1]];
        distance += findDistance(matrix.cageLocation[tour[i]], matrix.cageLocation[tour[i + 1]]);
    }
    //distance += matrix.adjMatrix[tour[0]][tour[tour.size() - 1]];
    distance += findDistance(matrix.cageLocation[tour[0]], matrix.cageLocation[tour[tour.size() - 1]]);
    return distance;
}

double findTotalDistAdjMatrix(vector<int> &tour, adjMatrix &matrix){
    double distance = 0;
    for (int i = 0; i < (int)tour.size() - 1; ++i){
        distance += matrix.adjMatrix[tour[i]][tour[i + 1]];
        //distance += findDistance(matrix.cageLocation[tour[i]], matrix.cageLocation[tour[i + 1]]);
    }
    distance += matrix.adjMatrix[tour[0]][tour[tour.size() - 1]];
    //distance += findDistance(matrix.cageLocation[tour[0]], matrix.cageLocation[tour[tour.size() - 1]]);
    return distance;
}


vector<int> modifiedFASTTSP(adjMatrix &matrix){
    vector<int> kv; // visited
    vector<double> dv; // distance
    int numCages = matrix.numCages;
    kv.assign(numCages, 0);
    dv.assign(numCages, __DBL_MAX__);
    dv[0] = 0;
    kv[0] = 1;
    
    // record distances from origin
    for (int i = 1; i < numCages; ++i){
        dv[i] = matrix.adjMatrix[0][i];
        //dv[i] = findDistance(matrix.cageLocation[0], matrix.cageLocation[i]);
    }
    
    // find first city from origin
    int index = 0;
    double minDist = __DBL_MAX__;
    for (int i = 0; i < numCages; ++i){
        if (kv[i] == 0 && dv[i] < minDist){
            index = i;
            minDist = dv[i];
        }
    }
    kv[index] = 1;
    vector<int> subTour;
    subTour.push_back(0);
    subTour.push_back(index);
    
    int new_cage;
    int numCagesVisited = 2;
    // loop till cycle is complete
    while (numCagesVisited != matrix.numCages){
        // selection
        new_cage = findCage(kv);
        
        // insertion
        
        int index = 0;
        double minDistance = __DBL_MAX__;
        double currentDistance;
        for (int i = 0; i < (int)numCagesVisited - 1; ++i){
            currentDistance = matrix.adjMatrix[subTour[i]][new_cage] + matrix.adjMatrix[new_cage][subTour[i + 1]] - matrix.adjMatrix[subTour[i]][subTour[i + 1]];
            //currentDistance = findDistance(matrix.cageLocation[subTour[i]], matrix.cageLocation[new_cage]) + findDistance(matrix.cageLocation[new_cage], matrix.cageLocation[subTour[i + 1]]) - findDistance(matrix.cageLocation[subTour[i]], matrix.cageLocation[subTour[i + 1]]);
            if (currentDistance < minDistance){
                minDistance = currentDistance;
                index = i + 1;
            }
        }
        
        subTour.insert(subTour.begin() + index, new_cage);
        
        //  mark cage as visited
        kv[new_cage] = 1;
        ++numCagesVisited;
    }
    
    return subTour;
}

vector<int> modifiedMST(deque<int> unvisited, adjMatrix &matrix){
    vector<int> kv; // visited
    vector<double> dv; // distance
    vector<int> pv; // preceding vertex
    deque<int> temp = unvisited;
    int numCages = int(unvisited.size());
    kv.assign(numCages, 0);
    dv.assign(numCages, __DBL_MAX__);
    pv.assign(numCages, 0);
    kv[0] = 1;
    dv[0] = 0;
    // find initial point
    for (int i = 1; i < numCages; ++i){
        //dv[i] = findDistance(matrix.cageLocation[0], matrix.cageLocation[i]);
        dv[i] = matrix.adjMatrix[unvisited[0]][unvisited[i]];
    }
    
    for (int i = 1; i < numCages; ++i){
        int index = 0;
        double minDist = __DBL_MAX__;
        
        
        // finds index and value of min distance, revise this so you dont have to linear search for min distance
        for (int j = 0; j < numCages; ++j){
            if (kv[j] == 0 && dv[j] < minDist){
                index = j;
                minDist = dv[j];
            }
        }
        kv[index] = 1; // mark cage as visited
        
        
        for (int j = 0; j < numCages; ++j){
            // finding distances from new vantage point
            if (kv[j] == 0 && matrix.adjMatrix[unvisited[index]][unvisited[j]] < dv[j]){
                dv[j] = matrix.adjMatrix[unvisited[index]][unvisited[j]];
                //int temp = unvisited[index];
                pv[j] = index;
            }
        }
        // print while doing algorithm, string stream
        //
        
    }
    return pv;
}

bool promising(vector<int> &currentPath, deque<int> unvisited, adjMatrix &matrix){
    // 2. promising function
    // have current path (weight) and the rest of nodes
    // use prims on unvisited nodes
    // now you have path and mst
    // connect them by finding the closest node btw mst and path
    // with this weight compare to current upper bound
    vector<int> unvisitedMST = modifiedMST(unvisited, matrix);
    //deque<int> temp = unvisited;
    double startMin = __DBL_MAX__;
    double endMin = __DBL_MAX__;
    // find cage closest to starting point and end point of current path
    for (int i = 0; i < (int)unvisited.size(); ++i){
        if (matrix.adjMatrix[currentPath[currentPath.size() - 1]][unvisited[i]] < endMin){
            endMin = matrix.adjMatrix[currentPath[currentPath.size() - 1]][unvisited[i]];
        }
        if (matrix.adjMatrix[currentPath[0]][unvisited[i]] < startMin){
            startMin = matrix.adjMatrix[currentPath[0]][unvisited[i]];
        }
    }
    // find total distance of MST
    double mstTotalDistance = 0;
    for (int i = 1; i < (int)unvisitedMST.size(); ++i){
        mstTotalDistance += matrix.adjMatrix[unvisited[i]][unvisited[unvisitedMST[i]]];
    }
    // comparing path distance with upper bound
    double lowerBound = currentTotalDist + mstTotalDistance + endMin + startMin;
    if (lowerBound < upperBound){
        return true;
    }
    else {
        return false;
    }
}

void makeOPTTSP(adjMatrix &matrix, vector<int> &currentPath, deque<int> &unvisited, vector<int> &bestPath){
    
    // 1. part B answer for initial upper bound
    // fasttsp is close to optimal path
    if (unvisited.empty()){
        // compare length of currentpath to upper bound
        // if it is lower update upperbound as well as the best path itself
        if (currentTotalDist + matrix.adjMatrix[currentPath[0]][currentPath[currentPath.size() - 1]] < upperBound){
            upperBound = currentTotalDist + matrix.adjMatrix[currentPath[0]][currentPath[currentPath.size() - 1]];
            bestPath = currentPath;
        }
        return;
    }
    if (!promising(currentPath, unvisited, matrix)){
        return;
    }
    for (int i = 0; i < (int)unvisited.size(); ++i){
        currentTotalDist += matrix.adjMatrix[currentPath[currentPath.size() - 1]][unvisited.front()];
        currentPath.push_back(unvisited.front());
        unvisited.pop_front();
        makeOPTTSP(matrix, currentPath, unvisited, bestPath);
        unvisited.push_back(currentPath.back());
        currentTotalDist -= matrix.adjMatrix[currentPath[currentPath.size() - 1]][currentPath[currentPath.size() - 2]];
        currentPath.pop_back();
    }
    
    
    // 3. if the weight is < upperbound, update upperbound
    
    
}

void printFASTTSP(const adjMatrix &matrix, const vector<int>& mst){
    double distance = 0;
    int cage1, cage2;
    for (int i = 0; i < matrix.numCages - 1; ++i){
        cage1 = mst[i];
        cage2 = mst[i + 1];
        //distance += matrix.adjMatrix[cage1][cage2];
        distance += findDistance(matrix.cageLocation[cage1], matrix.cageLocation[cage2]);
    }
    //distance += matrix.adjMatrix[0][mst[matrix.numCages - 1]];
    distance += findDistance(matrix.cageLocation[0], matrix.cageLocation[mst[matrix.numCages - 1]]);
    cout << setprecision(2);
    cout << fixed;
    cout << distance << '\n';
    cout << mst[0];
    for (int i = 1; i < matrix.numCages; ++i){
        cout << " " << mst[i];
    }
}

void printMST(adjMatrix &matrix, vector<int> &tour){
    double distance = 0;
    for (int i = 1; i < matrix.numCages; ++i){
        distance += findDistance(matrix.cageLocation[i], matrix.cageLocation[tour[i]]);
    }
    cout << setprecision(2);
    cout << fixed;
    cout << distance << '\n';
    for (int i = 1; i < matrix.numCages; ++i){
        if (tour[i] < i){
            cout << tour[i] << " " << i << '\n';
        }
        else {
            cout << i << " " << tour[i] << '\n';
        }
    }
}

void printOPTTSP(adjMatrix &matrix, vector<int> &bestPath){
    double distance = 0;
    for (int i = 0; i < (int)bestPath.size() - 1; ++i){
        distance += matrix.adjMatrix[bestPath[i]][bestPath[i + 1]];
    }
    distance += matrix.adjMatrix[0][bestPath[bestPath.size() - 1]];
    cout << setprecision(2);
    cout << fixed;
    cout << distance << '\n';
    cout << bestPath[0];
    for (int i = 1; i < (int)bestPath.size(); ++i){
        cout << " " << bestPath[i];
    }
}

int main(int argc, char * argv[]) {
#ifdef __APPLE__
    freopen("test-2-OPTTSP.txt", "r", stdin);
#endif
    option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"mode", 1, nullptr, 'm'},
    };
    int c;
    adjMatrix matrix;
    while ((c = getopt_long(argc, argv, "hm:", long_options, NULL)) != -1){
        switch(c){
            case 'h':
                cout << "help message" << '\n';
                exit(0);
            case 'm':
                matrix.mode = string(optarg);
                break;
        }
    }
    cin >> matrix.numCages;
    double x, y;
    Location temp;
    while (cin >> x >> y){
        // tame = 0, wild = 1, border = 2
        temp.x = x;
        temp.y = y;
        if (x < 0 && y < 0){
            temp.type = 1;
        }
        else if ((x <= 0 && y == 0) || (x == 0 && y <= 0)){
            temp.type = 2;
        }
        else {
            temp.type = 0;
        }
        matrix.cageLocation.push_back(temp);
    }
    if (matrix.mode == "MST"){
        vector<int> mst = makeMST(matrix);
        printMST(matrix, mst);
    }
    else {
        // clear unvisitied memory
        //matrix.cageLocation.clear();
        
        if (matrix.mode == "FASTTSP"){
            vector<int> mst = makeFASTTSP(matrix);
            printFASTTSP(matrix, mst);
        }
        // mdoe is OTTSP
        else {
            // adjacency matrix bc theres not many cages
            for (int i = 0; i < matrix.numCages; ++i){
                vector<double> temp;
                for (int j = 0; j < matrix.numCages; ++j){
                    temp.push_back(findDistance(matrix.cageLocation[i], matrix.cageLocation[j]));
                }
                matrix.adjMatrix.push_back(temp);
            }
            // part A is lower bound
            // part B is intitial upper bound
            vector<int> fastPath = modifiedFASTTSP(matrix);
            upperBound = findTotalDistAdjMatrix(fastPath, matrix);
            
            vector<int> optimalPath;
            optimalPath.push_back(0);
            deque<int> unvisited;
            //unvisited.resize(matrix.numCages);
            for (int i = 1; i < (int)fastPath.size(); ++i){
                unvisited.push_back(fastPath[i]);
            }
            vector<int> bestPath;
            bestPath = fastPath;
            makeOPTTSP(matrix, optimalPath, unvisited, bestPath);
            printOPTTSP(matrix, bestPath);
        }
    }
    

    return 0;
}
