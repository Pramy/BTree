//
// Created by Pramy on 2017/12/12.
//


#include <cstdio>
#include <cstring>
#include "insert.h"

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

void SearchNode(BTreeNode &root,KeyType k,Result &result){

    while (root!= nullptr){
        binarySearch(root, k, result);
        result->node = root;
        if(result->tag==Success || result->index==MIN_INT || root->child[result->index]== nullptr) {
            break;
        }
        root = root->child[result->index];
    }

}

int splitNode(BTree &bTree, BTreeNode &node){

    int splitIndex = bTree->splitIndex;
    int index = 0 , total = 0;
    BTreeNode parent = nullptr;
    BTreeNode increaseNode = nullptr;

    Result result{};
    createResult(result);

    while (node->num>bTree->max){
        total = node->num;

        createNode(bTree,increaseNode);

        if(increaseNode== nullptr){
            fprintf(stderr, "[%s][%d] Create increaseNode failed!\n", __FILE__, __LINE__);
            return Failure;
        }
        memcpy(increaseNode->key,node->key+splitIndex+1,(total-splitIndex-1)* sizeof(KeyType));
        memcpy(increaseNode->child,node->child+splitIndex+1,(total-splitIndex)* sizeof(BTreeNodeChild));

        increaseNode->num = total-splitIndex-1;
        increaseNode->parent = node->parent;

        node->num = splitIndex;

        parent = node->parent;

        /**
         * 此时parent为根节点
         */
        if(parent== nullptr){

            createNode(bTree,parent);

            if(parent== nullptr){
                fprintf(stderr, "[%s][%d] Create parentNode failed!\n", __FILE__, __LINE__);
                return Failure;
            }

            bTree->root = parent;
            node->parent = parent;
            increaseNode->parent = parent;

            parent->child[0] = node;
            parent->child[1] = increaseNode;

            parent->key[0] = node->key[splitIndex];
            parent->num++;
        }else{

            binarySearch(parent, node->key[splitIndex], result);
            index = result->index;
            memcpy(parent->key+index+1,parent->key+index, (parent->num-index)*sizeof(KeyType));
            memcpy(parent->child+index+1,parent->child+index, (parent->num-index+1)*sizeof(BTreeNodeChild));
            parent->key[index]=node->key[splitIndex];
            parent->child[index+1] = increaseNode;
            increaseNode->parent = parent;
            parent->num++;
        }

        memset(node->key+splitIndex, 0, (total - splitIndex) * sizeof(KeyType));
        memset(node->child+splitIndex+1, 0, (total - splitIndex) * sizeof(BTreeNodeChild));

        for(index=0; index<=increaseNode->num; index++) {
            if(nullptr != increaseNode->child[index]) {
                increaseNode->child[index]->parent = increaseNode;
            }
        }
        node = parent;
    }
    destroyResult(result);
}

int BTreeInsertByOrder(BTree &bTree,BTreeNode &node,KeyType key, int index){

    for(int i=node->num; i>index; i--) {
        node->key[i] = node->key[i-1];
    }
    node->key[index] = key;
    node->num++;

    if(node->num>bTree->max){
        return splitNode(bTree,node);
    }

    return Success;
}

int BTreeInsert(BTree &bTree,KeyType key){

    int index = 0;
    if(bTree== nullptr){
        fprintf(stderr,"[%s][%d] Parameter 'btree' can not be null\n", __FILE__, __LINE__);
        return Failure;
    }
    BTreeNode node = bTree->root;
    /**
     * 空树，构建根节点
     */
    if(node== nullptr){
        int flag = createNode(bTree,node);

        if(flag==Failure){
            fprintf(stderr, "[%s][%d] Create node failed!\n", __FILE__, __LINE__);
            return Failure;
        }
        node->num=1;
        //0开始还是1开始
        node->key[0] = key;
        node->parent = nullptr;
        bTree->root  = node;
        return Success;
    }
    Result result = {};
    createResult(result);

    SearchNode(node,key,result);
    if(result->tag==Success) {
        printf("key:[%d] already exits !\n",key);
        destroyResult(result);
        return Failure;
    }
    index = result->index;
    destroyResult(result);
    return BTreeInsertByOrder(bTree,node,key,index);
}


