//
// Created by Lenovo on 2017/12/13.
//

#include "constant.h"
#include "structure.h"

void binarySearch(BTreeNode &root, KeyType k, Result &result){

    int left = 0,right = root->num-1;
    while (right>=left){
        int mid = (left+right)/2;

        if(k == root->key[mid]){
            result->tag = Success;
            result->index = mid;
            return ;
        }
        if(root->key[mid]<k)
            left = mid+1;
        else
            right = mid-1;
    }
    result->index=left;
}

BTreeNode SearchNode(BTreeNode root,KeyType k,Result &result){

    BTreeNode bTreeNode = root;
    while (bTreeNode!= nullptr){
        binarySearch(bTreeNode, k, result);
        result->node = bTreeNode;
        if(result->tag==Success || result->index==MIN_INT || bTreeNode->child[result->index]== nullptr) {
            break;
        }
        bTreeNode = bTreeNode->child[result->index];
    }
    return bTreeNode;
}