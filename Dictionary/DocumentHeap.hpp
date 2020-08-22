#ifndef DOCUMENTHEAP_HPP_INCLUDED
#define DOCUMENTHEAP_HPP_INCLUDED

#include "Document.hpp"
#include <vector>

/**
 * \brief Documents max heap
 */
class DocumentHeap{
private:
    /// heap array of document
    std::vector<Document*> documents;
    /// maximum number of elements in the heap
    size_t MAX_SIZE = 0;
    /// actual number of elements in the heap
    size_t _size = 0;

    /**
     *  \brief Move the node up in the tree
     *  \param child index of the child node in the array
     *  \return void 
     */
    void up(size_t child);
    /**
     *  \brief Move the node down in the tree
     *  \param father index of the father node in the array
     *  \return void 
     */
    void down(size_t father);
public:
    /**
     *  \brief heap constructor
     *  \param max_size heap maximum size
     */
    DocumentHeap(const size_t max_size);
    /**
     *  \brief Returns the number of elements in the heap
     *  \return size_t 
     */
    size_t size() const { return _size; }
    /**
     *  \brief returns true if the heap is empty
     *  \return bool 
     */
    bool empty() const { return _size == 0; }
    /**
     *  \brief insert or update a node in the heap
     *  \param doc document to insert or update
     *  \return bool 
     */
    bool insertOrUpdate(Document* doc); 
    /**
     *  \brief update a node in the heap
     *  \param doc document to update
     *  \return bool 
     */
    bool update(Document* doc);
    /**
     *  \brief remove the node with maximum priority
     *  \return Document* 
     */
    Document* pop();
    /**
     *  \brief Returns the node with maximum priority
     *  \return Document* 
     */
    Document* top(){ return *documents.begin();}
    /// class destructor
    ~DocumentHeap();
};

#endif