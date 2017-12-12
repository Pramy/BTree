//
// Created by Pramy on 2017/12/12.
//


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "delete.h"


int btreeDeleteByOrder(BTree bTree,BTreeNode bTreeNode, int index);

int mergeNode(BTree bTree,BTreeNode node);

int mergeNodeByOrder(BTree bTree,BTreeNode leftNode,BTreeNode rightNode, int mid);

int btreeDelete(BTree &bTree,KeyType key){

    Result result{};
    createResult(result);

    BTreeNode node = SearchNode(bTree->root,key,result);
    if(result->tag!=Success)
        return Failure;
    return btreeDeleteByOrder(bTree,result->node,result->index);

}


int btreeDeleteByOrder(BTree bTree,BTreeNode bTreeNode, int index){

    BTreeNode node = bTreeNode , childNode = node->child[index];

    while (childNode!= nullptr){
        node = childNode;
        childNode = childNode->child[childNode->num];
    }

    bTreeNode->key[index] = node->key[node->num-1];

    node->num--;

    node->key[node->num] = 0;

    if(node->num < bTree->min){
        return mergeNode(bTree,node);
    }
    return Success;
}

int mergeNode(BTree bTree,BTreeNode node){

    int index = 0, m = 0, mid = 0;
    BTreeNode parent = node->parent, rightNode = nullptr, leftNode = nullptr;

    /**
     * 对根节点的处理
     */
    if(parent== nullptr){
        if( node->num == 0){
            //node节点为叶子节点，应该孩子节点全为null
            if(node->child[0] != nullptr){
                bTree->root = node->child[0];
                node->child[0]->parent = nullptr;
            }else{
                bTree->root = nullptr;
            }
            free(node);
        }
        return Success;
    }

    /**
     *  查找node是其父结点的第几个孩子结点
     */
    for (index = 0; index <=parent->num ; ++index) {
        if(parent->child[index] == node){
            break;
        }
    }

    if(index > parent->num) {
        fprintf(stderr, "[%s][%d] Didn't find node in parent's children array!\n", __FILE__, __LINE__);
        return INT_MIN;
    }

    /**
     * 当node为最右的节点时
     */
    if(index == parent->num){
        mid = index-1;
        leftNode = parent->child[mid];
        /**
         * 如果左节点不够，就合并
         */
        if(leftNode->num  <=bTree->min)
            return mergeNodeByOrder(bTree,leftNode,node,mid);

        //向左儿子借
        for ( m= node->num;m > 0;m--){
            node->key[m] = node->key[ m -1];
            node->child[m+1] = node->child[m];
        }

        node->child[1] = node->child[0];

        node->key[0] = parent->key[mid];
        node->num++;
        node->child[0] = leftNode->child[leftNode->num];

        if(leftNode->child[leftNode->num]!= nullptr){
            leftNode->child[leftNode->num]->parent = node;
        }

        parent->key[mid] = leftNode->key[leftNode->num - 1];
        leftNode->key[leftNode->num - 1] = 0;
        leftNode->child[leftNode->num] = nullptr;
        leftNode->num--;
        return Success;
    }

    if(index < parent->num){
        mid = index;

        rightNode = parent->child[mid+1];

        if(rightNode->num<=bTree->min){
            return mergeNodeByOrder(bTree,node,rightNode,mid);
        }

        //向右儿子借
        node->key[node->num++] = parent->key[mid];
        node->child[node->num] = rightNode->child[0];

        if(rightNode->child[0]!= nullptr){
            rightNode->child[0]->parent = node;
        }

        parent->key[mid] = rightNode->key[0];
        for(m=0; m<rightNode->num; m++) {
            rightNode->key[m] = rightNode->key[m+1];
            rightNode->child[m] = rightNode->child[m+1];
        }
        rightNode->child[m] = nullptr;
        rightNode->num--;
        return Success;
    }

}

int mergeNodeByOrder(BTree bTree,BTreeNode leftNode,BTreeNode rightNode, int mid){

    int m = 0;
    BTreeNode parent = leftNode->parent;
    leftNode->key[leftNode->num++] = parent->key[mid];

    memcpy(leftNode->key + leftNode->num, rightNode->key, rightNode->num*sizeof(KeyType));
    memcpy(leftNode->child + leftNode->num, rightNode->child, (rightNode->num+1)*sizeof(BTreeNode));
    for(m=0; m<=rightNode->num; m++) {
        if(nullptr != rightNode->child[m]) {
            rightNode->child[m]->parent = leftNode;
        }
    }
    leftNode->num += rightNode->num;

    for(m=mid; m<parent->num-1; m++) {
        parent->key[m] = parent->key[m+1];
        parent->child[m+1] = parent->child[m+2];
    }

    parent->key[m] = 0;
    parent->child[m+1] = nullptr;
    parent->num--;
    free(rightNode);

    if(parent->num < bTree->min) {
        return mergeNode(bTree, parent);
    }

    return Success;
}