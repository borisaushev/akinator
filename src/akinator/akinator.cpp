#include <string.h>
#include <stdio.h>

#include "treeDump.h"
#include "treeSctruct.h"
#include "akinator.h"

#include <cstdarg>

#include "stack.h"
#include "akinatorInput.h"
#include "speaker.h"

typedef struct akinatorInfo {
    treeNode* root;
    char* buffer;
} akinatorInfo_t;

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

static int printPropertiesFormatted(stack_t* properties) {
    characterProperty prop = {};
    size_t propCount = getStackElementCount(properties);

    for (size_t i = 0; i < propCount; i++) {
        SAFE_CALL(stackPop(properties, &prop));

        if (i == 0) {
            printf("  ");
        }
        else if (i % 2 == 0) {
            int linker = rand() % LINKERS_COUNT;
            printf("  %s ", AKINATOR_LINKERS[linker]);
            speak("%s", AKINATOR_LINKERS[linker]);
        } else {
            printf("  and ");
            speak("and");
        }

        if (!prop.answer) {
            printf("NOT ");
            speak("not");
        }
        printf("%s\n", prop.question);
        speak("%s", prop.question);
    }
    if (propCount > 0) {
        printf("\n");
    }

    return AK_SUCCESS;
}


static int describeCharacter(treeNode_t* root) {
    printf("Which character do you want me to describe?\n");
    char character[MAX_LINE_LENGTH];
    readUserAnswer(character);

    stack_t propertyStack = {};
    initStack(&propertyStack, STACK_BASE_SIZE);

    if (!describeRec(root, character, &propertyStack)) {
        printf(" - character was not found =(\n");
        return AK_SUCCESS;
    }

    printf("\n%s is:\n", character);
    speak("%s is", character);
    SAFE_CALL(printPropertiesFormatted(&propertyStack));

    speakFlush();

    stackDestroy(&propertyStack);

    return AK_SUCCESS;
}

static int distributeProperties(stack_t* propertyStack1, stack_t* propertyStack2,
                         stack_t* char1Unique,    stack_t* char2Unique,
                         stack_t* commonProperties) {
    characterProperty prop1 = {}, prop2 = {};
    while (getStackElementCount(propertyStack1) > 0 && getStackElementCount(propertyStack2) > 0) {
        SAFE_CALL(stackPop(propertyStack1, &prop1));
        SAFE_CALL(stackPop(propertyStack2, &prop2));

        if (strcmp(prop1.question, prop2.question) == 0 && prop1.answer == prop2.answer) {
            SAFE_CALL(stackPush(commonProperties, prop1));
        } else {
            SAFE_CALL(stackPush(char1Unique, prop1));
            SAFE_CALL(stackPush(char2Unique, prop2));
        }
    }

    // Добавляем оставшиеся свойства
    while (getStackElementCount(propertyStack1) > 0) {
        SAFE_CALL(stackPop(propertyStack1, &prop1));
        SAFE_CALL(stackPush(char1Unique, prop1));
    }
    while (getStackElementCount(propertyStack2) > 0) {
        SAFE_CALL(stackPop(propertyStack2, &prop2));
        SAFE_CALL(stackPush(char2Unique, prop2));
    }

    return AK_SUCCESS;
}

static int compareCharacters(treeNode_t* root) {
    printf("Which character do you want me to compare?\n");
    char character1[MAX_LINE_LENGTH];
    readUserAnswer(character1);

    stack_t propertyStack1 = {};
    initStack(&propertyStack1, STACK_BASE_SIZE);
    if (!describeRec(root, character1, &propertyStack1)) {
        printf(" - character was not found =(\n");
        return AK_SUCCESS;
    }

    printf("Which character do you want me to compare %s to?\n", character1);
    char character2[MAX_LINE_LENGTH];
    readUserAnswer(character2);

    stack_t propertyStack2 = {};
    initStack(&propertyStack2, STACK_BASE_SIZE);
    if (!describeRec(root, character2, &propertyStack2)) {
        printf(" - character was not found =(\n");
        return AK_SUCCESS;
    }

    // разделяем свойства
    stack_t commonProperties = {};
    stack_t char1Unique = {};
    stack_t char2Unique = {};
    initStack(&commonProperties, STACK_BASE_SIZE);
    initStack(&char1Unique, STACK_BASE_SIZE);
    initStack(&char2Unique, STACK_BASE_SIZE);

    SAFE_CALL(distributeProperties(&propertyStack1, &propertyStack2,
                                   &char1Unique,    &char2Unique,
                                   &commonProperties));

    if (getStackElementCount(&commonProperties) > 0) {
        printf("\nBoth %s and %s are:\n", character1, character2);
        speak("Both %s and %s are", character1, character2);
        SAFE_CALL(printPropertiesFormatted(&commonProperties));
    }

    // Уникальные свойства
    if (getStackElementCount(&char1Unique) > 0) {
        printf("\nUnlike %s, %s is:\n", character2, character1);
        speak("Unlike %s %s is", character2, character1);
        SAFE_CALL(printPropertiesFormatted(&char1Unique));
    }

    if (getStackElementCount(&char2Unique) > 0) {
        printf("\n%s, unlike %s, is:\n", character2, character1);
        speak("%s unlike %s is", character2, character1);
        SAFE_CALL(printPropertiesFormatted(&char2Unique));
    }

    speakFlush();

    stackDestroy(&commonProperties);
    stackDestroy(&char1Unique);
    stackDestroy(&char2Unique);
    stackDestroy(&propertyStack1);
    stackDestroy(&propertyStack2);

    return AK_SUCCESS;
}

int runAkinator() {
    akinatorInfo_t akinatorInfo = {};
    BB_TRY {
        BB_CHECKED(initTree(&akinatorInfo));
        printf("options: \n"
                              "\t'guess' - play akinator guess game\n"
                              "\t'describe' - give a definition of a character\n"
                              "\t'compare' - compare characters\n"
                              "\t'exit' - end program\n");
        char inp[MAX_LINE_LENGTH] = {};
        for (readUserAnswer(inp); strcmp(inp, "exit") != 0 ; readUserAnswer(inp)) {
            if (strcmp(inp, "guess") == 0) {
                BB_CHECKED(guessCharacter(akinatorInfo.root));
            }
            else if (strcmp(inp, "describe") == 0) {
                SAFE_CALL(describeCharacter(akinatorInfo.root));
            }
            else if (strcmp(inp, "compare") == 0) {
                SAFE_CALL(compareCharacters(akinatorInfo.root));
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
