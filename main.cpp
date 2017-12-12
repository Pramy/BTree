#include <iostream>
#include <cstring>
#include <cmath>
#include <stack>
#include <ctime>
#include <queue>
#include "algorithm"

//#define m 3
#define Success 1
#define Failure 0
#define KeyType int
#define Record int
#define MIN_INT 0x80000001
#define MAX_INT 0x7FFFFFFF
using namespace std;
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

typedef struct Result{

    int tag = Failure;
    int index = MIN_INT;
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
    tree->splitIndex = m/2;
    tree->root = nullptr;//空树
    return Success;
}

int createNode(BTree &bTree,BTreeNode &bTreeNode){

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
//    for (int i = 0; i < size; ++i) {
//        bTreeNode->child[i]= nullptr;
//    }
    bTreeNode->parent= nullptr;
    return Success;
}

Result binaryCompare(BTreeNode &root, KeyType k){

    Result result;
    int left = 0,right = root->num-1;
    while (right>=left){
        int mid = (left+right)/2;

        if(k == root->key[mid]){
            printf("key:%d already exits !\n",root->key[mid]);
            result.tag = Success;
            result.index = mid;
            return result;
        }
        if(root->key[mid]<k)
            left = mid+1;
        else
            right = mid-1;
    }
    result.index=left;
    return result;
}

Result SearchNode(BTreeNode &root,KeyType k){

    Result result;
    while (root!= nullptr){
        result = binaryCompare(root,k);
        if(result.tag==Success || result.index==MIN_INT || root->child[result.index]== nullptr)
            break;
        root = root->child[result.index];
    }
    return result;
}

int splitNode(BTree &bTree,BTreeNode &node){

    int splitIndex = bTree->splitIndex;
    int index = 0 , total = 0;
    BTreeNode parent = nullptr;
    BTreeNode increaseNode = nullptr;

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
            index = binaryCompare(parent,node->key[splitIndex]);
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

    SearchNode(node,key,index);
    if(index==-1)
        return Failure;
    return BTreeInsertByOrder(bTree,node,key,index);
}

void printBTree(BTreeNode node,int depth){

    if(node == nullptr)
        return;
    for (int j = 0; j <=node->num ; ++j) {
        printBTree(node->child[j],depth+1);
    }
    for (int k = 0; k < depth; ++k) {
        printf("%s"," ");
    }
    printf("第%d层:|--",depth);
    for (int i = 0; i < node->num; ++i) {
        printf("%c%d%c",i==0?'[':'\0',node->key[i],i==node->num-1?']':',');
    }
    printf("\n");
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
    int a[1000];
    int sum = 20;
    for(int i= 0;i<sum;i++){
        a[i]=rand()%1000;
    }
    for (int i = 0; i < sum; ++i) {
//        int a = rand()%100;
//        printf("%d%c",a,(i==10)?'\n':',');
        cout<<"insert:"<<a[i]<<" ";
        BTreeInsert(bTree,a[i]);
    }
    cout<<endl;
    printBTree(bTree->root,1);
    printf("MIN_INT:%d,MAX_INT:%d,result:%d\n",MIN_INT,MAX_INT,MIN_INT+MAX_INT);
//    BTreeNode bTreeNode;
//    int flag1=createNode(bTree,bTreeNode);
//    printf("flag1=%d , number=%d\n",flag1,bTreeNode->num);

//    memset(bTreeNode->key,0,(m+1)* sizeof(KeyType));
//    int r = 1;
//    for (int j = 0; j < m-1; ++j,r+=2) {
//        int a = rand()%100;
//        int index = binaryCompare(bTreeNode,a);
//        if(index!=-1) {
//            memcpy(bTreeNode->key+index+1,bTreeNode->key+index, (bTreeNode->num-index)*sizeof(KeyType));
//            bTreeNode->key[index] = a;
//            bTreeNode->num++;
//        }
//        printf("%d%c",r,j==m-2?'\n':' ');
//    }
//    bTreeNode->key[0] = 1;
//    bTreeNode->key[1] = 3;
//    bTreeNode->key[2] = 5;
//    bTreeNode->key[3] = 7;
//    bTreeNode->key[4] = 9;
//    bTreeNode->key[5] = 11;
//    bTreeNode->key[6] = 13;
//    bTreeNode->key[7] = 15;
//    bTreeNode->key[8] = 17;
//
//    bTreeNode->num = m-1;
//    int needToFind = 4;
//    int res = binaryCompare(bTreeNode, needToFind);
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
