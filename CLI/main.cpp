#include "Dictionary.hpp"
#include "TRIEDictionary.hpp"
#include "HashDictionary.hpp"
#include <iostream>

std::string strategy;

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cerr << "Available strategies:\n";
        std::cerr << "\ttrie\n";
        std::cerr << "\thash\n"; 
        std::cerr << "Usage: ./news_index <strategy>" << std::endl;
        return 1;
    }else{
        strategy += argv[1];
    }

    Dictionary *dict;
    std::string path("data/News_Category_Dataset_v2.json"), search_terms;

    if(strategy == "hash"){ 
        dict = new HashDictionary();
    }else{
        dict = new TRIEDictionary();
    }

    dict->setVerbose(true);
    dict->insert(path);

    clear();

    std::clog << "Number of documents: " << dict->getNumberOfDocuments() << std::endl;
    std::clog << "Number of distinct terms: " << dict->distinctTerms() << std::endl;
    std::clog << "\n-------------------------------------------------------------------\n\n";
    std::cout << "Search documents: ";
    std::cin >> search_terms;

    dict->findByTerms(search_terms);

    waitUserAction();
}