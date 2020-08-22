#include "HashDictionary.hpp"

HashDictionary::HashDictionary(){
    table.assign(table_size, nullptr);
}

size_t HashDictionary::h(const std::string key){
    size_t hash = 5381;

    for(size_t i = 0; i < key.size(); i++){
        hash = ((hash << 5) + hash) + key[i]; /* hash * 33 + c */
    }

    return hash;
}

DictNode* HashDictionary::find(const std::string& word){
    size_t key = h(word) % table_size;
    
    comparisons = 2;
    while(table[key] && table[key]->value != word){
        key++;
        comparisons++;
    }
    if(!table[key]){
        return nullptr;
    }

    return table[key];
}

bool HashDictionary::insert(const std::string& word, Document* doc_info){
    size_t key = h(word) % table_size;
    
    while(table[key] && table[key]->value != word){
        key++;
    }
    if(!table[key]){
        table[key] = new Term();
        table[key]->value = word;
        doc_info->unique_terms++;
        distinct_terms++;
    }
    table[key]->docs_counts[doc_info]++;

    return true;
}

void HashDictionary::computeTermsParameters(){
    size_t i = 0;
    for(auto it = table.begin(); it != table.end(); it++){
        auto term = (*it);
        if(!term) continue;
        auto n_docs = term->docs_counts.size();
        if(verbose){
            i++; 
            std::clog << "Computing the weights of the term " << i << std::endl;
        }
        if(n_docs){
            for(auto it = term->docs_counts.begin(); it != term->docs_counts.end(); it++){
                term->weight_i[(*it).first->id] = (*it).second*(log2(documents.size())/n_docs);
            }
        }
    }
}

HashDictionary::~HashDictionary(){
    for(auto it = table.begin(); it != table.end(); it++){
        if(*it) delete (*it);
    }
    table.clear();
}