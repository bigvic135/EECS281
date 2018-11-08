#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the 'priority_queue' ADT implemented as a pairing priority_queue.
template<typename TYPE, typename COMP = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP> {
    using base = Eecs281PQ<TYPE, COMP>;
    
public:
    // Each node within the pairing heap
    class Node {
    public:
        explicit Node(const TYPE &val)
        : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent{ nullptr }
        {}
        
        // Description: Allows access to the element at that Node's position.
        // Runtime: O(1) - this has been provided for you.
        const TYPE &operator*() const { return elt; }
        const Node *sibling_ptr() const { return sibling; }
        const Node *child_ptr() const { return child; }
        
        // The following line allows you to access any private data members of this
        // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
        // statement in PairingPQ's add_node() function).
        friend PairingPQ;
        
    private:
        TYPE elt;
        Node *child;
        Node *sibling; // sibling is next
        Node *parent; // parent is previous
        // TODO: Add one extra pointer (parent or previous) as desired.
    }; // Node
    
    
    // Description: Construct an empty priority_queue with an optional comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP comp = COMP()) :
    base{ comp } {
        root = nullptr;
        // something wrong with this
    } // PairingPQ()
    
    
    // Description: Construct a priority_queue out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP comp = COMP()) :
    base{ comp } {
        root = nullptr;
        while (start != end){
            push(*start);
            ++start;
        }
    } // PairingPQ()
    
    
    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ& other) :
    base{ other.compare } {
        root = nullptr;
        *this = other;
    } // PairingPQ()
    
    
    // Description: Copy assignment operator.
    // Runtime: O(n)
    // TODO: when you implement this function, uncomment the parameter names.
    PairingPQ& operator=(const PairingPQ& rhs) {
        if (this != &rhs){
            emptyHeap();
            root = copy(rhs.root);
        }
        
        return *this;
    } // operator=()
    
    
    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        emptyHeap();
    } // ~PairingPQ()
    
    
    // Description: Assumes that all elements inside the priority_queue are out of order and
    //              'rebuilds' the priority_queue by fixing the priority_queue invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        if(root == nullptr){
            return;
        }
        // create temp vector to store values
        std::vector<Node*> temp;
        Node *node = root;
        
        // if there is a child
        if(node->child != nullptr){
            temp.push_back(node->child);
        }
        
        // break links
        node->child = nullptr;
        node->sibling = nullptr;
        node->parent = nullptr;
        //newRoot = node;
        // loop until tree is empty
        while(!temp.empty())
        {
            // update node
            node = temp.back();
            temp.pop_back();
            // if there is a child push onto vector
            if(node->child != nullptr){
                temp.push_back(node->child);
            }
            // if there is a sibling push onto sibling
            if(node->sibling != nullptr){
                temp.push_back(node->sibling);
            }
            // break links of pointers of node you are about to push on vector
            node->child = nullptr;
            node->sibling = nullptr;
            node->parent = nullptr;
            meld(root, node);
        }
    } // updatePriorities()
    
    
    // Description: Add a new element to the priority_queue. This has been provided for you,
    //              in that you should implement push functionality in the
    //              addNode function.
    // Runtime: Amortized O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        addNode(val);
    } // push()
    
    
    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority_queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // removes the root
        Node *victim = root;
        // if root has no child
        if (root->child == NULL){
            root = nullptr;
        }
        // if root has child
        else {
            root = mergeSiblings(root->child);
        }
        --heapSize;
        delete victim;
    } // pop()
    
    
    // Description: Return the most extreme (defined by 'compare') element of
    //              the priority_queue.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        return root->elt;
    } // top()
    
    
    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return heapSize;
        
    } // size()
    
    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return (root == nullptr);
    } // empty()
    
    
    // Description: Updates the priority of an element already in the priority_queue by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain priority_queue invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    // TODO: when you implement this function, uncomment the parameter names.
    void updateElt(Node* node, const TYPE & new_value) {
        node->elt = new_value;
        updatePriorities();
    } // updateElt()
    
    
    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: Amortized O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    
     Node* addNode(const TYPE & val) {
         Node *newNode = new Node(val);
         if(root == nullptr){
             root = newNode;
         }
         else{
             meld(root, newNode);
         }
         heapSize++;
         return newNode;
     } // addNode()
    
    
    
private:
    // TODO: Add any additional member functions or data you require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap papers).
    size_t heapSize = 0;
    Node *root;
    // compare two max pairing heaps and combine into one
    // the tree with the smaller root is made the leftmost subtree of the other tree
    void meld(Node* &first, Node* &second){
        if (second == nullptr){
            return;
        }
        if (this->compare(first->elt, second->elt)){
            // attatch first as leftmost child of second
            first->parent = second;
            first->sibling = second->child;
            second->child = first;
            first = second;
        }
        else{
            // attach second as leftmost child of first
            second->parent = first;
            second->sibling = first->child;
            first->child = second;
        }
    }
    
    // takes out everything in heap dont think you need to use pop
    // pop will take too long
    void emptyHeap(){
        deleteMemory(root);
        root = nullptr;
    }
    
    void deleteMemory(Node *node) const {
        if (node != nullptr){
            deleteMemory(node->child);
            deleteMemory(node->sibling);
            delete node;
        }
    }
    
    Node* mergeSiblings(Node *node){
        // only one sibling
        if (node->sibling == nullptr){
            return node;
        }
        std::vector<Node*> tree(5);
        int numSiblings = 0;
        // storing subtrees in a vector
        while (node != nullptr){
            if (numSiblings == (int)tree.size()){
                tree.resize(numSiblings * 2);
            }
            tree[numSiblings] = node;
            node->parent->child = nullptr; // break links
            node = node->sibling;
            ++numSiblings;
        }
        if (numSiblings == (int)tree.size()){
            tree.resize(numSiblings + 1);
        }
        tree[numSiblings] = nullptr;
        
        // combine subtrees two at a time, going left to right
        int i = 0;
        while (i + 1 < numSiblings){
            meld(tree[i], tree[i + 1]);
            i += 2;
        }
        
        // j has the result of last mend and if it is odd number of trees you get the last one
        int j = i - 2;
        if (j == numSiblings - 3){
            meld(tree[j], tree[j + 2]);
        }
        
        // combine tree right to left
        while (j >= 2){
            meld(tree[j - 2], tree[j]);
            j -= 2;
        }
        return tree[0];
    }
     
    
    Node* copy(Node *input){
        if (input == nullptr){
            return nullptr;
        }
        else{
            Node *node = new Node(input->elt);
            if ((node->child = copy(input->child)) != nullptr){
                node->child->parent = node;
            }
            if ((node->sibling = copy(input->sibling)) != nullptr){
                node->sibling->parent = node;
            }
            return node;
        }
    }
    
};


#endif // PAIRINGPQ_H


