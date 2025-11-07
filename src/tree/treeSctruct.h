#ifndef TREE_TREESCTRUCT_H
#define TREE_TREESCTRUCT_H
#include "common.h"

typedef char* treeElType;

typedef struct treeNode {
    treeElType data;
    treeNode* left;
    treeNode* right;
} treeNode_t;

error_t createNode(treeElType data, treeNode_t** result);

error_t treeAdd(treeNode_t* root, treeElType val);
treeNode_t* getRight(treeNode_t* node);
void setRight(treeNode_t* node, treeNode_t* right);

treeNode_t* getLeft(treeNode_t* node);
void setLeft(treeNode_t* node, treeNode_t* left);

treeElType getData(treeNode_t* node);
void setData(treeNode_t* node, treeElType data);

error_t validateTree(treeNode_t* root);
error_t printTree(treeNode_t *root);
void destroyTree(treeNode_t* root);


#endif //TREE_TREESCTRUCT_H