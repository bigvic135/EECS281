#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP> {
    using base = Eecs281PQ<TYPE, COMP>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP comp = COMP()) :
        base{ comp } {
            this->compare = comp;
    } // BinaryPQ


    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP comp = COMP()) :
        base{ comp } {
            this->compare = comp;
            while (start != end){
                data.push_back(*start);
                ++start;
            }
            updatePriorities();
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        for (int i = (int)size() - 1; i >= 0; --i){
            updateDown(i);
        }
    } // updatePriorities()
    
    
    // Description: Add a new element to the heap.
    // Runtime: O(log(n))
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        data.push_back(val);
        if (size() > 1){
            updateUp((int)size() - 1);
        }
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        // swaps so min value is on top and max on bottom and pops it
        // then update priorities from the start
        std::swap(data[0], data[data.size() - 1]);
        data.pop_back();
        if (size() > 1){
            updateDown(0);
        }
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        return data.front();
    } // top()


    // Description: Get the number of elements in the heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
    } // size()


    // Description: Return true if the heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
    } // empty()


private:
    // Note: This vector *must* be used your heap implementation.
    std::vector<TYPE> data;

    void updateUp(int index){
        // value is on bottom of binary heap
        // as you go up compare the value and if index is smaller swap them
        while (index > 0 && this->compare(data[index/2], data[index])){
            std::swap(data[index/2], data[index]);
            index = index / 2;
        }
    }
    void updateDown(int index){
        // traverse down the tree and if a value is greater than index swap it
        while (index * 2 < (int)size()){
            int i = index * 2;
            if (i < (int)size() - 1 && this->compare(data[i], data[i + 1])){
                ++i;
            }
            if (!this->compare(data[index], data[i])){
                break;
            }
            std::swap(data[index], data[i]);
            index = i;
        }
    }

}; // BinaryPQ


#endif // BINARYPQ_H
