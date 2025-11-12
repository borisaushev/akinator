#include <string.h>
#include "akinator.h"

#include "treeDump.h"
#include "treeSctruct.h"

#include <stdio.h>

typedef struct akinatorInfo {
    treeNode* root;
    char* buffer;
} akinatorInfo_t;

static long get_file_size(const char* filename) {
    struct stat st = {};
    if (stat(filename, &st) == 0) {
        return st.st_size;
    }
    else {
        return -1;
    }
}

static void skipSpaces(char** curPos) {
    while (isspace(**curPos)) {
        (*curPos)++;
    }
}

static int read_file(const char *file_path, char** text, int* bytes_read) {
    int stream = open(file_path, 0);

    long file_size = get_file_size(file_path);
    DPRINTF("file size: %lu\n", file_size);

    if (file_size < 0) {
        PRINTERR("Could not open file %s\n", file_path);
        RETURN_ERR(AK_FILE_NOT_FOUND, "Could not open file");
    }
    *text = (char *) calloc(file_size, sizeof(char));
    *bytes_read = read(stream, *text, file_size);

    if (*bytes_read == -1) {
        PRINTERR("Could not read file %s with err: %s\n", file_path, strerror(errno));
        RETURN_ERR(AK_FILE_NOT_FOUND, "Could not read file");
    }
    DPRINTF("Read %d bytes\n", *bytes_read);
    close(stream);

    *text = (char *) realloc(*text,  *bytes_read + 2);
    (*text)[*bytes_read] = (*text)[*bytes_read-1] == '\n' ? '\0' : '\n';
    (*text)[*bytes_read + 1] = '\0';

    return 0;
}

//TODO вывести это говно в дамп
//TODO разделить на файлы

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

static void readUserAnswer(char inp[MAX_LINE_LENGTH]) {
    scanf("%[^\n]", inp);
    getchar();
}

static int parseNode(char** curPos, treeNode_t** cur) {
    DPRINTF("Parsing node with input: %s\n", *curPos);
    if (**curPos == '(') {
        DPRINTF("Read '(' \n");
        skipSpaces(curPos);
        (*curPos)++;
        skipSpaces(curPos);
        DPRINTF("skipped '(', remaining string: %s\n", *curPos);
        if (**curPos != '"') {
            DPRINTF("expected '\"'\n");
            RETURN_ERR(AK_FILE_NOT_FOUND, "expected '\"'");
        }

        char* end = strchr(*curPos + 1, '"');
        if (end == NULL) {
            DPRINTF("Havent found the end of the string\n");
            RETURN_ERR(AK_INVALID_INPUT, "invalid input tree");
        }

        *end = '\0';
        createNode(*curPos + 1, false, cur);
        DPRINTF("Created new node\n");
        TREE_DUMP(*cur, "Created node", AK_SUCCESS); //TODO

        *curPos = end + 1;

        skipSpaces(curPos);
        DPRINTF("skipped data, remaining str: %s\n", *curPos);

        DPRINTF("Parsing left subtree\n");
        SAFE_CALL(parseNode(curPos, &(*cur)->left));
        DPRINTF("Parsed left subtree\n");
        TREE_DUMP(*cur, "Parsed left subtree", AK_SUCCESS);


        DPRINTF("Parsing right subtree\n");
        SAFE_CALL(parseNode(curPos, &(*cur)->right));
        DPRINTF("Parsed right subtree\n");
        TREE_DUMP(*cur, "Parsed right subtree", AK_SUCCESS);

        skipSpaces(curPos);
        if (**curPos != ')') {
            RETURN_ERR(AK_INVALID_INPUT, "expected ')'");
        }
        (*curPos)++;
        skipSpaces(curPos);
        DPRINTF("skipped ')', remaining str: %s\n", *curPos);

        return AK_SUCCESS;
    }
    else if (strncmp(*curPos, "nil", 3) == 0) {
        DPRINTF("found nil node\n");
        *curPos += 3;

        DPRINTF("Skipped nil, remaining str: %s\n", *curPos);
        return AK_SUCCESS;
    }
    else {
        RETURN_ERR(AK_INVALID_INPUT, "invalid input tree");
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

static int saveAkinatorData(treeNode_t* root) {
    FILE* file = fopen(AKINATOR_FILE_PATH, "w");
    if (file == NULL) {
        RETURN_ERR(AK_NULL_PTR, "unable to open file");
    }
    writeNodeRec(root, file);

    fclose(file);
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
 * - 1 if the node is related to the character (is itself a character, or one of the children is related to the character)
 * - 0 otherwise
 */
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

void describeCharacter(treeNode_t* root, char inp[MAX_LINE_LENGTH]) {
    printf("Which character do you want me to describe?\n");
    readUserAnswer(inp);
    if (describeRec(root, inp) == 0) {
        printf(" - character was not found =(\n");
    }
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
                describeCharacter(akinatorInfo.root, inp);
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
