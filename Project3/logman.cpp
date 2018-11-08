//
//  main.cpp
//  Project3
//
//  Created by BigVic on 10/30/17.
//  Copyright © 2017 mac. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <utility>
#include <iomanip>
#include <deque>
#include <locale>

//using namespace std::chrono;
using namespace std;

bool searchOccurred = false;

struct Log{
    long long timestamp;
    string semicolTimestamp;
    string category;
    string messages;
    int entryId;
    
};


// this is wrong category comparison
/*
struct TimeCompare{
    // is this making another copy of the pointer?
    bool operator()(const Log* log1, const Log* log2) const{
        // returns true if log1 if less than log 2
        if (log1->timestamp < log2->timestamp){
            return true;
        }
        // else if check other way for timestamp
        else if (log1->timestamp > log2->timestamp){
            return false;
        }
        // this would be else
        else {
            if (log1->category < log2->category){
                return true;
            }
            else if (log1->category > log2->category){
                return false;
            }
            // category other way
            else {
                if (log1->entryId < log2->entryId){
                    return true;
                }
                else {
                    return false;
                }
            }
            // else compare entry Id
        }
    }
    // comapre there sorted ID since the master Log is already sorted
};
 */



struct masterCompare{
    // is this making another copy of the pointer?
    bool operator()(const Log &log1, const Log &log2) const{
        // returns true if log1 if less than log 2
        if (log1.timestamp < log2.timestamp){
            return true;
        }
        // else if check other way for timestamp
        else if (log1. timestamp > log2.timestamp){
            return false;
        }
        // this would be else
        else {
            if (log1.category < log2.category){
                return true;
            }
            else if (log1.category > log2.category){
                return false;
            }
            // category other way
            else {
                if (log1.entryId < log2.entryId){
                    return true;
                }
                else {
                    return false;
                }
            }
            // else compare entry Id
        }
    }
};


string convertToLower(string input){
    locale loc;
    for (size_t i = 0; i < input.size(); ++i){
        input[i] = tolower(input[i], loc);
    }
    return input;
}
/*
 int customLowerBound(vector<long long> &vec, long long val){
 int left = 0;
 int right = (int)vec.size();
 while (left < right) {
 int mid = (left + right) / 2;
 if (val <= vec[mid]) {
 right = mid;
 }
 else {
 left = mid + 1;
 }
 }
 return left;
 }
 */
// something wrong

void timestampSearch(long long timestamp1, long long timestamp2, vector<Log> &masterLog, vector<int> &recentSearch){
    //high_resolution_clock::time_point t1 = high_resolution_clock::now();
    int left = 0;
    int right = (int)masterLog.size();
    while (left < right) {
        int mid = (left + right) / 2;
        if (timestamp1 <= masterLog[mid].timestamp) {
            right = mid;
        }
        else {
            left = mid + 1;
        }
    }
    if (left == (int)masterLog.size()){
        cout << "Timestamps search: 0 entries found" << '\n';
        return;
    }
    else {
        while (left <= (int)masterLog.size() - 1 && masterLog[left].timestamp <= timestamp2){
            recentSearch.push_back(left);
            ++left;
        }
        cout << "Timestamps search: " << recentSearch.size() << " entries found" << '\n';
    }
    //high_resolution_clock::time_point t2 = high_resolution_clock::now();
    //auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    //++left;
    // takes 0.002739 sec
    
}


void matchingSearch(long long timestamp, vector<Log> &masterLog, vector<int> &recentSearch){
    int left = 0;
    int right = (int)masterLog.size();
    while (left < right) {
        int mid = (left + right) / 2;
        if (timestamp <= masterLog[mid].timestamp) {
            right = mid;
        }
        else {
            left = mid + 1;
        }
    }
    if (left == (int)masterLog.size()){
        cout << "Timestamps search: 0 entries found" << '\n';
        return;
    }
    else {
        /*
        for (size_t i = 0; i < timeMap[timestamp].size(); ++i){
            recentSearch.push_back(timeMap[timestamp][i]);
        }
         */ //6496094453
        while (left < (int)masterLog.size() && masterLog[left].timestamp == timestamp){
            recentSearch.push_back(left);
            ++left;
        }
        cout << "Timestamp search: " << recentSearch.size() << " entries found" << '\n';
    }
}
// something wrong
void categorySearch(string input, unordered_map<string, vector<int>> &categoryMap, vector<int> &recentSearch){
    // convert string into fingerprint
    // Doesnt account for lower case letters
    //string temp = convertToLower(input);
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    auto iter = categoryMap.find(input);
    if (iter == categoryMap.end()){
        cout << "Category search: 0 entries found" << '\n';
    }
    else {
        for (size_t i = 0; i < categoryMap[input].size(); ++i){
            recentSearch.push_back(categoryMap[input][i]);
            // still have to write
        }
        cout << "Category search: " << categoryMap[input].size() << " entries found" << '\n';
    }
}

// something wrong
void keywordSearch(unordered_map<string, vector<int>> &messageMap, vector<string> &keywords, vector<int> &recentSearch){
    // unordered map with string as key, vector of entry ID as value
    vector<int> intersect;
    for (size_t i = 0; i < keywords.size(); ++i){
        auto iter = messageMap.find(keywords[i]);
        if (iter == messageMap.end()){
            cout << "Keyword search: 0 entries found" << '\n';
            return;
        }
        else {
            if (intersect.empty() && i == 0){
                auto iter2 = messageMap[keywords[i]].begin();
                while (iter2 != messageMap[keywords[i]].end()){
                    intersect.push_back(*iter2);
                    ++iter2;
                }
            }
            else {
                vector<int> temp;
                auto first1 = intersect.begin();
                auto first2 = messageMap[keywords[i]].begin();
                while (first1 != intersect.end() && first2 != messageMap[keywords[i]].end()){
                    if (*first1 < *first2){
                        ++first1;
                    }
                    else if (*first1 > *first2){
                        ++first2;
                    }
                    else {
                        temp.push_back(*first1);
                        ++first1;
                        ++first2;
                    }
                }
                intersect = temp;
            }
        }
    }
    cout << "Keyword search: " << intersect.size() << " entries found" << '\n';
    for (size_t i = 0; i < intersect.size(); ++i){
        recentSearch.push_back(intersect[i]);
    }
    
}

void makeKeywordMap(Log &input, unordered_map<string, vector<int>> &messageMap, int index){
    string catTemp, mesTemp;
    // rewrite so you dont have to use find
    // bool found = false;
    // or account for if i is the last element and push the last one
    for (size_t i = 0; i < input.category.size(); ++i){
        if (input.category[i] == ' ' || !isalnum(input.category[i])){
            if (catTemp != ""){
                //transform(catTemp.begin(), catTemp.end(), catTemp.begin(), ::tolower);
                auto iter = messageMap.find(catTemp);
                if (iter == messageMap.end()){
                    messageMap[catTemp].push_back(index);
                    // found = true;
                }
                else {
                    // right here causing program to be really slow
                    // if found = false
                    if (!binary_search(messageMap[catTemp].begin(), messageMap[catTemp].end(), index)){
                        messageMap[catTemp].push_back(index);
                    }
                }
            }
            catTemp = "";
        }
        else {
            catTemp += (char)(tolower(input.category[i]));
            if (i == input.category.size() - 1){
                //transform(catTemp.begin(), catTemp.end(), catTemp.begin(), ::tolower);
                auto iter = messageMap.find(catTemp);
                if (iter == messageMap.end()){
                    messageMap[catTemp].push_back(index);
                }
                else {
                    if (!binary_search(messageMap[catTemp].begin(), messageMap[catTemp].end(), index)){
                        messageMap[catTemp].push_back(index);
                    }
                }
            }
        }
    }
    for (size_t i = 0; i < input.messages.size(); ++i){
        if (input.messages[i] == ' ' || !isalnum(input.messages[i])){
            if (mesTemp != ""){
                //transform(mesTemp.begin(), mesTemp.end(), mesTemp.begin(), ::tolower);
                auto iter = messageMap.find(mesTemp);
                if (iter == messageMap.end()){
                    messageMap[mesTemp].push_back(index);
                }
                else {
                    if (!binary_search(messageMap[mesTemp].begin(), messageMap[mesTemp].end(), index)){
                        messageMap[mesTemp].push_back(index);
                    }
                }
                mesTemp = "";
            }
        }
        else {
            mesTemp += (char)(tolower(input.messages[i]));
            if (i == input.messages.size() - 1){
                //transform(mesTemp.begin(), mesTemp.end(), mesTemp.begin(), ::tolower);
                auto iter = messageMap.find(mesTemp);
                if (iter == messageMap.end()){
                    messageMap[mesTemp].push_back(index);
                }
                else {
                    if (!binary_search(messageMap[mesTemp].begin(), messageMap[mesTemp].end(), index)){
                        messageMap[mesTemp].push_back(index);
                    }
                }
            }
        }
        
    }
}

void appendLog(int entryId, vector<Log> &masterLog, deque<int> &excerptList){
    // fix this
    //Log temp = masterLog[32834];
    for (int i = 0; i < (int)masterLog.size(); ++i){
        if (entryId == masterLog[i].entryId){
            excerptList.push_back(i);
            break;
        }
    }
    cout << "log entry " << entryId << " appended" << '\n';
}

void appendSearch (vector<int> &recentSearch, deque<int> &excerptList){
    // doing it at the beginning of this function will take too much time
    // need to write something inputs logs into recent search already sorted
    // maybe use heap for recent search
    //sort(recentSearch.begin(), recentSearch.end(), TimeCompare());
    for (size_t i = 0; i < recentSearch.size(); ++i){
        excerptList.push_back(recentSearch[i]);
    }
    cout << recentSearch.size() << " log entries appended" << '\n';
}

void deleteLog(int input, deque<int> &excerptList){
    excerptList.erase(excerptList.begin() + input);
    cout << "Deleted excerpt list entry " << input << '\n';
}

// works
void moveToBegin(int input, deque<int> &excerptList){
    auto it = excerptList.begin() + input;
    rotate(excerptList.begin(), it, it + 1);
    cout << "Moved excerpt list entry " << input << '\n';
}

// works
void moveToEnd(int input, deque<int> &excerptList){
    //Log* temp = excerptList[input];
    //int temp = excerptList[input];
    auto it = excerptList.begin() + input;
    rotate(it, it + 1, excerptList.end());
    cout << "Moved excerpt list entry " << input << '\n';
}

void sortExcerpt(deque<int> &excerptList, vector<Log> &masterLog){
    if (excerptList.size() == 0){
        cout << "excerpt list sorted" << '\n';
        cout << "(previously empty)" << '\n';
    }
    else {
        size_t size = excerptList.size() - 1;
        cout << "excerpt list sorted" << '\n';
        cout << "previous ordering:" << '\n';
        cout << 0 << '|' << masterLog[excerptList[0]].entryId << '|' << masterLog[excerptList[0]].semicolTimestamp << '|' << masterLog[excerptList[0]].category << '|' << masterLog[excerptList[0]].messages << '\n';
        cout << "..." << '\n';
        cout << size << '|' << masterLog[excerptList[size]].entryId << '|' << masterLog[excerptList[size]].semicolTimestamp << '|' << masterLog[excerptList[size]].category << '|' << masterLog[excerptList[size]].messages << '\n';
        sort(excerptList.begin(), excerptList.end());
        cout << "new ordering:" << '\n';
        cout << 0 << '|' << masterLog[excerptList[0]].entryId << '|' << masterLog[excerptList[0]].semicolTimestamp << '|' << masterLog[excerptList[0]].category << '|' << masterLog[excerptList[0]].messages << '\n';
        cout << "..." << '\n';
        cout << size << '|' << masterLog[excerptList[size]].entryId << '|' << masterLog[excerptList[size]].semicolTimestamp << '|' << masterLog[excerptList[size]].category << '|' << masterLog[excerptList[size]].messages << '\n';
    }
    
}

void clearExcerpt(deque<int> &excerptList, vector<Log> &masterLog){
    // STILL NEED TO FIX TIMESTAMP OUTPUT
    if (excerptList.size() == 0){
        cout << "excerpt list cleared" << '\n';
        cout << "(previously empty)" << '\n';
    }
    else {
        size_t size = excerptList.size() - 1;
        cout << "excerpt list cleared" << '\n' << "previous contents:" << '\n';
        cout << 0 << '|' << masterLog[excerptList[0]].entryId << '|' << masterLog[excerptList[0]].semicolTimestamp << '|' << masterLog[excerptList[0]].category << '|' << masterLog[excerptList[0]].messages << '\n';
        cout << "..." << '\n';
        cout << size << '|' << masterLog[excerptList[size]].entryId << '|' << masterLog[excerptList[size]].semicolTimestamp << '|' << masterLog[excerptList[size]].category << '|' << masterLog[excerptList[size]].messages << '\n';
        excerptList.clear();
    }
}
// redo error check
void printRecent(vector<Log> &masterLog, vector<int> &recentSearch){
    // sorting it will
    //sort(recentSearch.begin(), recentSearch.end(), TimeCompare());
    for (size_t i = 0; i < recentSearch.size(); ++i){
        cout << masterLog[recentSearch[i]].entryId << '|' << masterLog[recentSearch[i]].semicolTimestamp << '|' << masterLog[recentSearch[i]].category << '|' << masterLog[recentSearch[i]].messages << '\n';
    }
}

void printExcerpt(deque<int> &excerptList, vector<Log> &masterLog){
    for (size_t i = 0; i < excerptList.size(); ++i){
         cout << i << '|' << masterLog[excerptList[i]].entryId << '|' << masterLog[excerptList[i]].semicolTimestamp << '|' << masterLog[excerptList[i]].category << '|' << masterLog[excerptList[i]].messages << '\n';
    }
}

int main(int argc, const char * argv[]) {
    
     #ifdef __APPLE__
     freopen("sample-all-cmds.txt", "r", stdin);
     #endif
     
    //unordered_map<long long, vector<int>> timeMap;
    unordered_map<string, vector<int>> categoryMap;
    unordered_map<string, vector<int>> messageMap;
    vector<Log> masterLog;
    ifstream infile;
    infile.open(argv[1]);
    Log input;
    int count = 0;
    ++argc;
    //high_resolution_clock::time_point t1 = high_resolution_clock::now();
    while (getline(infile, input.semicolTimestamp, '|')){
        string temp;
        // fix timestamp so you read char by char instead of reading first then going through it again
        
        for (size_t i = 0; i < input.semicolTimestamp.size(); ++i){
            if (input.semicolTimestamp[i] != ':'){
                temp += input.semicolTimestamp[i];
            }
        }
        
        input.timestamp = stol(temp);
        getline(infile, input.category, '|');
        getline(infile, input.messages);
        input.entryId = count;
        masterLog.push_back(input);
        ++count;
    }
    infile.close();
    //high_resolution_clock::time_point t2 = high_resolution_clock::now();
    //auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << count << " entries read" << '\n';
    
    //high_resolution_clock::time_point t3 = high_resolution_clock::now();
    sort(masterLog.begin(), masterLog.end(), masterCompare());
    //high_resolution_clock::time_point t4 = high_resolution_clock::now();
    //auto duration2 = duration_cast<microseconds>( t4 - t3 ).count();
    
    //long long sum = 0;
    //long long sum2 = 0;
    //high_resolution_clock::time_point t5 = high_resolution_clock::now();
    for (int i = 0; i < (int)masterLog.size(); ++i){
        //high_resolution_clock::time_point t9 = high_resolution_clock::now();
        //timeMap[masterLog[i].timestamp].push_back(i);
        
        string temp = convertToLower(masterLog[i].category);
        categoryMap[temp].push_back(i);
        //high_resolution_clock::time_point t10 = high_resolution_clock::now();
        //auto duration5 = duration_cast<microseconds>( t10 - t9 ).count();
        //sum2 = sum2 + duration5;
        // these maps take about 0.8 seconds
        
        
        //high_resolution_clock::time_point t7 = high_resolution_clock::now();
        makeKeywordMap(masterLog[i], messageMap, i);
        //high_resolution_clock::time_point t8 = high_resolution_clock::now();
        //auto duration4 = duration_cast<microseconds>( t8 - t7 ).count();
        //sum = sum + duration4;
    }
    //high_resolution_clock::time_point t6 = high_resolution_clock::now();
    //auto duration3 = duration_cast<microseconds>( t6 - t1 ).count();
    // reading in input takes about 0.37 seconds
    // initializing maps takes 3-4 seconds
    // making keyword map takes 4 seconds for 40,000 entries
    // making other maps takes 0.067 seconds for 40,000 entries
    // find a way to push log pointers as you read in values
    vector<int> recentSearch;
    deque<int> excerptList;
    char command;
    //cin.open(argv[3]);
    cout << "% ";
    //high_resolution_clock::time_point t1 = high_resolution_clock::now();
    while (cin >> command){
        switch(command){
                // timestamp search
            case 't': {
                searchOccurred = true;
                recentSearch.clear();
                string temp1, temp2;
                cin >> std::ws;
                getline(cin, temp1, '|');
                getline(cin, temp2);
                string temp3, temp4;
                for (size_t i = 0; i < temp1.size(); ++i){
                    if (temp1[i] != ':'){
                        temp3 += temp1[i];
                        temp4 += temp2[i];
                    }
                }
                
                
                // make it so you read char by char instead of reading first then read char by char
                long long time1 = stol(temp3);
                long long time2 = stol(temp4);
                timestampSearch(time1, time2, masterLog, recentSearch);
                break;
            }
                // matching search should work
            case 'm': {
                searchOccurred = true;
                recentSearch.clear();
                string input;
                cin >> input;
                string temp;
                for (size_t i = 0; i < input.size(); ++i){
                    if (input[i] != ':'){
                        temp += input[i];
                    }
                }
                long long timestamp = stol(temp);
                matchingSearch(timestamp, masterLog, recentSearch);
                break;
                // look at this
                // invalid timestamps
            }
                // catergory search works
            case 'c': {
                searchOccurred = true;
                recentSearch.clear();
                string input;
                cin >> std::ws;
                getline(cin, input);
                //string temp = convertToLower(input);
                transform(input.begin(), input.end(), input.begin(), ::tolower);
                categorySearch(input, categoryMap, recentSearch);
                break;
            }
            case 'k':{
                searchOccurred = true;
                recentSearch.clear();
                string input;
                vector<string> keywords;
                // instead of reading whole line read char by char
                getline(cin, input);
                string temp;
                for (size_t i = 0; i < input.size() + 1; ++i){
                    if (input[i] == ' ' || !isalnum(input[i])){
                        if (temp != ""){
                            transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
                            keywords.push_back(temp);
                            temp = "";
                        }
                    }
                    else {
                        temp += input[i];
                    }
                }
                keywordSearch(messageMap, keywords, recentSearch);
                break;
            }
            case 'a':{
                int input;
                cin >> input;
                if (input > (int)masterLog.size() - 1){
                    cerr << "out of bounds input" << '\n';
                    break;
                }
                appendLog(input, masterLog, excerptList);
                break;
            }
            case 'r':{
                if (searchOccurred == false){
                    cerr << "search has not occured, invalid print" << '\n';
                    break;
                }
                appendSearch(recentSearch, excerptList);
                break;
            }
            case 'd':{
                int input;
                cin >> input;
                if (input > (int)excerptList.size() - 1){
                    cerr << "out of bounds input" << '\n';
                    break;
                }
                deleteLog(input, excerptList);
                break;
            }
            case 'b':{
                int input;
                cin >> input;
                if (input > (int)excerptList.size() - 1){
                    cerr << "out of bounds input" << '\n';
                    break;
                }
                moveToBegin(input, excerptList);
                break;
            }
            case 'e':{
                int input;
                cin >> input;
                if (input > (int)excerptList.size() - 1){
                    cerr << "out of bounds input" << '\n';
                    break;
                }
                moveToEnd(input, excerptList);
                break;
            }
            case 's':{
                sortExcerpt(excerptList, masterLog);
                break;
            }
            case 'l':{
                clearExcerpt(excerptList, masterLog);
                break;
            }
            case 'g':{
                if (searchOccurred == false){
                    cerr << "search has not occured, invalid print" << '\n';
                    break;
                }
                printRecent(masterLog, recentSearch);
                break;
            }
            case 'p':{
                printExcerpt(excerptList, masterLog);
                break;
            }
                
            case 'q':{
                //high_resolution_clock::time_point t2 = high_resolution_clock::now();
                //auto duration = duration_cast<microseconds>( t2 - t1 ).count();
                //cout << duration;
                return 0;
                break;
            }
            case '#':{
                string input;
                getline(cin, input);
                break;
                
            }
            default:{
                cerr << "aids" << '\n';
            }
        }
        cout << "% ";
    }
    //cin.close();
    
    // TODO
    // sorting too many times, only should be twice
    // theres a way to push values onto recent search already sorted
    // way to put values onto excerpt list already sorted
    
    // pointers are 8 bytes, ints are 4 bytes
    // change pointers to ints might save memory
    
    // reading in everything takes 4 secs
    // timestamp search of sample log takes 6 secs
    
    // deque store ints instead of pointers
    // comparator might be too slow
    // alot of time spent in deque
    // keep a global variable for size of deque.size
    
}
