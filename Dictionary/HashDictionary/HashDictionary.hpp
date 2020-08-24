#ifndef HASH_DICTIONARY_HPP_INCLUDED
#define HASH_DICTIONARY_HPP_INCLUDED

#include "Dictionary.hpp"

using json = nlohmann::json;

/**
 * \brief DictNode derived class for the hash dictionary
 */
class Term: public DictNode{
public:
    /// word stored in the term class
    std::string value;
};

/**
 * \brief Dictionary derived class for the hash strategy
 */
class HashDictionary: public Dictionary{
private:
    /// table size
    const size_t table_size = 113989;
    /// hash table
    std::vector<Term*> table;

    /// hash function using djb2 algorithm
    size_t h(const std::string key);
    void computeTermsParameters() override;

public:
    HashDictionary();

    DictNode* find(const std::string& word) override;
    bool insert(const std::string& word, Document* doc_info) override;
    void print() override;
    ~HashDictionary();
    friend std::ostream & operator << (std::ostream &out, HashDictionary &dict);
};

#endif