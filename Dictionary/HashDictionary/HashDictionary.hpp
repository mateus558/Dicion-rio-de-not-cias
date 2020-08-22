#ifndef HASH_DICTIONARY_HPP_INCLUDED
#define HASH_DICTIONARY_HPP_INCLUDED

#include "Dictionary.hpp"

using json = nlohmann::json;


class Term: public DictNode{
public:
    std::string value;
};

class HashDictionary: public Dictionary{
private:
    const size_t table_size = 113989;
    std::vector<Term*> table;

    size_t h(const std::string key);
    void computeTermsParameters();

public:
    HashDictionary();

    DictNode* find(const std::string& word);
    bool insert(const std::string& word, Document* doc_info);
    
    ~HashDictionary();
};

#endif