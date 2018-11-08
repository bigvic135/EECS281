//
//  PuzzleSolver.cpp
//  Project1
//
//  Created by BigVic on 9/13/17.
//  Copyright © 2017 mac. All rights reserved.
//

#include "PuzzleSolver.h"
using namespace std;
int num_colors, width, height, startRow, startCol, endx, endy, endFloor, startCount, endCount;
string runMode, outputMode;

int main(int argc, char *argv[]){
    
#ifdef __APPLE__
    freopen("big.txt", "r", stdin);
#endif
    
    outputMode = getOptions(argc, argv);
    ThreeDMap threeDmap;
    TwoDMap twoDmap = makeMap();
    threeDmap.resize(num_colors + 1);
    vector<char> colVector;
    colVector.resize(width);
    for (int i = 0; i < width; ++i) {
        colVector[i] = '.';
    }
    vector<vector<char>> rowVector;
    rowVector.resize(height);
    for (int i = 0; i < height; ++i) {
        rowVector[i] = colVector;
    }
    for (int i = 0; i < num_colors + 1; ++i) {
        threeDmap[i] = rowVector;
    }
    findEscape(threeDmap, twoDmap);
    
    // if then statement with map output mode
}
// command line flags then read in map input
// then plan and proceed with the algorithm and create necessary data structures.
string getOptions(int argc, char* argv[]){
    option long_options[]={
        {"help", no_argument, nullptr, 'h'},
        {"queue", no_argument, nullptr, 'q'},
        {"stack", no_argument, nullptr, 's'},
        {"output", required_argument, nullptr, 'o'},
        {nullptr, 0, nullptr, '\0'}
    };
    int choice;
    int runCount = 0;
    bool queue, stack, output;
    string outputMode;
    queue = stack = output = false;
    int index = 0;
    while ((choice = getopt_long(argc, argv, "hqso:", long_options, &index)) != - 1){
        switch(choice){
            case 'h':
                cout << "help message";
                exit(0);
                // exactly one of stack and queue are provided
                // no invalid command line flags
            case 'q': // type of collection used
                if (runCount == 0){
                    queue = true;
                    ++runCount;
                }
                else{
                    cerr << "More than one stack or queue\n";
                    exit(1);
                }
                break;
            case 's': // type of collection used
                if (runCount == 0){
                    stack = true;
                    ++runCount;
                }
                else{
                    cerr << "More than one stack or queue\n";
                    exit(1);
                }
                break;
            case 'o':
                outputMode = optarg;
                if (outputMode != "map" && outputMode != "list" && outputMode != ""){
                    cerr << "Invalid output option" << '\n';
                    exit(1);
                }
                // check for if there is no argument
                output = true;
                break;
            default:
                cerr << "Error" << '\n';
                exit(1);
        }
        // if no output mode is given continue as map
    }
    if (!queue && !stack){
        cerr << "Invalid! No route specified" << '\n';
        exit(1);
    }
    if (queue){
        runMode = "queue";
    }
    else {
        runMode = "stack";
    }
    return outputMode;
}


bool validInput(char symbol){
    if (!(symbol == '@' || symbol == '?' || symbol == '.' || symbol == '#' || symbol == '^' || isalpha(symbol))){
        cerr << "valid Input not working" << '\n';
        exit(1);
    }
    if (startCount > 1 || endCount > 1){
        cerr << "Too many starts or finishes" << '\n';
        exit(1);
    }
    if (isalpha(symbol) && int(symbol) > num_colors + 96){
        cerr << "Letter above number of floors" << '\n';
        exit(1);
    }
    else {
        if (symbol == '@'){
            ++startCount;
        }
        if (symbol == '?'){
            ++endCount;
        }
        return true;
    }
    // only one @, only one ?, A - Z and a - z, ^
    // letters have to be lower than the number of colors
    // ^ is always valid even if number of colors is 0
    // 0 <= num colors <= 26
    // no invalid characters
    // width and height both must be at least 1
    // puzzle with no solution can still be valid input
    // redo and name valid input or make a new function named valid input and include this
    // number of rooms is number of colors + 1
}
/*
 bool validLocation(int row, int col, int color, ){
 if (row >= map.height || col >= map.width || color != map.num_colors + 1){ // redo to check for more errors
 return false;
 }
 else {
 return true;
 }
 }*/


vector<vector<char> > makeMap(){
    cin >> num_colors >> width >> height;
    if (num_colors < 0 || num_colors > 26 || width < 1 || height < 1){
        cerr << "Invalid Map Input first three" << '\n';
        exit(1);
    }
    cin.ignore(1);
    string mapInput;
    //int mapMaxSize = map.width * map.height;
    int startCount, endCount;
    startCount = endCount = 1;
    // makes 2d vector with map.height as size of rows and map.width as size of cols and fills with '.'
    int i = 0;
    TwoDMap twoDmap(height, vector<char>(width, '.'));
    while(getline(cin, mapInput)){
        if (mapInput[0] == '/' && mapInput[1] == '/'){
            continue;
        }
        else if(mapInput[0] == ' ' || mapInput[0] == '\0') {
            break;
        }
        int index = 0;
        for (int j = 0; j < width; ++j){
            if (validInput(mapInput[index])){
                if (mapInput[index] == '@' && startCount == 1){
                    twoDmap[i][j] = mapInput[index];
                    ++startCount;
                    startRow = i;
                    startCol = j;
                }
                else if (mapInput[index] == '?' && endCount == 1){
                    twoDmap[i][j] = mapInput[index];
                    ++endCount;
                    endx = i;
                    endy = j;
                }
                else{
                    twoDmap[i][j] = mapInput[index];
                }
                ++index;
            }
            else {
                exit(1);
            }
        }
        ++i;
    }
    return twoDmap;
}

bool reachablePoint(TwoDMap &twoDmap, int direction, State &temp){
    if (direction == 0){
        if (temp.color == '^' && !isupper(twoDmap[temp.row - 1][temp.col])){
            return true;
        }
        else {
            if (temp.color != '^' && (twoDmap[temp.row - 1][temp.col]) == toupper(temp.color)){
                return true;
            }
            if (temp.color != '^' && !isupper(twoDmap[temp.row - 1][temp.col])){
                return true;
            }
        }
        return false;
    }
    if (direction == 1){
        if (temp.color == '^' && !isupper(twoDmap[temp.row][temp.col + 1])){
            return true;
        }
        else {
            if (temp.color != '^' && (twoDmap[temp.row][temp.col + 1]) == toupper(temp.color)){
                return true;
            }
            if (temp.color != '^' && !isupper(twoDmap[temp.row][temp.col + 1])){
                return true;
            }
        }
        return false;
    }
    if (direction == 2){
        if (temp.color == '^' && !isupper(twoDmap[temp.row + 1][temp.col])){
            return true;
        }
        else {
            if (temp.color != '^' && (twoDmap[temp.row + 1][temp.col]) == toupper(temp.color)){
                return true;
            }
            if (temp.color != '^' && !isupper(twoDmap[temp.row + 1][temp.col])){
                return true;
            }
        }
        return false;
    }
    if (direction == 3){
        if (temp.color == '^' && !isupper(twoDmap[temp.row][temp.col - 1])){
            return true;
        }
        else {
            if (temp.color != '^' && (twoDmap[temp.row][temp.col - 1]) == toupper(temp.color)){
                return true;
            }
            if (temp.color != '^' && !isupper(twoDmap[temp.row][temp.col - 1])){
                return true;
            }
        }
        return false;
    }
    return false;
}

void findEscape(ThreeDMap &threeDmap, TwoDMap &twoDmap){
    //threeDmap.resize(num_colors);
    // resize num states + 1
    // 1d with col
    // push that to 2d
    // then push that to 3d
    // fix it so its always push back
    deque<State> collection;
    int solution = 0;
    State start = {'^', startRow, startCol};
    State nextState;
    // step 2
    collection.push_back(start);
    threeDmap[0][startRow][startCol] = '@';
    collection.front() = start;
    State temp;
    // step 3
    while (!collection.empty()){
        // step 4
        if (runMode == "stack"){
            temp = collection.back();
            collection.pop_back();
            //cout << temp.color << " " << temp.row << " " << temp.col << '\n';
        }
        else {
            temp = collection.front();
            collection.pop_front();
        }
        // step 5
        if (((twoDmap[temp.row][temp.col] >= 97 && twoDmap[temp.row][temp.col] <= 122) || (twoDmap[temp.row][temp.col] == '^'))
            && (twoDmap[temp.row][temp.col] != temp.color)){
            // if trap
            if (twoDmap[temp.row][temp.col] == '^'){
                nextState = {'^', temp.row, temp.col};
                // check to see if map has not been reached
                if (!((threeDmap[0][nextState.row][nextState.col] >= 48 && (threeDmap[0][nextState.row][nextState.col] <= 51)) ||
                      (threeDmap[0][nextState.row][nextState.col] >= 97 && (threeDmap[0][nextState.row][nextState.col] <= 122)))){
                    // record where you came from
                    threeDmap[0][temp.row][temp.col] = temp.color;
                    collection.push_back(nextState);
                    //temp = nextState;
                }
            }
            //if button
            else  {
                char newSymbol = twoDmap[temp.row][temp.col];
                int floor = int (newSymbol) - 96;
                nextState = {newSymbol, temp.row, temp.col};
                // check to see if you have already visited the button
                if (threeDmap[floor][nextState.row][nextState.col] != '^' &&
                    !(threeDmap[floor][nextState.row][nextState.col] >= 48 && threeDmap[floor][nextState.row][nextState.col] <= 51) &&
                    !(threeDmap[floor][nextState.row][nextState.col] >= 97 && threeDmap[floor][nextState.row][nextState.col] <= 122)) {
                    threeDmap[floor][nextState.row][nextState.col] = temp.color;
                    collection.push_back(nextState);
                }
            }
        }
        else {
            int floor;
            if (temp.color == '^'){
                floor = 0;
            }
            else {
                floor = int(temp.color) - 96;
            }
            if (temp.row - 1 >= 0 && twoDmap[temp.row - 1][temp.col] != '#'){
                // first check if there is a letter or 0-4, if there is deny
                // then check for uppercase letters, if a letter is uppercase and temp.color is not equal then deny
                if (reachablePoint(twoDmap, 0, temp)){
                    char newSymbol = threeDmap[floor][temp.row - 1][temp.col];
                    if((newSymbol < 48 || newSymbol > 51) && newSymbol != '^' && newSymbol != '@'
                       && (newSymbol < 97 || newSymbol > 122)) {
                        nextState = {temp.color, temp.row - 1, temp.col};
                        threeDmap[floor][nextState.row][nextState.col] = '2';
                        if (twoDmap[temp.row - 1][temp.col] == '?'){
                            endx = nextState.row;
                            endy = nextState.col;
                            endFloor = floor;
                            ++solution;
                        }
                        collection.push_back(nextState);
                    }
                    
                }
                
            }
            // east
            if (temp.col + 1 < width && twoDmap[temp.row][temp.col + 1] != '#'){
                // first check if there is a letter or 0-4, if there is deny
                // then check for uppercase letters, if a letter is uppercase and temp.color is not equal then deny
                if (reachablePoint(twoDmap, 1, temp)){
                    char newSymbol = threeDmap[floor][temp.row][temp.col + 1];
                    if((newSymbol < 48 || newSymbol > 51) && newSymbol != '^' && newSymbol != '@'
                       && (newSymbol < 97 || newSymbol > 122)) {
                        nextState = {temp.color, temp.row, temp.col + 1};
                        threeDmap[floor][nextState.row][nextState.col] = '3';
                        if (twoDmap[temp.row][temp.col + 1] == '?'){
                            endx = nextState.row;
                            endy = nextState.col;
                            endFloor = floor;
                            ++solution;
                        }
                        collection.push_back(nextState);
                    }
                }
            }
            // south
            if (temp.row + 1 < height && twoDmap[temp.row + 1][temp.col] != '#'){
                // first check if there is a letter or 0-4, if there is deny
                // then check for uppercase letters, if a letter is uppercase and temp.color is not equal then deny
                if (reachablePoint(twoDmap, 2, temp)){
                    char newSymbol = threeDmap[floor][temp.row + 1][temp.col];
                    if((newSymbol < 48 || newSymbol > 51) && newSymbol != '^' && newSymbol != '@'
                       && (newSymbol < 97 || newSymbol > 122)) {
                        nextState = {temp.color, temp.row + 1, temp.col};
                        threeDmap[floor][nextState.row][nextState.col] = '0';
                        if (twoDmap[temp.row + 1][temp.col] == '?'){
                            endx = nextState.row;
                            endy = nextState.col;
                            endFloor = floor;
                            ++solution;
                        }
                        collection.push_back(nextState);
                    }
                }
                
            }
            // west
            if (temp.col - 1 >= 0 && twoDmap[temp.row][temp.col - 1] != '#'){
                // first check if there is a letter or 0-4, if there is deny
                // then check for uppercase letters, if a letter is uppercase and temp.color is not equal then deny
                if (reachablePoint(twoDmap, 3, temp)){
                    char newSymbol = threeDmap[floor][temp.row][temp.col - 1];
                    if((newSymbol < 48 || newSymbol > 51) && newSymbol != '^' && newSymbol != '@'
                       && (newSymbol < 97 || newSymbol > 122)) {
                        nextState = {temp.color, temp.row, temp.col - 1};
                        threeDmap[floor][nextState.row][nextState.col] = '1';
                        if (twoDmap[temp.row][temp.col - 1] == '?'){
                            endx = nextState.row;
                            endy = nextState.col;
                            endFloor = floor;
                            ++solution;
                        }
                        collection.push_back(nextState);
                    }
                }
                
            }
            // step 6 not a button
            // north
        }
    }
    if (solution == 0){
        noSolution(threeDmap, twoDmap);
    }
    else {
        if (outputMode == "" || outputMode == "map"){
            mapOutput(threeDmap, twoDmap);
        }
        else {
            listOutput(threeDmap, twoDmap);
        }
    }
}

void noSolution(ThreeDMap &threeDmap, TwoDMap &twoDmap){
    cout << "No solution." << '\n';
    cout << "Reachable:" << '\n';
    // back trace and i && i - 2
    vector<vector<char>> noSolution(height, vector<char>(width, '!'));
    for (int i = 0; i < num_colors + 1; ++i){
        for (int j = 0; j < height; ++j) {
            for (int k = 0; k < width; ++k) {
                if (threeDmap[i][j][k] != '.'){
                    noSolution[j][k] = '+';
                }
                else{
                    if (noSolution[j][k] != '+'){
                        noSolution[j][k] = '#';
                    }
                }
            }
        }
    }
    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j) {
            if (noSolution[i][j] == '+'){
                cout << twoDmap[i][j];
            }
            else {
                cout << noSolution[i][j];
            }
        }
        cout << '\n';
    }
}

void mapOutput(ThreeDMap &threeDmap, TwoDMap &twoDmap){
    /*
    int i, j, k;
    i = endFloor;
    j = endx;
    k = endy;
    while (threeDmap[i][j][k] != '@'){
        if (threeDmap[i][j][k] == '0'){
            threeDmap[i][j][k] = '+';
            --j;
        }
        else if (threeDmap[i][j][k] == '1'){
            threeDmap[i][j][k] = '+';
            ++k;
        }
        else if (threeDmap[i][j][k] == '2'){
            threeDmap[i][j][k] = '+';
            ++j;
        }
        else if (threeDmap[i][j][k] == '3'){
            threeDmap[i][j][k] = '+';
            --k;
        }
        else if (isalpha(threeDmap[i][j][k])){
            char temp = threeDmap[i][j][k];
            threeDmap[i][j][k] = '+';
            if (temp == 'a'){
                i = 1;
            }
            else {
                i = int(temp) - 96;
            }
        }
        else if (threeDmap[i][j][k] == '^'){
            threeDmap[i][j][k] = '+';
            i = 0;
        }
    }
    
    for (int i = 0; i < num_colors; ++i){
        for (int j = 0; j < height; ++j){
            for (int k = 0; k < width; ++k){
                if (threeDmap[i][j][k] != '+' && threeDmap[i][j][k] != '%' && threeDmap[i][j][k] != '@'){
                    if (twoDmap[j][k] == toupper((changefloortoColor(i)) && isupper(twoDmap[j][k]))){
                        threeDmap[i][j][k] = '.';
                    }
                    else if (twoDmap[j][k] == '^' && changefloortoColor(i) == '^'){
                        threeDmap[i][j][k] = '.';
                    }
                    else if (twoDmap[j][k] == '@' && changefloortoColor(i) != '^'){
                        
                    }
                    else if (twoDmap[j][k] == changefloortoColor(i)){
                        threeDmap[i][j][k] = '.';
                    }
                    else if (threeDmap[i][j][k] == '0' || threeDmap[i][j][k] == '1' || threeDmap[i][j][k] == '2' || threeDmap[i][j][k] == '3'){
                        threeDmap[i][j][k] = twoDmap[j][k];
                    }
                }
            }
        }
    }
    string map;
    map.resize(width + 1);
    for(int i = 0; i < num_colors + 1; ++i){
        cout << "// color " << changefloortoColor(i) << " \n";
        for(size_t j = 0; j < height; ++j){
            map = "";
            for(size_t k = 0; k < width; ++k){
                map += threeDmap[i][j][k];
            }
            map += "\n";
            cout << map;
        }
    }
     */
    char temp;
    deque<State> tempCollection;
    int i, j, k;
    i = endFloor;
    j = endx;
    k = endy;
    State current;
    while(threeDmap[i][j][k] != '@'){
        current = {'-', 0, 0};
        if (threeDmap[i][j][k] == '0'){
            current = {changefloortoColor(i), j, k};
            tempCollection.push_front(current);
            if(twoDmap[j][k] == '?'){
                threeDmap[i][j][k] = '?';
            }
            else if((islower(twoDmap[j][k]) && twoDmap[j][k] != changefloortoColor(i)) || (twoDmap[j][k] == '^' && changefloortoColor(i) != '^')){
                threeDmap[i][j][k] = '%';
            }
            else {
                threeDmap[i][j][k] = '+';
            }
            --j;
        }
        else if(threeDmap[i][j][k] == '1'){
            current = {changefloortoColor(i), j, k};
            tempCollection.push_front(current);
            if(twoDmap[j][k] == '?'){
                threeDmap[i][j][k] = '?';
            }
            else if((islower(twoDmap[j][k]) && twoDmap[j][k] != changefloortoColor(i)) || (twoDmap[j][k] == '^' && changefloortoColor(i) != '^')){
                threeDmap[i][j][k] = '%';
            }
            else {
                threeDmap[i][j][k] = '+';
            }
            ++k;
        }
        else if(threeDmap[i][j][k] == '2'){
            current = {changefloortoColor(i), j, k};
            tempCollection.push_front(current);
            if(twoDmap[j][k] == '?'){
                threeDmap[i][j][k] = '?';
            }
            else if((islower(twoDmap[j][k]) && twoDmap[j][k] != changefloortoColor(i)) || (twoDmap[j][k] == '^' && changefloortoColor(i) != '^')){
                threeDmap[i][j][k] = '%';
            }
            else {
                threeDmap[i][j][k] = '+';
            }
            ++j;
        }
        else if(threeDmap[i][j][k] == '3'){
            current = {changefloortoColor(i), j, k};
            tempCollection.push_front(current);
            if(twoDmap[j][k] == '?'){
                threeDmap[i][j][k] = '?';
            }
            else if((islower(twoDmap[j][k]) && twoDmap[j][k] != changefloortoColor(i)) || (twoDmap[j][k] == '^' && changefloortoColor(i) != '^')){
                threeDmap[i][j][k] = '%';
            }
            else {
                threeDmap[i][j][k] = '+';
            }
            --k;
        }
        else if((threeDmap[i][j][k] >= 97 && threeDmap[i][j][k] <= 122) || threeDmap[i][j][k] == '^'){ //if prev is a button, go to the corresponding floor
            current = {changefloortoColor(i), j, k};
            tempCollection.push_front(current);
            temp = threeDmap[i][j][k];
            threeDmap[i][j][k] = '@';
            if(temp == '^'){
                i = 0;
            } else {
                i = int(temp) - 96;
            }
        }
    }
    tempCollection.push_front(current);
    threeDmap[0][startRow][startCol] = '@';
    
    // not needed for list
    for (int i = 0; i < num_colors + 1; ++i){
        for (int j = 0; j < height; ++j){
            for (int k = 0; k < width; ++k){
                if (threeDmap[i][j][k] != '+' && threeDmap[i][j][k] != '%' && threeDmap[i][j][k] != '@'){
                    // fill uppercase letters with dots if on same floor
                    if (twoDmap[j][k] == toupper(changefloortoColor(i)) && isupper(twoDmap[j][k])){
                        threeDmap[i][j][k] = '.';
                    }
                    // change traps to dots if on first floor
                    else if (twoDmap[j][k] == '^' && changefloortoColor(i) == '^'){
                        threeDmap[i][j][k] = '.';
                    }
                    // change start sign to dots if not on first floor
                    else if (twoDmap[j][k] == '@' && changefloortoColor(i) != '^'){
                        threeDmap[i][j][k] = '.';
                    }
                    //
                    else if (twoDmap[j][k] == changefloortoColor(i)){
                        threeDmap[i][j][k] = '.';
                    }
                    else {
                        threeDmap[i][j][k] = twoDmap[j][k];
                    }
                }
            }
        }
    }

    
    string output;
    output.resize(width + 1);
    for (int i = 0; i < num_colors + 1; ++i){
        cout << "// color " << changefloortoColor(i) << '\n';
        for (int j = 0; j < height; ++j) {
            output = "";
            for (int k = 0; k < width; ++k){
                output = output + threeDmap[i][j][k];
            }
            output = output + '\n';
            cout << output;
        }
    }

}


char changefloortoColor(int floor){
    char color;
    if (floor == 0){
        color = '^';
    }
    else {
        color = char(floor + 96);
    }
    return color;
}

void listOutput(ThreeDMap &threeDmap, TwoDMap &twoDmap){
    char temp;
    State tempOutput;
    deque<State> tempCollection;
    int i, j, k;
    i = endFloor;
    j = endx;
    k = endy;
    State current;
    while(threeDmap[i][j][k] != '@'){
        current = {'-', 0, 0};
        if (threeDmap[i][j][k] == '0'){
            current = {changefloortoColor(i), j, k};
            tempCollection.push_front(current);
            --j;
        }
        else if(threeDmap[i][j][k] == '1'){
            current = {changefloortoColor(i), j, k};
            tempCollection.push_front(current);
            ++k;
        }
        else if(threeDmap[i][j][k] == '2'){
            current = {changefloortoColor(i), j, k};
            tempCollection.push_front(current);
            ++j;
        }
        else if(threeDmap[i][j][k] == '3'){
            current = {changefloortoColor(i), j, k};
            tempCollection.push_front(current);
            --k;
        }
        else if((threeDmap[i][j][k] >= 97 && threeDmap[i][j][k] <= 122) || threeDmap[i][j][k] == '^'){ //if prev is a button, go to the corresponding floor
            current = {changefloortoColor(i), j, k};
            tempCollection.push_front(current);
            temp = threeDmap[i][j][k];
            if(temp == '^'){
                i = 0;
            } else {
                i = int(temp) - 96;
            }
        }
    }
    current = {changefloortoColor(i), j, k};
    tempCollection.push_front(current);
    threeDmap[0][startRow][startCol] = '@';
    tempOutput = tempCollection.front();
    while (tempCollection.size() != 0){
        cout << "(" << tempOutput.color << ", (" << tempOutput.col << ", " << tempOutput.row << "))" << '\n';
        tempCollection.pop_front();
        tempOutput = tempCollection.front();
    }
}
