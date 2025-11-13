#include <string.h>
#include "akinator.h"

#include "treeDump.h"
#include "treeSctruct.h"

#include <stdio.h>

#include "stack.h"
#include "akinatorInput.h"

typedef struct akinatorInfo {
    treeNode* root;
    char* buffer;
} akinatorInfo_t;

//TODO сделать озвучку и приставки

static void getNextNode(treeNode_t** cur, char inp[MAX_LINE_LENGTH]) {
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

static int initTree(akinatorInfo_t* akinatorInfo) {
    FILE* file = fopen(AKINATOR_FILE_PATH, "r");
    if (file == NULL) {
        RETURN_ERR(AK_NULL_PTR, "unable to open file");
    }
    int bytesRead = -1;
    SAFE_CALL(read_file(AKINATOR_FILE_PATH, &akinatorInfo->buffer, &bytesRead));

    char* cur = akinatorInfo->buffer;
    SAFE_CALL(parseNode(&cur, &akinatorInfo->root));

    fclose(file);

    TREE_DUMP(akinatorInfo->root, "parsed tree dump", AK_SUCCESS);

    return AK_SUCCESS;
}


static int addNewCharacter(treeNode_t* root, treeNode_t* cur, char inp[MAX_LINE_LENGTH]) {
    TREE_DUMP(root, "BEFORE ADD NEW CHARACTER", AK_SUCCESS);

    printf("Who was your character?\n");

    readUserAnswer(inp);
    treeNode_t* newCharacterNode = {};
    SAFE_CALL(createNode(inp, true, &newCharacterNode));
    setDataCopy(newCharacterNode, inp);

    treeNode_t* prevCharacterNode = {};
    SAFE_CALL(createNode(getData(cur), true, &prevCharacterNode));
    setDataCopy(prevCharacterNode, getData(cur));

    printf("Whats the difference between %s and %s?\n",
           getData(newCharacterNode), getData(cur));
    printf("%s...", getData(newCharacterNode));
    readUserAnswer(inp);
    printf("\n");

    setDataCopy(cur, inp);
    setLeft(cur, newCharacterNode);
    setRight(cur, prevCharacterNode);

    TREE_VALID(root);
    TREE_DUMP(root, "AFTER ADD NEW CHARACTER", AK_SUCCESS);

    return AK_SUCCESS;
}

static int guessCharacter(treeNode_t* root) {
    treeNode_t* cur = root;
    char inp[MAX_LINE_LENGTH] = {};
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

/**
 *
 * @param node current node
 * @param character the character string
 * @return:
 * - true if the node is related to the character (is itself a character, or one of the children is related to the character)
 * - false otherwise
 */
static bool describeRec(treeNode_t* node, const char* character, stack_t* propertyStack) {
    if (node == NULL) {
        return false;
    }

    if (strcmp(character, getData(node)) == 0) {
        return true;
    }

    if (describeRec(getLeft(node), character, propertyStack) == 1) {
        stackPush(propertyStack, {getData(node), true});
        return true;
    }
    if (describeRec(getRight(node), character, propertyStack) == 1) {
        stackPush(propertyStack, {getData(node), false});
        return true;
    }

    return false;
}

void describeCharacter(treeNode_t* root) {
    printf("Which character do you want me to describe?\n");
    char character[MAX_LINE_LENGTH];
    readUserAnswer(character);

    stack_t propertyStack = {};
    initStack(&propertyStack, STACK_BASE_SIZE);

    if (!describeRec(root, character, &propertyStack)) {
        printf(" - character was not found =(\n");
        return;
    }

    characterProperty property = {};
    printf("your character is:\n");
    while (getStackElementCount(&propertyStack) > 0) {
        stackPop(&propertyStack, &property);
        printf("\t");
        if (!property.answer) {
            printf("NOT ");
        }
        printf("%s\n", property.question);
    }
}

void compareCharacters(treeNode_t* root) {
    printf("Which character do you want me compare?\n");
    char character1[MAX_LINE_LENGTH];
    readUserAnswer(character1);

    stack_t propertyStack1 = {};
    initStack(&propertyStack1, STACK_BASE_SIZE);
    if (!describeRec(root, character1, &propertyStack1)) {
        printf(" - character was not found =(\n");
        return;
    }

    printf("Which character do you want me to campare the first characted to?\n");
    char character2[MAX_LINE_LENGTH];
    readUserAnswer(character2);

    stack_t propertyStack2 = {};
    initStack(&propertyStack2, STACK_BASE_SIZE);
    if (!describeRec(root, character2, &propertyStack2)) {
        printf(" - character was not found =(\n");
        return;
    }

    printf("%s and %s are both:\n", character1, character2);
    characterProperty property1 = {};
    characterProperty property2 = {};
    while (getStackElementCount(&propertyStack1) > 0 && getStackElementCount(&propertyStack2) > 0) {
        stackPop(&propertyStack1, &property1);
        stackPop(&propertyStack2, &property2);
        if (property1.answer) {}
    }

    //TODO
}

int runAkinator() {
    akinatorInfo_t akinatorInfo = {};
    BB_TRY {
        BB_CHECKED(initTree(&akinatorInfo));
        printf("options: \n"
                              "\t'guess' - play akinator guess game\n"
                              "\t'describe' - give a definition of a character\n"
                              "\t'exit' - end program\n");
        char inp[MAX_LINE_LENGTH] = {};
        for (readUserAnswer(inp); strcmp(inp, "exit") != 0 ; readUserAnswer(inp)) {
            if (strcmp(inp, "guess") == 0) {
                BB_CHECKED(guessCharacter(akinatorInfo.root));
            }
            else if (strcmp(inp, "describe") == 0) {
                describeCharacter(akinatorInfo.root);
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
        destroyTree(akinatorInfo.root);
        free(akinatorInfo.buffer);
    }

    printf("cause its boiled\n");

    return AK_SUCCESS;
}
