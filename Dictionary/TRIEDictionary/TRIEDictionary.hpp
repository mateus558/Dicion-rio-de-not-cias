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
    bool end = false;
    TrieNode** children;
    ChildrenList* children_list = nullptr;

    TrieNode(bool is_leaf = false){
        if(!is_leaf){
            build_children_list();
        }else{
            children = nullptr;
        }
        children_list = nullptr;    
    }

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

using WordInfo = std::pair<std::string, TrieNode*>;
using DocRank = std::pair<size_t, double>;


class TRIEDictionary: public Dictionary {
private:
    TrieNode *root;
    
    void transverse_helper(TrieNode* current, int i, std::string &buffer, std::vector<WordInfo> &words_info);
    void computeTermsParameters_helper(TrieNode* root, int *i);
    void computeTermsParameters();
    std::vector<WordInfo> transverse();

public:
    TRIEDictionary();

    size_t distinctTerms(){ return distinct_terms; }
    DictNode* find(const std::string& word);
    bool insert(const std::string& word, Document* doc_info);

    ~TRIEDictionary();

    friend std::ostream & operator << (std::ostream &out, TRIEDictionary &dict);
};


#endif