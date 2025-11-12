#ifndef TREE_TREESCTRUCT_H
#define TREE_TREESCTRUCT_H
#include "common.h"

typedef char* treeElType_t;

typedef struct treeNode {
    treeElType_t data;
    treeNode* left;
    treeNode* right;
    bool shouldFree;
} treeNode_t;

int createNode(treeElType_t data, bool shouldFree, treeNode_t** result);

int treeAdd(treeNode_t* root, treeElType_t val);
treeNode_t* getRight(treeNode_t* node);
void setRight(treeNode_t* node, treeNode_t* right);

treeNode_t* getLeft(treeNode_t* node);
void setLeft(treeNode_t* node, treeNode_t* left);

treeElType_t getData(treeNode_t* node);
void setDataCopy(treeNode_t* node, treeElType_t data);
void setData(treeNode_t* node, treeElType_t data);

int validateTree(treeNode_t* root);
int printTree(treeNode_t *root);
void destroyTree(treeNode_t* root);


#endif //TREE_TREESCTRUCT_H