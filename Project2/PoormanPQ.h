#ifndef POORMANPQ_H
#define POORMANPQ_H

#include "Eecs281PQ.h"

#include <utility> // needed for swap()
#include <limits>  // needed for UNKNOWN

static const size_t UNKNOWN = std::numeric_limits<size_t>::max();


// A specialized version of the 'heap' ADT that is implemented with an
// underlying unordered array-based container that is linearly searched
// for the most extreme element every time it is needed.

// Since top() is almost always followed by pop(), when top() has to find
// the most extreme element, it remembers that index so that pop() does
// does not have to search again.  Note the use of the mutable variable.

// TODO: Read and understand this priority queue implementation!
// Pay particular attention to how the constructors and findExtreme()
// are written, especially the use of this->compare.

template<typename TYPE, typename COMP = std::less<TYPE>>
class PoormanPQ : public Eecs281PQ<TYPE, COMP> {
    using base = Eecs281PQ<TYPE, COMP>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    explicit PoormanPQ(COMP comp = COMP()) :
        base{ comp }, extreme{ UNKNOWN } {
    } // PoormanPQ()


    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PoormanPQ(InputIterator start, InputIterator end, COMP comp = COMP()) :
        base{ comp }, data{ start, end }, extreme{ UNKNOWN } {
    } // PoormanPQ()


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~PoormanPQ() {
    } // ~PoormanPQ()


    // Description: Does nothing for this implementation, as items can never be
    //              'out of order'. This function is implemented for you.
    // Runtime: O(1)
    virtual void updatePriorities() {
        extreme = UNKNOWN;
    } // updatePriorities()


    // Description: Add a new element to the heap.
    // Runtime: Amortized O(1)
    virtual void push(const TYPE & val) {
        data.push_back(val);
        extreme = UNKNOWN;
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(n)
    // Note: If the most extreme element is already known (as would happen if
    //       .top() was called before .pop()), this function is O(1).
    virtual void pop() {
        if (extreme == UNKNOWN)
            findExtreme();
        std::swap(data[extreme], data.back());
        data.pop_back();
        extreme = UNKNOWN;
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.
    // Runtime: O(n)
    virtual const TYPE & top() const {
        if (extreme == UNKNOWN)
            findExtreme();
        return data[extreme];
    } // top()


    // Description: Get the number of elements in the heap. This has been
    //              implemented for you.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
    } // size()

    // Description: Return true if the heap is empty. This has been implemented
    //              for you.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
    } // empty()


private:
    // Note: This vector *must* be used your heap implementation.
    std::vector<TYPE> data;

private:
    mutable size_t extreme;
    void findExtreme() const {
        size_t index = 0;

        for (size_t i = 1; i < data.size(); ++i)
            if (this->compare(data[index], data[i]))
                index = i;

        extreme = index;
    } // findExtreme()
}; // PoormanPQ

#endif // POORMANPQ_H
