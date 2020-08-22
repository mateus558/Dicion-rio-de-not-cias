#ifndef DICTIONARY_HPP_INCLUDED
#define DICTIONARY_HPP_INCLUDED

#include "DocumentHeap.hpp"
#include "../Utils/json.hpp"
#include "../Utils/gnuplot-iostream.h"
#include "../Utils/utils.hpp"

#include <map>

using json = nlohmann::json;
using namespace std::chrono;


class DictNode{
public:
    std::map<size_t, double> weight_i;
    std::map<Document*, size_t, DocumentCompare> docs_counts;
};

class Dictionary {
protected:
    bool verbose = true;
    bool print_frequency = true;
    size_t distinct_terms = 0;
    size_t print_limit = 0;
    size_t comparisons = 0;
    std::vector<Document*> documents;
    std::map<unsigned char, char> char_translator;
    std::vector<std::pair<double,double> > time_measures;
    std::vector<std::pair<double, double> > mem_measures;

    virtual void computeTermsParameters() = 0;
    bool isValidTerm(const std::string& term);

public:
    Dictionary();
    std::string processString(const std::string& str);

    void setPrintLimit(size_t limit){ this->print_limit = limit; }
    void setPrintFrequency(bool print_frequency){ this->print_frequency = print_frequency; }
    void setVerbose(bool verbose){ this->verbose = verbose; }
    size_t distinctTerms(){ return distinct_terms; }
    std::vector<std::pair<double,double> > getTimeMeasurements(){ return time_measures; }
    size_t getNumberOfDocuments(){return documents.size();}
    size_t numberComparisons(){ return comparisons;}
    void generatePlots(const std::string& strategy);

    virtual DictNode* find(const std::string& word) = 0;
    virtual DocumentHeap* findByTerms(const std::string& terms);
    virtual bool insert(const std::string& word, Document* doc_info) = 0;
    virtual bool insert(const json& document);
    virtual bool insert(const std::string& path);

    virtual ~Dictionary(){ 
        for(size_t i = 0; i < documents.size(); i++){
            if(documents[i]) delete documents[i];
        }
        documents.clear();
    }
};

#endif