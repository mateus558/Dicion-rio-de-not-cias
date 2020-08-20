#ifndef DOCUMENT_HPP_INCLUDED
#define DOCUMENT_HPP_INCLUDED

#include <string>

struct Document {
    size_t id = 0;
    double rank = 0.0;
    size_t unique_terms = 0;
    std::string category;
    std::string headline;
    std::string authors;
    std::string link;
    std::string short_description;
    std::string date;

    Document(std::string category, std::string headline, std::string authors, std::string link, 
    std::string short_description, std::string date): category(category), headline(headline), 
    authors(authors), link(link), short_description(short_description), date(date) {}

    bool operator==(const Document &other) const
    { return (id == other.id);
    }
};

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