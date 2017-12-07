#include <iostream>
#include <cstring>
#include <cmath>
#include "algorithm"

//#define m 3
#define Success 1
#define Failure 0;
#define KeyType int
#define Record int

using namespace std;
/**
 * B树节点
 */
typedef struct BNode{

    int keyNumber;              //关键字数量
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
    int sidx;                    // 分裂索引 sidx = (max+1)/2
    BTreeNode root;              // 根节点
} *BTree;

typedef struct Result{

    BTreeNode resultPoint;
    int index;
    int tag;
};
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
    tree->root = nullptr;//空树
    return Success;
}

int createNode(BTree &bTree,BTreeNode &bTreeNode){

    bTreeNode = (BTreeNode)malloc(sizeof(BNode));
    if(bTreeNode == nullptr){
        fprintf(stderr, "[%s][%d] errmsg:[%d] %s\n", __FILE__, __LINE__, errno, strerror(errno));
        return Failure;
    }
    bTreeNode->keyNumber=0;

    bTreeNode->key = (KeyType *)malloc((bTree->max+2)*sizeof(KeyType));

    if(bTreeNode->key == nullptr){
        free(bTreeNode);
        bTreeNode= nullptr;
        fprintf(stderr, "[%s][%d] errmsg:[%d] %s\n", __FILE__, __LINE__, errno, strerror(errno));
        return Failure;
    }

    bTreeNode->child = (BTreeNodeChild)malloc((bTree->max+2)*sizeof(BTreeNodeChild));

    if(bTreeNode->child == nullptr){
        free(bTreeNode->key);
        bTreeNode->key= nullptr;
        free(bTreeNode);
        bTreeNode = nullptr;
        fprintf(stderr, "[%s][%d] errmsg:[%d] %s\n", __FILE__, __LINE__, errno, strerror(errno));
        return Failure;
    }
    return Success;
}

int Compare(BTreeNode &root,KeyType k){
    int i = 1;
    while ( i <=root->keyNumber && k > root->key[i]){
        i++;
    }
    return i;
}

Result SearchNode(BTreeNode &root,KeyType k){

    int index = 0,found = 0;
    BTreeNode nowNode = root, parent = nullptr;

    while (nowNode!= nullptr && found == 0){

    }
}

int main() {

    BTree  bTree;
    int flag=createBTree(bTree,3);
    Result result = SearchNode(bTree->root,1);
    cout<<flag<<endl;
    printf("max = %d,min = %d,sidx = %d \n",bTree->max,bTree->min,bTree->sidx);
    BTreeNode bTreeNode;
    int flag1=createNode(bTree,bTreeNode);
    printf("flag1=%d , number=%d\n",flag1,bTreeNode->keyNumber);

    bTreeNode->key[1] = 1;
    bTreeNode->key[2] = 3;

    bTreeNode->keyNumber = 2;
    int res = Compare(bTreeNode,6);
    printf("index = %d",res);

}