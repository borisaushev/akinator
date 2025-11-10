#include "akinator.h"

#include "treeDump.h"
#include "treeSctruct.h"

static void getNextNode(treeNode_t** cur, char inp[MAX_INPUT_SIZE]) {
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

static void readUserAnswer(char inp[MAX_INPUT_SIZE]) {
    scanf("%[^\n]", inp);
    getchar();
}

static int readNode(treeNode_t** cur, FILE* file) {
    char firstCh = 0;
    if (fscanf(file, "%c", &firstCh) != 1) {
        RETURN_ERR(AK_INVALID_INPUT, "UNABLE TO PARSE akinator file");
    }

    if (firstCh == '(') {
        char inp[MAX_INPUT_SIZE] = {};
        fscanf(file, "\"%[^\"]\"", inp); // scanning text from format: "desired content"
        SAFE_CALL(createNode(inp, cur));
        readNode(&(*cur)->left, file);
        readNode(&(*cur)->right, file);
        fscanf(file, "%c", &firstCh);
    }
    else if (firstCh == '&') {
        *cur = NULL;
    }
    else {
        RETURN_ERR(AK_INVALID_INPUT, "invalid symbol in akinator file");
    }

    return AK_SUCCESS;
}

static int initTree(treeNode_t** root) {
    FILE* file = fopen(AKINATOR_FILE_PATH, "r");
    if (file == NULL) {
        RETURN_ERR(AK_NULL_PTR, "unable to open file");
    }
    readNode(root, file);
    fclose(file);

    TREE_DUMP(*root, "parsed tree dump", AK_SUCCESS);

    return AK_SUCCESS;
}

static int addNewCharacter(treeNode_t *root, treeNode_t *cur, char inp[MAX_INPUT_SIZE]) {
    TREE_DUMP(root, "BEFORE ADD NEW CHARACTER", AK_SUCCESS);

    printf("Who was your character?\n");

    readUserAnswer(inp);
    treeNode_t* newCharacterNode = {};
    SAFE_CALL(createNode(inp, &newCharacterNode));

    treeNode_t* prevCharacterNode = {};
    SAFE_CALL(createNode(getData(cur), &prevCharacterNode));

    printf("Whats the difference between %s and %s?\n",
           getData(newCharacterNode), getData(cur));
    printf("%s...", getData(newCharacterNode));
    readUserAnswer(inp);
    printf("\n");

    setData(cur, inp);
    setLeft(cur, newCharacterNode);
    setRight(cur, prevCharacterNode);

    TREE_VALID(root);
    TREE_DUMP(root, "AFTER ADD NEW CHARACTER", AK_SUCCESS);

    return AK_SUCCESS;
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

static int saveAkinatorData(treeNode_t* root) {
    FILE* file = fopen(AKINATOR_FILE_PATH, "w");
    if (file == NULL) {
        RETURN_ERR(AK_NULL_PTR, "unable to open file");
    }

    writeNodeRec(root, file);

    fclose(file);

    return AK_SUCCESS;
}

static int guessCharacter(treeNode_t *root) {
    treeNode_t* cur = root;
    char inp[MAX_INPUT_SIZE] = {};
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
    return AK_SUCCESS;
}

static int describeRec(treeNode_t* node, const char* character) {
    if (node == NULL) {
        return 0;
    }

    if (strcmp(character, getData(node)) == 0) {
        printf("%s is:\n", character);
        return 1;
    }

    if (describeRec(getLeft(node), character) == 1) {
        printf("    %s\n", getData(node));
        return 1;
    }
    if (describeRec(getRight(node), character) == 1) {
        printf("    NOT %s\n", getData(node));
        return 1;
    }

    return 0;
}

void describeCharacter(treeNode_t *root, char inp[100]) {
    printf("Which character do you want me to describe?\n");
    readUserAnswer(inp);
    if (describeRec(root, inp) == 0) {
        printf(" - character was not found =(\n");
    }
}

int runAkinator() {
    treeNode_t* root = {};
    BB_TRY {
        BB_CHECKED(initTree(&root));
        printf("options: 'guess' - play akinator guess game, "
                              "'describe' - give a definition of a character\n");
        char inp[MAX_INPUT_SIZE] = {};
        for (readUserAnswer(inp); strcmp(inp, "exit") != 0 ; readUserAnswer(inp)) {
            if (strcmp(inp, "guess") == 0) {
                BB_CHECKED(guessCharacter(root));
            }
            else if (strcmp(inp, "describe") == 0) {
                describeCharacter(root, inp);
            }
            else {
                printf("invalid command\n");
            }
            printf("\nwhat are they gonna do now?\n");
        }
    } BB_ENDTRY
    BB_CATCH {
    } BB_ENDCATCH
    BB_FINALLY {
        destroyTree(root);
    }

    printf("cause its boiled\n");

    return AK_SUCCESS;
}
