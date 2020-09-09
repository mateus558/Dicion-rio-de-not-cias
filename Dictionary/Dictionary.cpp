#include "Dictionary.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

Dictionary::Dictionary(){
    // accented characters mapping
    char_translator[(unsigned char)'á'] = 'a';
    char_translator[(unsigned char)'Á'] = 'a';
    char_translator[(unsigned char)'à'] = 'a';
    char_translator[(unsigned char)'À'] = 'a';
    char_translator[(unsigned char)'ã'] = 'a';
    char_translator[(unsigned char)'Ã'] = 'a';
    char_translator[(unsigned char)'â'] = 'a';
    char_translator[(unsigned char)'Â'] = 'a';
    char_translator[(unsigned char)'é'] = 'e';
    char_translator[(unsigned char)'É'] = 'e';
    char_translator[(unsigned char)'è'] = 'e';
    char_translator[(unsigned char)'È'] = 'e';
    char_translator[(unsigned char)'ê'] = 'e';
    char_translator[(unsigned char)'Ê'] = 'e';
    char_translator[(unsigned char)'ẽ'] = 'e';
    char_translator[(unsigned char)'Ẽ'] = 'e';
    char_translator[(unsigned char)'í'] = 'i';
    char_translator[(unsigned char)'Í'] = 'i';
    char_translator[(unsigned char)'ì'] = 'i';
    char_translator[(unsigned char)'Ì'] = 'i';
    char_translator[(unsigned char)'î'] = 'i';
    char_translator[(unsigned char)'Î'] = 'i';
    char_translator[(unsigned char)'ĩ'] = 'i';
    char_translator[(unsigned char)'Ĩ'] = 'i';
    char_translator[(unsigned char)'ó'] = 'o';
    char_translator[(unsigned char)'Ó'] = 'o';
    char_translator[(unsigned char)'ò'] = 'o';
    char_translator[(unsigned char)'Ò'] = 'o';
    char_translator[(unsigned char)'ô'] = 'o';
    char_translator[(unsigned char)'Ô'] = 'o';
    char_translator[(unsigned char)'õ'] = 'o';
    char_translator[(unsigned char)'Õ'] = 'o';
    char_translator[(unsigned char)'ú'] = 'u';
    char_translator[(unsigned char)'Ú'] = 'u';
    char_translator[(unsigned char)'ù'] = 'u';
    char_translator[(unsigned char)'Ù'] = 'u';
    char_translator[(unsigned char)'û'] = 'u';
    char_translator[(unsigned char)'Û'] = 'u';
    char_translator[(unsigned char)'ũ'] = 'u';
    char_translator[(unsigned char)'Ũ'] = 'u';
    char_translator[(unsigned char)'ç'] = 'c';
    char_translator[(unsigned char)'Ç'] = 'c';
    char_translator[(unsigned char)'ñ'] = 'n';
    char_translator[(unsigned char)'Ñ'] = 'n';
}

bool Dictionary::isValidTerm(const std::string& term){
    // verify if first character is a number and if the term size is less than 3 or empty
    if(term[0] >= '0' && term[0] <= '9') return false;
    if(term.size() < 3 || term.empty()) return false;
    // check if the term is a number
    if(!term.empty() && std::find_if(term.begin(), term.end(), [](unsigned char c) { return !std::isdigit(c); }) == term.end()) return false;
    // otherwise, it's a valid term
    return true;
}

void Dictionary::addToDocsCounts(Document* doc, DictNode* node){
   node->insertOrCount(doc);
}


std::string Dictionary::normalizeString(const std::string& str){
    std::string clean_str, nopunct;

    clean_str.resize(str.size());
    std::transform(str.begin(),str.end(), clean_str.begin(), [this](unsigned char c){
        // if it's a lower case character, return it
        if(c <= 122 && c >= 97) return (char)c;
        // if it's a uppercase character return it's lower case version
        if(c >= 65 && c <= 90) return (char)(c+32);
        // if it's a number, return it
        if(c >= 48 && c <= 57) return (char)c;
        // the - char represents the separation of two words, so it must return a space
        if(c == '-') return ' ';
        // if it's a punctuation character, return a mark to remove later
        if(std::ispunct(c)) return (char)'.';
        // transform accented letters in lower case non accentuated letter
        if(char_translator.find(c) != char_translator.end()){
            return (char)char_translator[c];
        }
        // as accentuated letters can't be represented by a char, it is represented by
        // a multi char, causing problems, so this conditional mark unecessary codes to be removed
        if(c == 195 || c == 197 || c == 225) return '.';
        if(c == 196) return 'i';
        return (char)' ';
    });
    // remove all chars that are marked with .
    clean_str.erase(std::remove(clean_str.begin(), clean_str.end(), '.'), clean_str.end());

    return clean_str;
}

std::vector<Document*> Dictionary::findByTerms(const std::string& terms, const size_t &n_results){
    // clean the query string
    std::string term, clean_terms = normalizeString(terms);
    std::stringstream input_stringstream(clean_terms);
    // initialize the results array and the heap for the results
    std::vector<Document*> results((n_results > documents.size())?documents.size():n_results, nullptr);
    heap = new DocumentHeap(n_results);

    size_t n_terms = 0;
    size_t n_comparisons = 0;
    // break the query string into terms
    while(std::getline(input_stringstream, term, ' ')){
        // verify if its a valid term
        if(!this->isValidTerm(term)) continue;
        n_terms++;
        if(n_terms > 2){
            std::cerr << "Error: too many terms for search. [Only 2 allowed]" << std::endl;
            return std::vector<Document*>();
        }
        // find the list of documents that match this term
        auto search_result = this->find(term);
        // count the number of comparisons
        n_comparisons += this->numberComparisons();
        if(!search_result) continue;
        // compute the documents ranks and add then to the heap
        std::vector<Document*> ranked_docs = search_result->computeRanks();
        for(size_t i = 0; i < ranked_docs.size(); i++){
            if(ranked_docs[i] && ranked_docs[i]->rank > 0) heap->insertOrUpdate(ranked_docs[i]);
        }
    }
    comparisons = n_comparisons;

    // get the documents with biggest ranks and add then to the results array
    size_t i = 0;
    while(i < results.size() && !heap->empty()){
        results[i] = heap->pop();
        i++;
    }

    delete heap;
    heap = nullptr;
    return results;
}

bool Dictionary::insert(const json& document){
    std::string term, terms_str = document["headline"];
    // create the document object
    Document* doc_info = new Document(document["category"], document["headline"], document["authors"], document["link"], 
                            document["short_description"], document["date"]);
    
    terms_str += ' ';
    terms_str += document["short_description"];
    doc_info->id = documents.size();
    documents.push_back(doc_info);
    
    // normalize the terms string
    std::string clean_terms_str = normalizeString(terms_str);
    std::stringstream terms_stringstream(clean_terms_str);
    // split the terms string into terms, verify if they are valid and insert into the dictionary
    // with their respective document
    while(std::getline(terms_stringstream, term, ' ')){
        if(!isValidTerm(term)) continue;
        if(!insert(term, doc_info)) return false;
    }
    return true;
}

bool Dictionary::insert(const std::string& path){
    std::ifstream file(path);
    if(file.is_open()){
        std::string raw_doc;
        duration<double> time_elapsed;
        // get all the documents from a file line by line
        while(std::getline(file, raw_doc)){
            // at each 20000 documents save the average insertion time in milliseconds and the virtual memory usage
            if(documents.size() % 20000 == 0){
                time_measures.push_back(std::make_pair(documents.size(), (time_elapsed.count() * 1E3)/20000));
                double vm, rss;
                process_mem_usage(vm, rss);
                mem_measures.push_back(std::make_pair(documents.size(), vm/1024));
            }
            if(verbose){
                std::clog << "Parsing document " << documents.size()+1 << std::endl;
            }
            bool status;
            auto start = high_resolution_clock::now();
            // insert the parsed document into the dictionary
            status = insert(json::parse(raw_doc));
            auto end = high_resolution_clock::now();
            time_elapsed += (end-start);
            if(!status){ 
                std::cerr << "Error inserting document " << documents.size()+1 << ". Error code: " << status << std::endl;
                return false;
            }
        }
    }else{
        std::cerr << "The file could not be opened." << std::endl;
        return false;
    }
    file.close();

    if(verbose){
            std::cout << "Number of unique terms: " << distinct_terms << std::endl;
    }
    // compute the weights of all terms
    computeTermsParameters();
    return true;
}