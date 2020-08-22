#include "DocumentHeap.hpp"

DocumentHeap::DocumentHeap(const size_t max_size){
    this->MAX_SIZE = max_size;
    // initialize the heap with null entries
    this->documents.assign(MAX_SIZE, nullptr);
}

void DocumentHeap::up(size_t child){ 
    size_t father = (child-1)/2;
    // swap child and father while father rank is smaller than child rank
    while((child > 0) && (documents[father]->rank <= documents[child]->rank)){
        auto temp = documents[child];
        size_t fid = documents[father]->heap_id;
        size_t cid = documents[child]->heap_id;
        
        documents[child] = documents[father];
        documents[father] = temp;
        // swap father and child heap ids
        documents[father]->heap_id = fid;
        documents[child]->heap_id = cid;

        child = father;
        father = (father-1)/2;
    }

}

void DocumentHeap::down(size_t father){
    size_t child = 2 * father + 1;

    while(child < _size){
        if(child < _size-1){
            // compare with the last element
            if(documents[child]->rank < documents[child+1]->rank){
                child++;
            }
        }
        // if father's rank is bigger, break while
        if(documents[father]->rank >= documents[child]->rank){
            break;
        }

        // swap father and child
        auto temp = documents[father];
        size_t fid = documents[father]->heap_id;
        size_t cid = documents[child]->heap_id;
        documents[father] = documents[child];
        documents[child] = temp;
        documents[father]->heap_id = fid;
        documents[child]->heap_id = cid;
        // update father and child
        father = child;
        child = 2 * father + 1;
    }

}

bool DocumentHeap::update(Document* doc){
    // if the rank is the same, do nothing
    if(doc->rank == documents[doc->heap_id]->rank) return false;
    // if is bigger, move the node up in the heap
    if(doc->rank > documents[doc->heap_id]->rank){
        up(doc->heap_id);
    // otherwise, move down in the heap
    }else{
        down(doc->heap_id);
    }
    return true;
}

bool DocumentHeap::insertOrUpdate(Document* doc){
    if(!doc) return false;
    if(_size == MAX_SIZE) return false;
    // if the document is already in the heap, update it
    if(doc->heap_id > -1){
        return update(doc); 
    }
    // set heap id to its position
    doc->heap_id = _size;
    documents[_size] = doc;
    // move entry up in the heap
    up(_size);
    _size++;
    return true;
}

Document* DocumentHeap::pop(){ 
    if(!_size) return nullptr;
    // reset heap index
    documents[0]->heap_id = -1;
    auto begin = documents[0];
    _size--;
    // put last document to first position and fix heap
    documents[0] = documents[_size];
    down(0);
    return begin;
}

DocumentHeap::~DocumentHeap(){
    // reset documents heap indexes
    for(size_t i = 0; i < documents.size(); i++){
        if(documents[i]) documents[i]->heap_id = -1;
    }
}