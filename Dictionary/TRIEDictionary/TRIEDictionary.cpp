#include "TRIEDictionary.hpp"
#include <sstream>
#include <math.h>
#include <cctype>
#include <algorithm>

TRIEDictionary::TRIEDictionary(){
    root = new TrieNode();
}

bool TRIEDictionary::insert(const std::string& word, Document* doc_info){
    auto current = this->root;

    for(auto it = word.begin(); it != word.end(); it++){
        char c = *it;
        if(c == ' ') continue;

        if(c >= 48 && c <= 57){
            size_t key = c - '0';
            if(!current->children_list){
                current->children_list = new ChildrenList;
                current->children_list->key = key;
                if(it == word.end()-1){
                    current->children_list->node = new TrieNode(true);
                }else{
                    current->children_list->node = new TrieNode();
                }
                current = current->children_list->node;
            }else{
                auto curr = current->children_list;
                while(curr){
                    if(curr->key == key){
                        break;
                    } 
                    curr = curr->next;
                }
                if(!curr){
                    auto temp = current->children_list;
                    current->children_list = new ChildrenList;
                    current->children_list->key = key;
                    if(it == word.end()-1){
                        current->children_list->node = new TrieNode(true);
                    }else{
                        current->children_list->node = new TrieNode();
                    }
                    current->children_list->next = temp;
                    current = current->children_list->node;
                }else{
                    current = curr->node;
                }
            }
        }else{
            size_t key = c - 'a';
            if(!current->children || !current->children[key]){
                if(!current->children) current->build_children_list();
                if(it == word.end()-1){
                    current->children[key] = new TrieNode(true);    
                }else{
                    current->children[key] = new TrieNode();
                }
            }
            current = current->children[key];
        }
    }
    if(!current->end){ 
        distinct_terms++;
        documents[doc_info->id]->unique_terms++;
    }
    current->end = true;
    
    
    current->docs_counts[doc_info]++;

    return true;
}

void TRIEDictionary::computeTermsParameters(){
    int i = 0;
    computeTermsParameters_helper(root, &i);
}

void TRIEDictionary::computeTermsParameters_helper(TrieNode* root, int *j){ 
    auto current = root;

    if(current->end){
        auto n_docs = current->docs_counts.size();
        if(verbose){
            (*j)++; 
            std::clog << "Computing the weights of the term " << *j << std::endl;
        }
        if(n_docs){
            for(auto it = current->docs_counts.begin(); it != current->docs_counts.end(); it++){
                current->weight_i[(*it).first->id] = (*it).second*(log2(documents.size())/n_docs);
            }
        }
    }

    for(size_t i = 0; i < 26; i++){
        if(current->children && current->children[i]) computeTermsParameters_helper(current->children[i], j);
    }

    auto list_curr = root->children_list;
    while(list_curr){
        computeTermsParameters_helper(list_curr->node, j);
        list_curr = list_curr->next;
    }
}

DictNode* TRIEDictionary::find(const std::string& word){
    auto current = this->root;
    comparisons = 0;
    for(auto it = word.begin(); it != word.end(); it++){
        char c = *it;
        if(c == ' ') continue;
        if(c >= 48 && c <= 57){
            size_t key = c - '0';
            if(!current->children_list){
                return nullptr;
            }else{
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
    
    if(current->end){
        words_info.push_back(std::make_pair(buffer, current));
        return;
    }
    for(size_t j = 0; j < 26; j++){
        if(current->children && current->children[j]){
            transverse_helper(current->children[j], j + 'a', buffer, words_info);
            buffer.pop_back();
        }
    }

    auto list_curr = current->children_list;
    while(list_curr){
        transverse_helper(list_curr->node, list_curr->key + '0', buffer, words_info);
        buffer.pop_back();
        list_curr = list_curr->next;
    }
}

std::vector<WordInfo> TRIEDictionary::transverse(){
    auto current = root;
    std::vector<WordInfo> words_info;

    for(size_t i = 0; i < 26; i++){
        std::string buffer;
        if(current->children && current->children[i]) transverse_helper(current->children[i], i + 'a', buffer, words_info);
    }

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
    for(size_t i = 0; i < documents.size(); i++){
        delete documents[i];
    }
}