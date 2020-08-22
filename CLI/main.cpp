#include "Dictionary.hpp"
#include "TRIEDictionary.hpp"
#include "HashDictionary.hpp"

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
    std::string path("data/"), fname, search_terms;
    std::cout << "Enter the dataset file name: ";
    std::cin >> fname;
    path += fname;

    if(strategy == "hash"){ 
        dict = new HashDictionary();
    }else{
        dict = new TRIEDictionary();
    }

    clear();
    dict->setVerbose(false);
    std::cout << "Loading dataset..." << std::endl;
    if(!dict->insert(path)){
        std::cerr << "Error loading the dataset!" << std::endl;
        return 1;
    }

    while(true){
        //clear();

        std::clog << "Number of documents: " << dict->getNumberOfDocuments() << std::endl;
        std::clog << "Number of distinct terms: " << dict->distinctTerms() << std::endl;
        std::clog << "\n-------------------------------------------------------------------\n\n";
        std::cout << "Search documents: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, search_terms);

        auto search_results = dict->findByTerms(search_terms, 20);
        if(search_results.empty()) return 1;
        
        for(size_t i = 0, id = 0; i < search_results.size(); i++, ++id){
            auto news = search_results[i];
            std::cout << "[" << id+1 << "] Document: " << news->id << " Rank: " << news->rank << std::endl;
            std::cout << "Headline: " << news->headline << std::endl;
            std::cout << "Short description: " << news->short_description << std::endl;
            std::cout << "Link: " << news->link << std::endl;
            std::cout << std::endl;
            std::cout << "---------------------------------------------------------------------------------\n";
            std::cout << std::endl;
        }
        std::cin.clear();
        std::cout << "Continue to search? [y/n]: ";
        std::string opt;
        std::cin >> opt;
        if(opt != "y"){
            break;
        }
    }
    waitUserAction();
}