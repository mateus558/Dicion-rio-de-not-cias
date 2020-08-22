#ifndef DOCUMENT_HPP_INCLUDED
#define DOCUMENT_HPP_INCLUDED

#include <string>
/**
 * \brief Class for the document information
 */
struct Document {
    /// document identifier
    size_t id = 0;
    /// inform the document position in the heap, if -1 then its not in the heap
    int heap_id = -1;
    /// Rank of the document given the query terms
    double rank = 0.0;
    /// number of unique terms in the document
    size_t unique_terms = 0;
    /// category of the document
    std::string category;
    /// headline of the document
    std::string headline;
    /// authors of the document
    std::string authors;
    /// link to the document
    std::string link;
    /// document short description
    std::string short_description;
    /// date of publication of the document
    std::string date;

    Document(std::string category, std::string headline, std::string authors, std::string link, 
    std::string short_description, std::string date): category(category), headline(headline), 
    authors(authors), link(link), short_description(short_description), date(date) {}

    bool operator==(const Document &other) const
    { return (id == other.id);
    }
};

/// structures used on stl data structures and algorithms for document comparison
struct DocumentCompare
{
   bool operator() (const Document* lhs, const Document* rhs) const
   {
       return lhs->unique_terms < rhs->unique_terms;
   }
};

struct DocumentRankCompare
{
   bool operator() (const Document* lhs, const Document* rhs) const
   {
       return lhs->rank < rhs->rank;
   }
};

#endif