#ifndef DICTIONARY_HPP_INCLUDED
#define DICTIONARY_HPP_INCLUDED

#include "DocumentHeap.hpp"
#include "DictNode.hpp"
#include "../Utils/json.hpp"
#include "../Utils/utils.hpp"

#include <map>
#include <chrono>
#include <unordered_map>

using json = nlohmann::json;
using namespace std::chrono;


/**
 * \brief Base class for the document dictionary
 */
class Dictionary {
protected:
    /// control the output to the console
    bool verbose = true;
    /// if true, print the inverted index
    bool print_frequency = true;
    /// the number of unique terms in the dictionary
    size_t distinct_terms = 0;
    /// number of elements to be printed
    size_t print_limit = 0;
    /// the number of comparisons in the last query
    size_t comparisons = 0;
    /// array of documents in the dictionary
    std::vector<Document*> documents;
    /// maps accented characters to non-accented characters
    std::map<unsigned char, char> char_translator;
    /// array of tuples containing the time and the step of it
    std::vector<std::pair<double,double> > time_measures;
    /// array of tuples containing the memory usage and the step of it
    std::vector<std::pair<double, double> > mem_measures;
    /// a max heap that contains the results to be returned in a query
    DocumentHeap *heap = nullptr;


    /**
     * \brief Compute the terms weights according to the derived class strategy.
     * \return int
     */
    virtual void computeTermsParameters() = 0;

    void addToDocsCounts(Document* doc, DictNode *node);
    
public:
    Dictionary();
    /**
     * \brief Returns if a term is valid.
     * \param term string to be verified.
     * \return bool
     */
    bool isValidTerm(const std::string& term);
    /**
     * \brief Normalize a string removing accents, numbers and transforming uppercase to lower case
     * \param str string to be normalized.
     * \return std::string
     */
    std::string normalizeString(const std::string& str);
    /**
     * \brief Set a limit for the console output.
     * \param limit number of terms to be printed.
     * \return void
     */
    void setPrintLimit(size_t limit){ this->print_limit = limit; }
    /**
     * \brief Set if the list of documents of a term must be printed.
     * \param print_frequency true prints the list, otherwise not.
     * \return void
     */
    void setPrintFrequency(bool print_frequency){ this->print_frequency = print_frequency; }
    /**
     * \brief Controls the output of the dictionary.
     * \param verbose prints the dictionary log.
     * \return void
     */
    void setVerbose(bool verbose){ this->verbose = verbose; }
    /**
     * \brief Returns the number of unique terms in the dictionary.
     * \return size_t
     */
    size_t distinctTerms(){ return distinct_terms; }
    /**
     * \brief Returns the array of time measurements at each 20000 documents.
     * \return vector of pairs where first is the step and second is the time measurement
     */
    std::vector<std::pair<double,double> > getTimeMeasurements(){ return time_measures; }
    /**
     * \brief Returns the number of documents in the dictionary.
     * \return size_t
     */
    size_t getNumberOfDocuments(){return documents.size();}
    /**
     * \brief Returns the number of comparisons in the last query.
     * \return size_t
     */
    size_t numberComparisons(){ return comparisons;}

    /**
     * \brief Find a term in the dictionary and return it, implemented according with the derived class strategy.
     * \return DictNode*
     */
    virtual DictNode* find(const std::string& word) = 0;
    /**
     * \brief Returns an array of the documents with the best ranks according to the query string.
     * \param terms string with the query terms.
     * \param n_results number of results to return.
     * \return std::vector<Document*>
     */
    virtual std::vector<Document*> findByTerms(const std::string& terms, const size_t &n_results);
    /**
     * \brief Insert a word and its associated document into the dictionary.
     * \param word term to be inserted.
     * \param doc_info document object.
     * \return bool
     */
    virtual bool insert(const std::string& word, Document* doc_info) = 0;
    /**
     * \brief Insert a document into the dictionary.
     * \param document document to be inserted.
     * \return bool
     */
    virtual bool insert(const json& document);
    /**
     * \brief Insert a dataset of documents into the dictionary.
     * \param path path to the dataset to be inserted.
     * \return bool
     */
    virtual bool insert(const std::string& path);
    /**
     * \brief Generate the insertion time and memory consumption plots with gnuplot.
     * \param dictionary Dictionary with the necessary information for the plot.
     * \param strategy Strategy used by the dictionary.
     * \return void
     */
    friend void generatePlots(Dictionary* dictionary, const std::string &strategy);

    virtual void print() = 0;
    virtual ~Dictionary(){ 
        for(size_t i = 0; i < documents.size(); i++){
            if(documents[i]) delete documents[i];
        }
        documents.clear();
    }
};

#endif