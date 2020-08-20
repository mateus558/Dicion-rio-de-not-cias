#ifndef DICTNODE_HPP_INCLUDED
#define DICTNODE_HPP_INCLUDED

#include "Document.hpp"
#include <map>

class DictNode{
public:
    std::map<size_t, double> weight_i;
    std::map<Document*, size_t, DocumentCompare> docs_counts;
};

#endif