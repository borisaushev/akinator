#include "treeSctruct.h"

#include <cassert>
#include <cstdio>

#include "common.h"
#include "dump/treeDump.h"

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

treeElType getData(treeNode_t* node) {
    assert(node);
    return node->data;
}

void setData(treeNode_t* node, treeElType data) {
    assert(node);
    node->data = data;
}

error_t printTree(treeNode_t *root) {
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

    return SUCCESS;
}

error_t createNode(treeElType data, treeNode_t** result) {
    assert(result);
    *result = (treeNode_t*)calloc(1, sizeof(treeNode_t));

    if (*result == NULL) {
        RETURN_ERR(NULL_PTR, "unable to allocate memory for node");
    }

    (*result)->left = NULL;
    (*result)->right = NULL;

    (*result)->data = (char*) calloc(strlen(data), sizeof(char));
    strcpy((*result)->data, data);

    return SUCCESS;
}

static void countTreeSize(treeNode_t* node, size_t* size) {
    if ((*size)++ > MAX_REASONABLE_SIZE) {
        return;
    }

    if (getLeft(node) != NULL) {
        countTreeSize(getLeft(node), size);
    }
    if (getRight(node) != NULL) {
        countTreeSize(getRight(node), size);
    }
}

error_t validateTree(treeNode_t* root) {
    if (root == NULL) {
        RETURN_ERR(NULL_PTR, "root is null");
    }

    size_t size = 0;
    countTreeSize(root, &size);
    if (size > MAX_REASONABLE_SIZE) {
        RETURN_ERR(INVALID_CAPACITY, "tree is cycled");
    }

    return SUCCESS;
}

static error_t insertValue(treeNode_t** cur, treeElType val) {
    if (*cur == NULL) {
        SAFE_CALL(createNode(val, cur));

        return SUCCESS;
    }

    if (val == getData(*cur)) {
        return SUCCESS;
    }
    else if (val < getData(*cur)) {
        return insertValue(&(*cur)->left, val);
    }
    else {
        return insertValue(&(*cur)->right, val);
    }
}

error_t treeAdd(treeNode_t* root, treeElType val) {
    TREE_VALID(root);
    TREE_DUMP(root, "BEFORE ADD", SUCCESS);

    insertValue(&root, val);

    TREE_VALID(root);
    TREE_DUMP(root, "AFTER ADD", SUCCESS);

    return SUCCESS;
}

static void destroyNode(treeNode_t* node) {
    if (node != NULL) {
        destroyNode(getLeft(node));
        destroyNode(getRight(node));
        free(getData(node));
        free(node);
    }
}

error_t destroyLeftNode(treeNode* parentNode) {
    destroyNode(getLeft(parentNode));
    setLeft(parentNode, NULL);

    return SUCCESS;
}

error_t destroyRightNode(treeNode* parentNode) {
    destroyNode(getRight(parentNode));
    setRight(parentNode, NULL);

    return SUCCESS;
}

void destroyTree(treeNode_t* root) {
    destroyNode(root);
}

