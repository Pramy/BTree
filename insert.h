//
// Created by Pramy on 2017/12/12.
//

#ifndef DEMO_INSERT_H
#define DEMO_INSERT_H

#endif //DEMO_INSERT_H

#include "structure.h"

void binarySearch(BTreeNode &root, KeyType k, Result &result);

void SearchNode(BTreeNode &root,KeyType k,Result &result);

int splitNode(BTree &bTree, BTreeNode &node);

int BTreeInsertByOrder(BTree &bTree,BTreeNode &node,KeyType key, int index);

int BTreeInsert(BTree &bTree,KeyType key);

