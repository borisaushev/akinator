#include "akinator.h"

#include "treeDump.h"
#include "treeSctruct.h"

static void getNextNode(treeNode_t** cur, char inp[50]) {
    if (strcmp(inp, "yes") == 0) {
        *cur = getLeft(*cur);
    }
    else if (strcmp(inp, "no") == 0) {
        *cur = getRight(*cur);
    }
    else {
        printf("normalno napishy\n");
    }
}

static void readUserAnswer(char inp[50]) {
    char lastChar = 0;
    scanf("%[^\n]%c", inp, &lastChar);
}

error_t readNode(treeNode_t** cur, FILE* file) {
    char firstCh = 0;
    if (fscanf(file, "%c", &firstCh) != 1) {
        RETURN_ERR(INVALID_INPUT, "UNABLE TO PARSE akinator file");
    }

    if (firstCh == '(') {
        char inp[50] = {};
        fscanf(file, "\"%[^\"]\"", inp);
        SAFE_CALL(createNode(inp, cur));
        readNode(&(*cur)->left, file);
        readNode(&(*cur)->right, file);
        fscanf(file, "%c", &firstCh);
    }
    else if (firstCh == '&') {
        *cur = NULL;
    }
    else {
        RETURN_ERR(INVALID_INPUT, "invalid symbol in akinator file");
    }

    return SUCCESS;
}

error_t initTree(treeNode_t** root) {
    FILE* file = fopen(AKINATOR_FILE_PATH, "r");
    if (file == NULL) {
        RETURN_ERR(NULL_PTR, "unable to open file");
    }
    readNode(root, file);
    fclose(file);

    TREE_DUMP(*root, "akinator main dump", SUCCESS);

    return SUCCESS;
}

error_t addNewCharacter(treeNode_t *root, treeNode_t *cur, char inp[50]) {
    TREE_DUMP(root, "BEFORE ADD NEW CHARACTER", SUCCESS);

    printf("Who was your character?\n");

    readUserAnswer(inp);
    treeNode_t* newCharacterNode = {};
    SAFE_CALL(createNode(inp, &newCharacterNode));

    treeNode_t* prevCharacterNode = {};
    SAFE_CALL(createNode(getData(cur), &prevCharacterNode));

    printf("Whats the difference between %s and %s?\n", getData(newCharacterNode), getData(cur));
    printf("%s...", getData(newCharacterNode));
    readUserAnswer(inp);
    printf("\n");

    setData(cur, inp);
    setLeft(cur, newCharacterNode);
    setRight(cur, prevCharacterNode);

    TREE_VALID(root);
    TREE_DUMP(root, "AFTER ADD NEW CHARACTER", SUCCESS);

    return SUCCESS;
}

static void writeNodeRec(treeNode_t* node, FILE* file) {
    if (node == NULL) {
        fprintf(file, "&");
    }
    else {
        fprintf(file, "(\"%s\"", getData(node));
        writeNodeRec(getLeft(node), file);
        writeNodeRec(getRight(node), file);
        fprintf(file, ")");
    }
}

error_t saveAkinatorData(treeNode_t* root) {
    FILE* file = fopen(AKINATOR_FILE_PATH, "w");
    if (file == NULL) {
        RETURN_ERR(NULL_PTR, "unable to open file");
    }

    writeNodeRec(root, file);

    fclose(file);

    return SUCCESS;
}

error_t runAkinator() {
    treeNode_t* root = {};
    SAFE_CALL(initTree(&root));

    treeNode_t* cur = root;
    char inp[50] = {};
    while (getRight(cur) != NULL) {
        printf("your character %s?\n", getData(cur));
        readUserAnswer(inp);

        getNextNode(&cur, inp);
    }

    printf("is your character %s?\n", getData(cur));
    readUserAnswer(inp);
    if (strcmp(inp, "yes") == 0) {
        printf("Ugu\n");
    }
    else if (strcmp(inp, "no") == 0) {
        SAFE_CALL(addNewCharacter(root, cur, inp));
    }
    else {
        printf("nu ty i daun\n");
    }

    SAFE_CALL(saveAkinatorData(root));
    destroyTree(root);

    return SUCCESS;
}
