#ifndef DOCUMENTCOUNTERAVL_HPP_INCLUDED
#define DOCUMENTCOUNTERAVL_HPP_INCLUDED

#include "Document.hpp"
#include "DocumentHeap.hpp"
#include <set>
#include <map>
#include <cmath>
#include <vector>

struct DocCount { 
    size_t height = 0, count = 0;
    Document *doc = nullptr;
    DocCount *left = nullptr, *right = nullptr;

    ~DocCount(){ 
        if(left)  delete left;
        if(right) delete right;
    }
};

class DocumentCounterAVL{
    private:
        DocCount *root = nullptr;
        size_t _size = 0, n_docs = 0;
        std::map<size_t, double> weights;
        std::vector<Document*> ranked_docs;
        
        DocCount *rightRotate(DocCount *y);
        DocCount *leftRotate(DocCount *x);
        DocCount *newNode(Document *doc);
        size_t height(DocCount *N){ return (N == nullptr)?0:N->height; }
        int getBalance(DocCount *N){ return (N == NULL)?0:height(N->left) - height(N->right); } 
        DocCount* insertOrCount_helper(DocCount *root, Document * doc);
        void computeWeights_helper(DocCount *root, size_t &total_documents);
        void computeRanks_helper(DocCount *root);
        void print_helper(DocCount *root);
        DocCount* find(DocCount* root,  Document * doc);
    public:
        DocumentCounterAVL();

        bool insertOrCount(Document* doc);
        size_t size() const { return _size; }
        double getWeight(const size_t &doc_id){ return weights[doc_id]; }
        void computeWeights(size_t total_documents);
        std::vector<Document*> computeRanks();
        size_t& operator[](Document * doc);
        void print();

        ~DocumentCounterAVL(){
            if(root) delete root;
        }
};

#endif