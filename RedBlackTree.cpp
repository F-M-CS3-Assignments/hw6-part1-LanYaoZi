#include <iostream>
#include <string>
#include "RedBlackTree.h"

using namespace std;

RedBlackTree::RedBlackTree(){
    //I think I should not add any node and leave root empty there
}

RedBlackTree::RedBlackTree(int newData){
    RBTNode *tem_node=new RBTNode;
    tem_node->data=newData;
    tem_node->color=COLOR_BLACK;
    this->root=tem_node;
    this->numItems+=1;
}

RBTNode *copy_tree(RBTNode*parent,const RBTNode* copy_one){
    if(copy_one==nullptr){
        return nullptr;
    }
    RBTNode *new_node=new RBTNode;
    new_node->data=copy_one->data;
    new_node->color=copy_one->color;
    new_node->parent=parent;
    new_node->left=copy_tree(new_node,copy_one->left);
    new_node->right=copy_tree(new_node,copy_one->right);
    return new_node;
}
RedBlackTree::RedBlackTree(const RedBlackTree &rbt){
    this->root=copy_tree(nullptr,rbt.root);
    this->numItems=rbt.numItems;
}

void BasicInsert_recur(RBTNode *node,RBTNode *position){
    if(position->data>node->data){
        if(position->left==nullptr){
            position->left=node;
            node->parent=position;
            return;
        }
        BasicInsert_recur(node,position->left);
    }
    if(position->data<=node->data){
        if(position->right==nullptr){
            position->right=node;
            node->parent=position;
            return;
        }
        BasicInsert_recur(node,position->right);
    }
}

void RedBlackTree::BasicInsert(RBTNode *node){
    BasicInsert_recur(node,this->root);
}

bool RedBlackTree::IsLeftChild(RBTNode *node) const{
    RBTNode *parent=node->parent;
    if(parent->left==node){
        return true;
    }
    return false;
}
bool RedBlackTree::IsRightChild(RBTNode *node) const{
    RBTNode *parent=node->parent;
    if(parent->right==node){
        return true;
    }
    return false;
}

RBTNode* RedBlackTree::GetUncle(RBTNode *node)const{
    RBTNode *parent=node->parent;
    RBTNode *grand_parent = parent->parent;
    if(IsLeftChild(parent)){
        return grand_parent->right;
    }
    if(IsRightChild(parent)){
        return grand_parent->left;
    }
    return nullptr;
}

void RedBlackTree::InsertFixUp(RBTNode *new_node){
    RBTNode *parent = new_node->parent;
    RBTNode *uncle = GetUncle(new_node);
    RBTNode *grand_parent = parent->parent;
    if(uncle==nullptr || uncle->color==COLOR_BLACK){
        // Case 2, uncle is BLACK. nullptr also counts as black
        // This happens in every case
        if(grand_parent != nullptr){
            grand_parent->color = COLOR_RED;
        }
        if(IsLeftChild(new_node) && IsLeftChild(parent)){
            // Left Left
            RightRotate(grand_parent);
            parent->color = COLOR_BLACK;
        } else if(IsRightChild(new_node) && IsRightChild(parent)){
            // Right Right
            LeftRotate(grand_parent);
            parent->color = COLOR_BLACK;
        } else if(IsLeftChild(new_node) && IsRightChild(parent)) {
            // Left Right
            RightRotate(parent);
            LeftRotate(grand_parent);
            new_node->color = COLOR_BLACK;
            parent->color = COLOR_RED;
        } else if(IsRightChild(new_node) && IsLeftChild(parent)){
            // Right Left
            LeftRotate(parent);
            RightRotate(grand_parent);
            new_node->color = COLOR_BLACK;
            parent->color = COLOR_RED;
        } else {
            //output error message and throw an exception
            throw invalid_argument("impossible state!");
        }
    }
    else if(uncle->color==COLOR_RED){
        // Case 6, uncle is RED
        parent->color = COLOR_BLACK;
        uncle->color = COLOR_BLACK;
        if(grand_parent != nullptr){
            if(root != grand_parent){
                grand_parent->color = COLOR_RED;
                if(grand_parent->parent != nullptr){
                        if(grand_parent->parent->color == COLOR_RED){
                            InsertFixUp(grand_parent);
                        }
                    }
                }
            }
        }
    }

void  RedBlackTree::LeftRotate(RBTNode *node){
    
	//cout<<"LeftRotate"<<endl;
    RBTNode *grand_parent = node;
    RBTNode *parent = node->right;
    if(parent->left!=nullptr){
        grand_parent->right=parent->left;
        parent->left->parent=grand_parent;
        parent->left=nullptr;
    }else{   
        grand_parent->right=nullptr;
    }
    if(grand_parent==this->root){
        root=parent;
        parent->parent=nullptr;
    }else{
        parent->parent=grand_parent->parent;
        if(IsLeftChild(grand_parent)){
            parent->parent->left=parent;
        }else if(IsRightChild(grand_parent)){
            parent->parent->right=parent;
        }
    }
    grand_parent->parent=parent;
    parent->left=grand_parent;
}

void RedBlackTree::RightRotate(RBTNode *node){

    //cout<<"RightRotate"<<endl;
    RBTNode *grand_parent = node;
    RBTNode *parent = node->left;
    if(parent->right!=nullptr){
        grand_parent->left=parent->right;
        parent->right->parent=grand_parent;
        parent->right=nullptr;
    }else{   
        grand_parent->left=nullptr;
    }
    if(grand_parent==this->root){
        root=parent;
        parent->parent=nullptr;
    }else{
        parent->parent=grand_parent->parent;
        if(IsRightChild(grand_parent)){
            parent->parent->right=parent;
        }else if(IsLeftChild(grand_parent)){
            parent->parent->left=parent;
        }
    }
    grand_parent->parent=parent;
    parent->right=grand_parent;
}

void RedBlackTree::Insert(int newData){
    //cout<<"Insert:"<<newData<<endl;
    RBTNode *tem_node=new RBTNode;
    tem_node->data=newData;
    if(this->root==nullptr){
        tem_node->color=COLOR_BLACK;
        this->root=tem_node;
        this->numItems+=1;
    }else{
        tem_node->color=COLOR_RED;
        BasicInsert(tem_node);
        if(tem_node->parent!=nullptr && tem_node->parent->color==COLOR_RED){
            InsertFixUp(tem_node);
        }
        this->numItems+=1;
    } 
}

bool contains_recur(RBTNode *start_node,int data){
    if(start_node==nullptr){
        return false;
    }
    if(start_node->data==data){
        return true;
    }
    if(start_node->left!=nullptr){
        if (contains_recur(start_node->left,data)){
            return true;
        }
    }
    if(start_node->right!=nullptr){
        if (contains_recur(start_node->right,data)){
            return true;
        }
    }
    return false;
}

bool RedBlackTree::Contains(int data) const{
    return contains_recur(this->root,data);
}

int GetMin_recur(RBTNode *start_node,int data){
    int tem;
    if(start_node->data<data){
        data=start_node->data;
    }
    if(start_node->left!=nullptr){
        tem=GetMin_recur(start_node->left,data);
        if(tem<data){
            data=tem;
        }
    }
    if(start_node->right!=nullptr){
        tem=GetMin_recur(start_node->right,data);
        if(tem<data){
            data=tem;
        }
    }
    return data;
}

int RedBlackTree::GetMin() const{
    return GetMin_recur(this->root,INT_MAX);
}

int GetMax_recur(RBTNode *start_node,int data){
    int tem;
    if(start_node->data>data){
        data=start_node->data;
    }
    if(start_node->left!=nullptr){
        tem=GetMax_recur(start_node->left,data);
        if(tem>data){
            data=tem;
        }
    }
    if(start_node->right!=nullptr){
        tem=GetMax_recur(start_node->right,data);
        if(tem>data){
            data=tem;
        }
    }
    return data;
}

int RedBlackTree::GetMax() const{
    return GetMax_recur(this->root,INT_MIN);
}

string RedBlackTree::ToInfixString(const RBTNode *n){
    
    if(n==nullptr){
        return "";
    }
    string outcome="";
    if(n->left!=nullptr){
        outcome+=ToInfixString(n->left);
    }
    outcome+=" ";
    if(n->color==COLOR_RED){
        outcome+="R";
    }else{
        outcome+="B";
    }
    outcome+=to_string(n->data);
    outcome+=" ";
    if(n->right!=nullptr){
        outcome+=ToInfixString(n->right);
    }
    return outcome;
}
string RedBlackTree::ToPrefixString(const RBTNode *n){
    if(n==nullptr){
        return "";
    }
    string outcome="";
    outcome+=" ";
    if(n->color==COLOR_RED){
        outcome+="R";
    }else{
        outcome+="B";
    }
    outcome+=to_string(n->data);
    outcome+=" ";
    if(n->left!=nullptr){
        outcome+=ToPrefixString(n->left);
    }
    if(n->right!=nullptr){
        outcome+=ToPrefixString(n->right);
    }
    return outcome;
}
string RedBlackTree::ToPostfixString(const RBTNode *n){
    if(n==nullptr){
        return "";
    }
    string outcome="";
    if(n->left!=nullptr){
        outcome+=ToPostfixString(n->left);
    }
    if(n->right!=nullptr){
        outcome+=ToPostfixString(n->right);
    }
    outcome+=" ";
    if(n->color==COLOR_RED){
        outcome+="R";
    }else{
        outcome+="B";
    }
    outcome+=to_string(n->data);
    outcome+=" ";
    return outcome;
}