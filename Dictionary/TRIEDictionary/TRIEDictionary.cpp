#include "TRIEDictionary.hpp"
#include <math.h>
#include <cctype>

TRIEDictionary::TRIEDictionary(){
    root = new TrieNode();
}

bool TRIEDictionary::insert(const std::string& word, Document* doc_info){
    auto current = this->root;

    // go through each word char ignoring spaces and add the word to the TRIE
    for(auto it = word.begin(); it != word.end(); it++){
        char c = *it;
        if(c == ' ') continue;
        // if the char is a number, add it to the children list
        if(c >= '0' && c <= '9'){
            size_t key = c - '0';
            // if it's empty allocate a new children list
            if(!current->children_list){
                current->children_list = new ChildrenList;
                current->children_list->key = key;
                // if it's a leaf, allocate node memory without allocating memory for children array
                if(it == word.end()-1){
                    current->children_list->node = new TrieNode(true);
                }else{
                    current->children_list->node = new TrieNode();
                }
                // update current node
                current = current->children_list->node;
            }else{
                auto curr = current->children_list;
                // find number position in the children list
                while(curr){
                    if(curr->key == key){
                        break;
                    } 
                    curr = curr->next;
                }
                // if not found, add it to the begining of the children list
                if(!curr){
                    auto temp = current->children_list;
                    current->children_list = new ChildrenList;
                    current->children_list->key = key;
                    // if it's a leaf, allocate node memory without allocating memory for children array
                    if(it == word.end()-1){
                        current->children_list->node = new TrieNode(true);
                    }else{
                        current->children_list->node = new TrieNode();
                    }
                    current->children_list->next = temp;
                    // update current node
                    current = current->children_list->node;
                }else{
                    // if found, only update current node
                    current = curr->node;
                }
            }
        }else{
            // if the char is a character, add it to the children array
            size_t key = c - 'a';
            if(!current->children || !current->children[key]){
                // if it's not a internal node, build the children array
                if(!current->children) current->build_children_list();
                if(it == word.end()-1){
                    current->children[key] = new TrieNode(true);    
                }else{
                    current->children[key] = new TrieNode();
                }
            }
            // update current node
            current = current->children[key];
        }
    }
    if(!current->end){ 
        // if its a new end of word, update number of unique words in the document and dictionary
        distinct_terms++;
        documents[doc_info->id]->unique_terms++;
    }
    // mark as end of word
    current->end = true;
    
    // update number of occurrences of the word in the document
    current->docs_counts[doc_info]++;

    return true;
}

void TRIEDictionary::computeTermsParameters(){
    int i = 0;
    // transverse the tree
    computeTermsParameters_helper(root, &i);
}

void TRIEDictionary::computeTermsParameters_helper(TrieNode* root, int *j){ 
    auto current = root;

    // if it's the end of a word, compute it's weights
    if(current->end){
        auto n_docs = current->docs_counts.size();
        if(verbose){
            (*j)++; 
            std::clog << "Computing the weights of the term " << *j << std::endl;
        }
        if(n_docs){
            // go through the list of documents updating the word weights
            for(auto it = current->docs_counts.begin(); it != current->docs_counts.end(); it++){
                current->weight_i[(*it).first->id] = (*it).second*(log2(documents.size())/n_docs);
            }
        }
    }

    // transverse each char of the chars children array
    for(size_t i = 0; i < 26; i++){
        if(current->children && current->children[i]) computeTermsParameters_helper(current->children[i], j);
    }

    // transverse each number of the numbers children array
    auto list_curr = root->children_list;
    while(list_curr){
        computeTermsParameters_helper(list_curr->node, j);
        list_curr = list_curr->next;
    }
}

DictNode* TRIEDictionary::find(const std::string& word){
    auto current = this->root;
    comparisons = 0;
    // go through each word char ignoring spaces and check if the word exists
    for(auto it = word.begin(); it != word.end(); it++){
        char c = *it;
        if(c == ' ') continue;
        // if the char is a number, look at the children list
        if(c >= '0' && c <= '9'){
            size_t key = c - '0';
            if(!current->children_list){
                return nullptr;
            }else{
                // go through the numbers children list
                auto curr = current->children_list;
                while(curr){
                    comparisons++;
                    if(curr->key == key){
                        current = curr->node;
                        break;
                    }
                    curr = curr->next;
                }
            }
        }else{
            // if the char is a character, look at the children array
            size_t key = c - 'a';
            comparisons++;
            if(current->children && current->children[key]){
                current = current->children[key];
            }else{
                return nullptr;
            }
        }
    }

    if(!current->end){
        return nullptr;
    }

    return current;
}

void TRIEDictionary::transverse_helper(TrieNode* current, int i, std::string &buffer, std::vector<WordInfo> &words_info){
    buffer += (char)(i);
    
    // if it's the end, add the string buffer to the words info array
    if(current->end){
        words_info.push_back(std::make_pair(buffer, current));
        return;
    }
    // transverse children array
    for(size_t j = 0; j < 26; j++){
        if(current->children && current->children[j]){
            transverse_helper(current->children[j], j + 'a', buffer, words_info);
            // remove last char added to buffer 
            buffer.pop_back();
        }
    }
    // transverse children list
    auto list_curr = current->children_list;
    while(list_curr){
        transverse_helper(list_curr->node, list_curr->key + '0', buffer, words_info);
        // remove last char added to buffer 
        buffer.pop_back();
        list_curr = list_curr->next;
    }
}

std::vector<WordInfo> TRIEDictionary::transverse(){
    auto current = root;
    std::vector<WordInfo> words_info;

    // transverse children array, reseting the buffer each time the recursion goes back to the root
    for(size_t i = 0; i < 26; i++){
        std::string buffer;
        if(current->children && current->children[i]) transverse_helper(current->children[i], i + 'a', buffer, words_info);
    }
    // transverse children list, reseting the buffer each time the recursion goes back to the root
    auto list_curr = root->children_list;
    while(list_curr){
        std::string buffer;
        transverse_helper(list_curr->node, list_curr->key + '0', buffer, words_info);
        buffer.pop_back();
        list_curr = list_curr->next;
    }

    return words_info;
}

std::ostream & operator << (std::ostream &out, TRIEDictionary &dict) 
{ 
    std::vector<WordInfo> infos = dict.transverse();
    std::vector<WordInfo>::iterator end;
    if(dict.print_limit > infos.size() || dict.print_limit == 0){
        end = infos.end();
    }else{
        end = infos.begin() + dict.print_limit;
    }

    // print a list of words in alphabet order and its docs list
    for(auto it = infos.begin(); it != end; ++it){
        out << (*it).first;
        if(dict.print_frequency){
            for(auto it1 = (*it).second->docs_counts.begin(); it1 != (*it).second->docs_counts.end(); it1++){
                out << " (" << (*it1).second << ", " << (*it1).first->id << ")"; 
            }
        }
        out << std::endl;
    }
    return out; 
} 

TRIEDictionary::~TRIEDictionary(){
    delete root;
}