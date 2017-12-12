//
// Created by Pramy on 2017/12/12.
//
//#pragma once
#ifndef DEMO_STRUCTURE_H
#define DEMO_STRUCTURE_H


#include "constant.h"

/**
 * B���ڵ�
 */

typedef struct BNode{

    int num;                    //�ؼ�������
    KeyType *key;               //�ؼ��֣���ռ�ռ�Ϊ(max+1) - �������1���ռ����ڽ����ռ�ʹ��
    BNode *parent;              //���ڵ�
    BNode **child;              //�ӽ�㣺��ռ�ռ�Ϊ��max+2��- �������1���ռ����ڽ����ռ�ʹ��
    Record *record;

}*BTreeNode,**BTreeNodeChild;
/**
 * B��
 */
typedef struct Tree{

    int max;                     // ����������ؼ��ָ��� - ��max = m - 1
    int min;                     // ���������С�ؼ��ָ��� - ��min = [m/2] - 1
    int splitIndex;              // �������� splitIndex = m/2
    BTreeNode root;              // ���ڵ�
} *BTree;
/**
 * �����
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