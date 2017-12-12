#include <iostream>
#include <ctime>
#include "insert.h"
#include "delete.h"
using namespace std;


void printBTree(BTreeNode node,int depth,int num){

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
        printBTree(node->child[j],depth+1,j);
    }
}

int main() {

    BTree  bTree;
    int m = 3;
    int flag=createBTree(bTree,m);

//    int searchIndex;
//    SearchNode(bTree->root,1);
//    cout<<flag<<endl;
//    printf("max = %d,min = %d,sidx = %d \n",bTree->max,bTree->min,bTree->splitIndex);
    srand((unsigned)time(nullptr));
//    int a[]={39,98,26,23,25,41,26,24,97,20};
//    int a[1000]={44,23,52,89,2,49,60,36,11,99,69};
    int  a[] = {36,21,39,33,43,14,37,15,28,34,27,35,42,48,38,40,49};
    int sum = 17;
//    for(int i= 0;i<sum;i++){
//        a[i]=rand()%100;
//    }
    for (int i = 0; i < sum; ++i) {
//        int a = rand()%100;
//        printf("%d%c",a,(i==10)?'\n':',');
//        cout<<"insert:"<<a[i]<<" ";
        BTreeInsert(bTree,a[i]);
    }
    cout<<endl;
    printBTree(bTree->root,1,0);
    printf("MIN_INT:%d,MAX_INT:%d,result:%d\n",MIN_INT,MAX_INT,MIN_INT+MAX_INT);

    Result result{};
    createResult(result);
    SearchNode(bTree->root,a[10],result);
    if(result->tag == Success){
        printf("找到%d,下标为：%d\n",a[10],result->index);
        for (int i = 0; i < result->node->num; ++i) {
            printf("%d,",result->node->key[i]);
        }
    }
    test(bTree->root);
//    BTreeNode bTreeNode;
//    int flag1=createNode(bTree,bTreeNode);
//    printf("flag1=%d , number=%d\n",flag1,bTreeNode->num);

//    memset(bTreeNode->key,0,(m+1)* sizeof(KeyType));
//    int r = 1;
//    for (int j = 0; j < m-1; ++j,r+=2) {
//        int a = rand()%100;
//        int index = binarySearch(bTreeNode,a);
//        if(index!=-1) {
//            memcpy(bTreeNode->key+index+1,bTreeNode->key+index, (bTreeNode->num-index)*sizeof(KeyType));
//            bTreeNode->key[index] = a;
//            bTreeNode->num++;
//        }
//        printf("%d%c",r,j==m-2?'\n':' ');
//    }
//
//    bTreeNode->num = m-1;
//    int needToFind = 4;
//    int res = binarySearch(bTreeNode, needToFind);
//    printf("%d insert index = %d\n",needToFind,res);
//
//    for (int i = 0; i < bTree->max + 2; ++i) {
//        printf("%d%c", bTreeNode->child[i],i==bTree->max+1?'\n':' ');
//    }
//    cout<<endl;
//    memcpy(bTreeNode->key+res+1,bTreeNode->key+res, (bTreeNode->num-res)*sizeof(KeyType));
//    memcpy(bTreeNode->child+res+1,bTreeNode->child+res, (bTreeNode->num-res+1)*sizeof(BTreeNodeChild));
//    for (int i = 0; i < bTree->max + 2; ++i) {
//        printf("%d%c", bTreeNode->child[i],i==bTree->max+1?'\n':' ');
//    }
//    bTreeNode->key[res]=needToFind;
//    for (int k = 0; k < m; ++k) {
//        printf("%d%c",bTreeNode->key[k],k==m-1?'\n':' ');
//    }
}
