#ifndef TRIEDICTIONARY_HPP_INCLUDED
#define TRIEDICTIONARY_HPP_INCLUDED
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmultichar"

#include "Dictionary.hpp"

using json = nlohmann::json;

struct TrieNode;

struct ChildrenList{
    size_t key = 0;
    TrieNode* node = nullptr;
    ChildrenList* next = nullptr;
};

class TrieNode: public DictNode{
public:
    /// flag indicating if the node is the end of a word
    bool end = false;
    /// array of children nodes for characters
    TrieNode** children;
    /// list of children nodes for numbers
    ChildrenList* children_list = nullptr;

    TrieNode(bool is_leaf = false){
        // only allocate children array if it is a leaf
        if(!is_leaf){
            build_children_list();
        }else{
            children = nullptr;
        }
        children_list = nullptr;    
    }

    /// children array constructor
    void build_children_list(){ 
        children = new TrieNode*[26];
        for(size_t i = 0; i < 26; i++){
            children[i] = nullptr;
        }
    }

    ~TrieNode(){
        for(size_t i = 0; i < 26; i++){
            if(children && children[i]) delete children[i];
        }
        auto curr = children_list;
        while(curr){
            auto next = curr->next;
            delete curr;
            curr = next;
        }
    }
};

/// pair with the term and its associated node
using WordInfo = std::pair<std::string, TrieNode*>;
using DocRank = std::pair<size_t, double>;

/**
 * \brief Dictionary derived class for the TRIE strategy
 */
class TRIEDictionary: public Dictionary {
private:
    /// TRIE root node
    TrieNode *root;

    /**
     * \brief helper for tree transverse 
     * \return void
     */
    void transverse_helper(TrieNode* current, int i, std::string &buffer, std::vector<WordInfo> &words_info);
    /**
     * \brief helper for weights computing 
     * \return void
     */
    void computeTermsParameters_helper(TrieNode* root, int *i);
    /**
     * \brief transverse the tree and return an array of elements in alphabet order
     * \return std::vector<WordInfo>
     */
    std::vector<WordInfo> transverse();
    void computeTermsParameters() override;

public:
    TRIEDictionary();

    DictNode* find(const std::string& word) override;
    bool insert(const std::string& word, Document* doc_info) override;

    ~TRIEDictionary();

    friend std::ostream & operator << (std::ostream &out, TRIEDictionary &dict);
};


#endif