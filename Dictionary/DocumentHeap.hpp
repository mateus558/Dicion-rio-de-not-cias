#ifndef DOCUMENTHEAP_HPP_INCLUDED
#define DOCUMENTHEAP_HPP_INCLUDED

#include "Document.hpp"
#include <vector>

class DocumentHeap{
private:
    std::vector<Document*> documents;
    size_t MAX_SIZE = 0;
    size_t _size = 0;

    void up(size_t child);
    void down(size_t father);
public:
    DocumentHeap(const size_t max_size);
    size_t size() const { return _size; }
    bool empty() const { return _size == 0; }
    bool insertOrUpdate(Document* doc); 
    bool update(Document* doc);
    Document* pop();
    Document* top(){ return *documents.begin();}
    ~DocumentHeap();
};

#endif