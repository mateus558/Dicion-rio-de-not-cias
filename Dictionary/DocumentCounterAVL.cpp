#include "DocumentCounterAVL.hpp"
#include <algorithm>
#include <iostream>


DocumentCounterAVL::DocumentCounterAVL(){

}

DocCount *DocumentCounterAVL::newNode(Document *doc){
    DocCount* node = new DocCount(); 
    node->doc = doc;  
    node->left = nullptr;  
    node->right = nullptr;  
    node->height = 1;
    node->doc->unique_terms++;
    _size++;
    node->count++;
    n_docs++;
    return node;
}

DocCount* DocumentCounterAVL::rightRotate(DocCount *y){
    DocCount *x = y->left;  
    DocCount *T2 = x->right;  
  
    // Perform rotation  
    x->right = y;  
    y->left = T2;  
  
    // Update heights  
    y->height = std::max(height(y->left), 
                    height(y->right)) + 1;  
    x->height = std::max(height(x->left), 
                    height(x->right)) + 1;  
  
    // Return new root  
    return x;  
}

DocCount* DocumentCounterAVL::leftRotate(DocCount *x){
    DocCount *y = x->right;  
    DocCount *T2 = y->left;  
  
    // Perform rotation  
    y->left = x;  
    x->right = T2;  
  
    // Update heights  
    x->height = std::max(height(x->left),     
                    height(x->right)) + 1;  
    y->height = std::max(height(y->left),  
                    height(y->right)) + 1;  
  
    // Return new root  
    return y; 
}

DocCount* DocumentCounterAVL::insertOrCount_helper(DocCount *root, Document * doc){
    if (root == nullptr)  
        return newNode(doc);  
  
    if (doc->id < root->doc->id){  
        root->left = insertOrCount_helper(root->left, doc);  
    }else if (doc->id > root->doc->id){  
        root->right = insertOrCount_helper(root->right, doc);  
    }else{ // Equal keys are not allowed in BST  
        root->count++;
        return root;  
    }
  
    /* 2. Update height of this ancestor node */
    root->height = 1 + std::max(height(root->left),  
                        height(root->right));  
  
    /* 3. Get the balance factor of this ancestor  
        node to check whether this node became  
        unbalanced */
    int balance = getBalance(root);  
  
    // If this node becomes unbalanced, then  
    // there are 4 cases  
  
    // Left Left Case  
    if (balance > 1 && doc->id < root->left->doc->id)  
        return rightRotate(root);  
  
    // Right Right Case  
    if (balance < -1 && doc->id > root->right->doc->id)  
        return leftRotate(root);  
  
    // Left Right Case  
    if (balance > 1 && doc->id > root->left->doc->id)  
    {  
        root->left = leftRotate(root->left);  
        return rightRotate(root);  
    }  
  
    // Right Left Case  
    if (balance < -1 && doc->id < root->right->doc->id)  
    {  
        root->right = rightRotate(root->right);  
        return leftRotate(root);  
    }  
    /* return the (unchanged) node pointer */
    return root;
}

bool DocumentCounterAVL::insertOrCount(Document* doc){
    root = insertOrCount_helper(root, doc);
    return true;
}

DocCount* DocumentCounterAVL::find(DocCount *root, Document * doc){
    if((root == NULL) || (root->doc->id == doc->id)){  
        return root; 
    } 
    if(root->doc->id > doc->id) return find(root->left, doc);  
    if(root->doc->id < doc->id) return find(root->right, doc);
    return nullptr;
}

size_t& DocumentCounterAVL::operator[](Document * doc){
    return find(root, doc)->count;
}

void DocumentCounterAVL::computeWeights_helper(DocCount* root, size_t &total_documents){ 
    if(root == nullptr){
        return ;
    }else{
        if(n_docs > 0) weights[root->doc->id] = root->count * (log2(total_documents)/n_docs);
    }

    computeWeights_helper(root->left, total_documents);
    computeWeights_helper(root->right, total_documents);
}

void DocumentCounterAVL::computeWeights(size_t total_documents){ 
    computeWeights_helper(root, total_documents);
}

void DocumentCounterAVL::computeRanks_helper(DocCount *root){ 
    if(root == nullptr){
        return ;
    }else{
        auto doc = root->doc;
        doc->rank += ((double)1.0/doc->unique_terms)*weights[doc->id];
        if(doc->rank > 0){
            ranked_docs.push_back(root->doc);
        }
    }

    computeRanks_helper(root->left);
    computeRanks_helper(root->right);
}

std::vector<Document*> DocumentCounterAVL::computeRanks(){ 
    ranked_docs.clear();
    computeRanks_helper(root);
    return ranked_docs;
}

void DocumentCounterAVL::print_helper(DocCount * root){
    if(root == nullptr){
        return ;
    }

    print_helper(root->left);
    std::cout << " (" << root->count << ", " << root->doc->id + 1 << ")";
    print_helper(root->right);
}

void DocumentCounterAVL::print(){ 
    print_helper(root);
}