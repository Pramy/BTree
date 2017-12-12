//
// Created by Pramy on 2017/12/12.
//

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "structure.h"

/**
 * 创建b树
 * @param tree ：b树
 * @param m :阶数
 * @return status：状态
 */
int createBTree(BTree &tree, int m){

    if(m<3){
        fprintf(stderr, "[%s][%d] Parameter 'max' must greater than 2.\n", __FILE__, __LINE__);
        return Failure;
    }

    tree = (BTree)calloc(1, sizeof(Tree));

    if(tree == nullptr){
        fprintf(stderr, "[%s][%d] errmsg:[%d] %s!\n", __FILE__, __LINE__, errno, strerror(errno));
        return Failure;
    }
    tree->max = m - 1;
    tree->min = m/2-1;
    if(m%2 != 0)
        tree->min++;
    tree->splitIndex = m/2;
    tree->root = nullptr;//空树
    return Success;
}

void destroyTree(BTree &bTree){
    if(bTree== nullptr)
        return;
    bTree->root = nullptr;
    free(bTree);
    bTree= nullptr;
}

int createNodeByOrder(BTree &bTree, BTreeNode &bTreeNode, int recordSize){

    int size = bTree->max+2;

    bTreeNode = (BTreeNode)malloc(sizeof(BNode));

    if(bTreeNode == nullptr){
        fprintf(stderr, "[%s][%d] errmsg:[%d] %s\n", __FILE__, __LINE__, errno, strerror(errno));
        return Failure;
    }

    bTreeNode->num=0;

    bTreeNode->key = (KeyType *)malloc(size*sizeof(KeyType));

    if(bTreeNode->key == nullptr){
        free(bTreeNode);
        bTreeNode= nullptr;
        fprintf(stderr, "[%s][%d] errmsg:[%d] %s\n", __FILE__, __LINE__, errno, strerror(errno));
        return Failure;
    }
    memset(bTreeNode->key,0,(size)* sizeof(KeyType));

    bTreeNode->child = (BTreeNodeChild)malloc(size*sizeof(BTreeNodeChild));

    if(bTreeNode->child == nullptr){
        free(bTreeNode->key);
        bTreeNode->key= nullptr;
        free(bTreeNode);
        bTreeNode = nullptr;
        fprintf(stderr, "[%s][%d] errmsg:[%d] %s\n", __FILE__, __LINE__, errno, strerror(errno));
        return Failure;
    }
    memset(bTreeNode->child, 0,(size)* sizeof(BTreeNodeChild));

    bTreeNode->record = (Record*)malloc(recordSize*sizeof(Record));

    bTreeNode->parent= nullptr;
    return Success;
}

int createNode(BTree &bTree,BTreeNode &bTreeNode){
    return createNodeByOrder(bTree,bTreeNode,1);
}

void destroyNode(BTreeNode &bTreeNode){

    if(bTreeNode== nullptr)
        return;

    for (int i = 0; i <= bTreeNode->num; ++i) {
        bTreeNode->child[i] = nullptr;
    }
    free(bTreeNode->child);
    bTreeNode->child= nullptr;

    bTreeNode->parent = nullptr;

    free(bTreeNode->key);
    bTreeNode->key = nullptr;

    free(bTreeNode->record);
    bTreeNode->record = nullptr;

    free(bTreeNode);
    bTreeNode = nullptr;

}

int createResult(Result &result){

    result = (Result)calloc(1, sizeof(ResultNode));

    if(result== nullptr){
        fprintf(stderr, "[%s][%d] errmsg:[%d] %s\n", __FILE__, __LINE__, errno, strerror(errno));
        return Failure;
    }

    result->node = nullptr;
    result->index = MIN_INT;
    result->tag = Failure;

    return Success;
}

void destroyResult(Result &result){

    free(result);
    result = nullptr;
}

void beforePrintBTree(BTreeNode node,int depth,int num){

    if(node == nullptr)
        return;
    for (int k = 0; k < depth; ++k) {
        printf("%s",k==0?"":"      ");
    }
    printf("第%d层---第%d个节点:",depth,num);
    for (int i = 0; i < node->num; ++i) {
        printf("%c%d%c",i==0?'[':'\0',node->key[i],i==node->num-1?']':',');
    }
    printf("\n");
    for (int j = 0; j <=node->num ; ++j) {
        beforePrintBTree(node->child[j],depth+1,j);
    }
}