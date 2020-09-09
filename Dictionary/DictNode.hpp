#ifndef DOCUMENTCOUNTERAVL_HPP_INCLUDED
#define DOCUMENTCOUNTERAVL_HPP_INCLUDED

#include "Document.hpp"
#include "DocumentHeap.hpp"
#include <set>
#include <map>
#include <cmath>
#include <vector>

struct AVLNode {
    /// Node height on the tree and its frequency in the document
    size_t height = 0, count = 0;
    /// Term weight on the document
    double weight = 0.0;
    /// Document where the term appears
    Document *doc = nullptr;
    /// Left and right nodes
    AVLNode *left = nullptr, *right = nullptr;

    ~AVLNode(){ 
        if(left)  delete left;
        if(right) delete right;
    }
};

/**
 * \brief Base class for the document dictionary node
 */

class DictNode{
    private:
        /// Root of the AVL containing the inverted index
        AVLNode *root = nullptr;
        /// Number of documents
        size_t _size = 0, n_docs = 0;
        /// Array of docs on AVL
        std::vector<Document*> ranked_docs;
        
        // Right rotation receiving a node y 
        AVLNode *rightRotate(AVLNode *y);
        // Left rotation receiving a node x
        AVLNode *leftRotate(AVLNode *x);
        // function for allocation of a new node
        AVLNode *newNode(Document *doc);
        // returns the height of a subtree
        size_t height(AVLNode *N){ return (N == nullptr)?0:N->height; }
        // returns the balance factor of a subtree
        int getBalance(AVLNode *N){ return (N == NULL)?0:height(N->left) - height(N->right); }
        // Insert a document if it doesn't already exist or update the term frequency in it
        AVLNode* insertOrCount_helper(AVLNode *root, Document * doc);
        // computes all term weights
        void computeWeights_helper(AVLNode *root, size_t &total_documents);
        // compute the ranks based on the inversed index of a term
        void computeRanks_helper(AVLNode *root);
        void print_helper(AVLNode *root);
        // find a document in the inversed index of the term
        AVLNode* find(AVLNode* root,  Document * doc);
    public:
        DictNode();

        bool insertOrCount(Document* doc);
        size_t size() const { return _size; }
        void computeWeights(size_t total_documents);
        std::vector<Document*> computeRanks();
        size_t& operator[](Document * doc);
        void print();

        ~DictNode(){
            if(root) delete root;
        }
};

#endif