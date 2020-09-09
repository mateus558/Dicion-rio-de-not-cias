#include "DictNode.hpp"
#include <algorithm>
#include <iostream>


DictNode::DictNode(){

}

AVLNode *DictNode::newNode(Document *doc){
    AVLNode* node = new AVLNode(); 
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

AVLNode* DictNode::rightRotate(AVLNode *y){
    AVLNode *x = y->left;  
    AVLNode *T2 = x->right;  
  
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

AVLNode* DictNode::leftRotate(AVLNode *x){
    AVLNode *y = x->right;  
    AVLNode *T2 = y->left;  
  
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

AVLNode* DictNode::insertOrCount_helper(AVLNode *root, Document * doc){
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

bool DictNode::insertOrCount(Document* doc){
    root = insertOrCount_helper(root, doc);
    return true;
}

AVLNode* DictNode::find(AVLNode *root, Document * doc){
    if((root == NULL) || (root->doc->id == doc->id)){  
        return root; 
    } 
    if(root->doc->id > doc->id) return find(root->left, doc);  
    if(root->doc->id < doc->id) return find(root->right, doc);
    return nullptr;
}

void DictNode::computeWeights_helper(AVLNode* root, size_t &total_documents){ 
    if(root == nullptr){
        return ;
    }else{
        if(n_docs > 0) root->weight = root->count * (log2(total_documents)/n_docs);
    }

    computeWeights_helper(root->left, total_documents);
    computeWeights_helper(root->right, total_documents);
}

void DictNode::computeWeights(size_t total_documents){ 
    computeWeights_helper(root, total_documents);
}

void DictNode::computeRanks_helper(AVLNode *root){ 
    if(root == nullptr){
        return ;
    }else{
        // computes the rank for the document
        auto doc = root->doc;
        doc->rank += ((double)1.0/doc->unique_terms)*root->weight;
        if(doc->rank > 0){
            ranked_docs.push_back(root->doc);
        }
    }

    computeRanks_helper(root->left);
    computeRanks_helper(root->right);
}

std::vector<Document*> DictNode::computeRanks(){ 
    ranked_docs.clear();
    computeRanks_helper(root);
    return ranked_docs;
}

void DictNode::print_helper(AVLNode * root){
    if(root == nullptr){
        return ;
    }

    print_helper(root->left);
    std::cout << " (" << root->count << ", " << root->doc->id + 1 << ")";
    print_helper(root->right);
}

void DictNode::print(){ 
    print_helper(root);
}