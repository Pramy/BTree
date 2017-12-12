//
// Created by Pramy on 2017/12/12.
//
//#pragma once
#ifndef DEMO_STRUCTURE_H
#define DEMO_STRUCTURE_H


#include "constant.h"

/**
 * B树节点
 */

typedef struct BNode{

    int num;                    //关键字数量
    KeyType *key;               //关键字：所占空间为(max+1) - 多出来的1个空间用于交换空间使用
    BNode *parent;              //父节点
    BNode **child;              //子结点：所占空间为（max+2）- 多出来的1个空间用于交换空间使用
    Record *record;

}*BTreeNode,**BTreeNodeChild;
/**
 * B树
 */
typedef struct Tree{

    int max;                     // 单个结点最大关键字个数 - 阶max = m - 1
    int min;                     // 单个结点最小关键字个数 - 阶min = [m/2] - 1
    int splitIndex;              // 分裂索引 splitIndex = m/2
    BTreeNode root;              // 根节点
} *BTree;
/**
 * 结果集
 */
typedef struct ResultNode{

    int tag ;
    int index ;
    BTreeNode node ;
}*Result;

int createBTree(BTree &tree, int m);

void destroyTree(BTree &bTree);

int createNode(BTree &bTree,BTreeNode &bTreeNode);

int createNodeByOrder(BTree &bTree,BTreeNode &bTreeNode,int recordSize);

void destroyNode(BTreeNode &bTreeNode);

int createResult(Result &result);

void destroyResult(Result &result);

void beforePrintBTree(BTreeNode node,int depth,int num);

#endif //DEMO_STRUCTURE_H