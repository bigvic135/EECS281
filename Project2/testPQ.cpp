/*
 * Compile this test against your .h files to make sure they compile. Note how
 * the eecs281 priority queues can be constructed with the different types. We
 * suggest adding to this file or creating your own test cases to test your
 * priority queue implementations. 
 *
 * Notice that testPairing() tests the range-based constructor but main() and
 * testPriorityQueue() do not.
 *
 * These tests are NOT a complete test of your priority queues!
 */

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "Eecs281PQ.h"
#include "PoormanPQ.h"
#include "SortedPQ.h"
#include "BinaryPQ.h"
#include "PairingPQ.h"

using namespace std;


// Very basic testing.
void testPriorityQueue(Eecs281PQ<int> *pq, const string &pqType) {
    cout << "Testing priority queue: " << pqType << endl;

    pq->push(3);
    pq->push(4);
    assert(pq->size() == 2);
    assert(pq->top() == 4);

    pq->pop();
    assert(pq->size() == 1);
    assert(pq->top() == 3);
    assert(!pq->empty());

    pq->pop();
    assert(pq->size() == 0);
    assert(pq->empty());

    cout << "testPriorityQueue() succeeded!" << endl;
} // testPriorityQueue()


// Test the pairing heap's range-based constructor, copy constructor,
// and operator=().  Still not complete, should have more code, test
// addNode(), updateElt(), etc.
void testPairing(vector<int> & vec) {
    cout << "Testing Pairing Heap separately" << endl;
    Eecs281PQ<int> * pq1 = new PairingPQ<int>(vec.begin(), vec.end());
    Eecs281PQ<int> * pq2 = new PairingPQ<int>(*((PairingPQ<int> *)pq1));
    // This line is different just to show two different ways to declare a
    // pairing heap: as an Eecs281PQ and as a PairingPQ. Yay for inheritance!
    PairingPQ<int> * pq3 = new PairingPQ<int>();
    *pq3 = *((PairingPQ<int> *)pq2);

    pq1->push(3);
    pq2->pop();
    assert(pq1->size() == 3);
    assert(!pq1->empty());
    assert(pq1->top() == 3);
    pq2->push(pq3->top());
    assert(pq2->top() == pq3->top());
    
    // have to test merge siblings, update elt, meld, and copy
    // testing meld, merge child, and pop
    PairingPQ<int> * pq4 = new PairingPQ<int>();
    pq4->push(9);
    pq4->push(8);
    pq4->push(7);
    pq4->push(5);
    pq4->push(6);
    pq4->push(4);
    pq4->push(10);
    assert(pq4->top() == 10);
    pq4->pop();
    assert(pq4->top() == 9);
    pq4->pop();
    assert(pq4->top() == 8);
    pq4->pop();
    assert(pq4->top() == 7);
    pq4->pop();
    assert(pq4->top() == 6);
    pq4->pop();
    assert(pq4->top() == 5);
    pq4->pop();
    assert(pq4->top() == 4);
    
    // test update Elt and updatePriorities
    PairingPQ<int> * pq5 = new PairingPQ<int>();
    pq5->push(5);
    pq5->updateElt(pq5->addNode(10), 20); // very basic updateElt with node at the root
    assert(pq5->top() == 20);
    pq5->pop();
    assert(pq5->top() == 5);
    
    // test update elt where node is a child
    pq5->updateElt(pq5->addNode(4), 6);
    assert(pq5->top() == 6);
    pq5->pop();
    pq5->pop();
    cout << "test update elt where node is a child" << endl;
    
    // test update elt with Sahni example
    pq5->push(7);
    pq5->push(5);
    pq5->push(6);
    pq5->updateElt(pq5->addNode(4), 10);
    assert(pq5->top() == 10);
    cout << "test update elt with Sahni example" << endl;
    
    // test update elt where you dont change anything, still matches the priorities
    pq5->updateElt(pq5->addNode(4), 8);
    assert(pq5->top() == 10);
    
    // test updateELT where new value greater than parent but parent is not root
    pq5->updateElt(pq5->addNode(5), 9);
    assert(pq5->top() == 10);
    pq5->pop();
    assert(pq5->top() == 9);
    cout << "Basic tests done, calling destructors" << endl;
    
    delete pq1;
    delete pq2;
    delete pq3;
    delete pq4;
    delete pq5;

    cout << "testPairing() succeeded" << endl;
} // testPairing()


int main() {
    // Basic pointer, allocate a new PQ later based on user choice.
    Eecs281PQ<int> *pq;
    vector<string> types{ "Poorman", "Sorted", "Binary", "Pairing" };
    int choice;

    cout << "PQ tester" << endl << endl;
    for (size_t i = 0; i < types.size(); ++i)
        cout << "  " << i << ") " << types[i] << endl;
    cout << endl;
    cout << "Select one: ";
    cin >> choice;

    if (choice == 0) {
        pq = new PoormanPQ<int>;
    } // if
    else if (choice == 1) {
        pq = new SortedPQ<int>;
    } // else if
    else if (choice == 2) {
        pq = new BinaryPQ<int>;
    } // else if
    else if (choice == 3) {
        pq = new PairingPQ<int>;
    } // else if
    else {
        cout << "Unknown container!" << endl << endl;
        exit(1);
    } // else
   
    testPriorityQueue(pq, types[choice]);

    if (choice == 3) {
        vector<int> vec;
        vec.push_back(0);
        vec.push_back(1);
        testPairing(vec);
    } // if

    // Clean up!
    delete pq;

    return 0;
} // main()
