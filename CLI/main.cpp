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

    dict->setVerbose(false);
    dict->insert(path);

    while(true){
        clear();

        std::clog << "Number of documents: " << dict->getNumberOfDocuments() << std::endl;
        std::clog << "Number of distinct terms: " << dict->distinctTerms() << std::endl;
        std::clog << "\n-------------------------------------------------------------------\n\n";
        std::cout << "Search documents: ";
        std::cin >> search_terms;

        auto search_results = dict->findByTerms(search_terms);

        for(size_t i = 0; i < search_results.size(); i++){
            std::cout << "Document: " << search_results[i]->id << " Rank: " << search_results[i]->rank << std::endl;
            std::cout << "Headline: " << search_results[i]->headline << std::endl;
            std::cout << "Short description: " << search_results[i]->short_description << std::endl;
            std::cout << "Link: " << search_results[i]->link << std::endl;
            std::cout << std::endl;
            std::cout << "---------------------------------------------------------------------------------\n";
            std::cout << std::endl;
        }

        std::cout << "Continue to search? [y/n]: ";
        char c;
        std::cin.clear();
        std::cin >> c;
        if(c != 'y'){
            break;
        }
    }
    waitUserAction();
}