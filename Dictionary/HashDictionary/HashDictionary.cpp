#include "HashDictionary.hpp"

HashDictionary::HashDictionary(){
    // initialize the hash table with null positions
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
    // while theres colision, advance to the next table entry (linear probing)
    while(key < table_size && table[key] && table[key]->value != word){
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
    
    // while theres colision, advance to the next table entry (linear probing)
    while(table[key] && table[key]->value != word){
        key++;
    }
    if(!table[key]){
        table[key] = new Term();
        table[key]->value = word;
        // update the number of unique terms in the document and dictionary if the position is empty
        doc_info->unique_terms++;
        distinct_terms++;
    }
    // count the number of occurrences of the word in the document
    table[key]->docs_counts[doc_info]++;

    return true;
}

void HashDictionary::computeTermsParameters(){
    size_t i = 0;
    // go through the table ignoring null entries
    for(auto it = table.begin(); it != table.end(); it++){
        auto term = (*it);
        if(!term) continue;
        auto n_docs = term->docs_counts.size();
        if(verbose){
            i++; 
            std::clog << "Computing the weights of the term " << i << std::endl;
        }
        if(n_docs){
            // go through the list of documents of the term and compute the term weight for each document
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