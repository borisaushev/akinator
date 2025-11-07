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
    char endChar = 0;
    scanf("%[^\n]%c", inp, &endChar);
}

error_t initTree(treeNode_t** root) {
    treeNode* physteh = {};
    SAFE_CALL(createNode("MIPT student", &physteh));
    *root = physteh;

    treeNode_t* cryptoScam = {};
    SAFE_CALL(createNode("crypto scammer", &cryptoScam));

    treeNode_t* baggage = {};
    SAFE_CALL(createNode("got a ride in a car trunk", &baggage));

    treeNode_t* notAGoodTeacher = {};
    SAFE_CALL(createNode("govorit idi gugli", &notAGoodTeacher));

    setRight(physteh, cryptoScam);
    setLeft(physteh, notAGoodTeacher);
    setRight(cryptoScam, baggage);


    treeNode_t* tate = {};
    SAFE_CALL(createNode("Andrew Tate", &tate));

    treeNode_t* arsen = {};
    SAFE_CALL(createNode("Arsen Markaryan", &arsen));

    treeNode_t* samSulek = {};
    SAFE_CALL(createNode("Sam Sulek", &samSulek));

    treeNode_t* sanya = {};
    SAFE_CALL(createNode("Aleksandr Dashchinski", &sanya));

    treeNode_t* BossBoriss = {};
    SAFE_CALL(createNode("BossBoriss", &BossBoriss));


    setLeft(cryptoScam, tate);
    setLeft(baggage, arsen);
    setRight(baggage, samSulek);
    setLeft(notAGoodTeacher, sanya);
    setRight(notAGoodTeacher, BossBoriss);

    TREE_DUMP(physteh, "akinator main dump", SUCCESS);

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
        fprintf(file, "nil");
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
