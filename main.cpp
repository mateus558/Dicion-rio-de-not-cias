#include "Dictionary.hpp"
#include "TRIEDictionary.hpp"
#include "HashDictionary.hpp"
#include <string>
#include <memory>


int main(int argc, char* argv[]){
    std::string path("data/teste.json");
    Dictionary *dict = new HashDictionary();
    //HashDictionary *hash = new HashDictionary();

    dict->setVerbose(false);
    dict->setPrintLimit(20);
    //hash->setPrintLimit(20);

    dict->insert(path);
    //hash->insert(path);

    dict->print();
   // std::cout << *hash << std::endl;

    delete dict;
    //delete hash;
}

