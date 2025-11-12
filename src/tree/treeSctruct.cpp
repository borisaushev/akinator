#include "treeSctruct.h"

#include <cassert>
#include <cstdio>

#include "common.h"
#include "treeDump.h"

treeNode_t* getRight(treeNode_t* node) {
    assert(node);
    return node->right;
}

void setRight(treeNode_t* node, treeNode_t* right) {
    assert(node);
    node->right = right;
}

treeNode_t* getLeft(treeNode_t* node) {
    assert(node);
    return node->left;
}

void setLeft(treeNode_t* node, treeNode_t* left) {
    assert(node);
    node->left = left;
}

treeElType_t getData(treeNode_t* node) {
    assert(node);
    return node->data;
}

void setDataCopy(treeNode_t* node, treeElType_t data) {
    assert(node);
    assert(data);

    node->data = (char*) calloc(strlen(data), sizeof(char));
    strcpy(node->data, data);
}

void setData(treeNode_t* node, treeElType_t data) {
    assert(node);

    node->data = data;
}

int printTree(treeNode_t *root) {
    TREE_VALID(root);

    printf("(");
    if (getLeft(root) != NULL) {
        printTree(getLeft(root));
    }
    printf("'%s'", getData(root));
    if (getRight(root) != NULL) {
        printTree(getRight(root));
    }
    printf(")");

    return AK_SUCCESS;
}

int createNode(treeElType_t data, bool shouldFree, treeNode_t** result) {
    assert(result);
    *result = (treeNode_t*)calloc(1, sizeof(treeNode_t));
    (*result)->shouldFree = shouldFree;

    if (*result == NULL) {
        RETURN_ERR(AK_NULL_PTR, "unable to allocate memory for node");
    }

    (*result)->left = NULL;
    (*result)->right = NULL;

    (*result)->data = data;

    return AK_SUCCESS;
}

static void countTreeSize(treeNode_t* node, size_t* size) {
    if ((*size)++ > MAX_TREE_DEPTH) {
        return;
    }

    if (getLeft(node) != NULL) {
        countTreeSize(getLeft(node), size);
    }
    if (getRight(node) != NULL) {
        countTreeSize(getRight(node), size);
    }
}

int validateTree(treeNode_t* root) {
    if (root == NULL) {
        RETURN_ERR(AK_NULL_PTR, "root is null");
    }

    size_t size = 0;
    countTreeSize(root, &size);
    if (size > MAX_TREE_DEPTH) {
        RETURN_ERR(AK_INVALID_CAPACITY, "tree is cycled");
    }

    return AK_SUCCESS;
}

static int insertValue(treeNode_t** cur, treeElType_t val) {
    if (*cur == NULL) {
        SAFE_CALL(createNode(val, false, cur));

        return AK_SUCCESS;
    }

    if (val == getData(*cur)) {
        return AK_SUCCESS;
    }
    else if (val < getData(*cur)) {
        return insertValue(&(*cur)->left, val);
    }
    else {
        return insertValue(&(*cur)->right, val);
    }
}

int treeAdd(treeNode_t* root, treeElType_t val) {
    TREE_VALID(root);
    TREE_DUMP(root, "BEFORE ADD", AK_SUCCESS);

    insertValue(&root, val);

    TREE_VALID(root);
    TREE_DUMP(root, "AFTER ADD", AK_SUCCESS);

    return AK_SUCCESS;
}

static void destroyNode(treeNode_t* node) {
    if (node != NULL) {
        destroyNode(getLeft(node));
        destroyNode(getRight(node));
        if (node->shouldFree) {
            free(node->data);
        }
        free(node);
    }
}

int destroyLeftNode(treeNode* parentNode) {
    destroyNode(getLeft(parentNode));
    setLeft(parentNode, NULL);

    return AK_SUCCESS;
}

int destroyRightNode(treeNode* parentNode) {
    destroyNode(getRight(parentNode));
    setRight(parentNode, NULL);

    return AK_SUCCESS;
}

void destroyTree(treeNode_t* root) {
    destroyNode(root);
}

