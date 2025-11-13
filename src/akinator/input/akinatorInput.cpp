#include "akinatorInput.h"

#include "common.h"
#include "akinator.h"
#include "treeDump.h"


long get_file_size(const char* filename) {
    struct stat st = {};
    if (stat(filename, &st) == 0) {
        return st.st_size;
    }

    return -1;
}

void skipSpaces(char** curPos) {
    while (isspace(**curPos)) {
        (*curPos)++;
    }
}

int read_file(const char *file_path, char** text, int* bytes_read) {
    int stream = open(file_path, 0);

    long file_size = get_file_size(file_path);
    DPRINTF("file size: %lu\n", file_size);

    if (file_size < 0) {
        PRINTERR("Could not open file %s\n", file_path);
        RETURN_ERR(AK_FILE_NOT_FOUND, "Could not open file");
    }
    *text = (char *) calloc((size_t) file_size, sizeof(char));
    *bytes_read = read(stream, *text, (unsigned int) file_size);

    if (*bytes_read == -1) {
        PRINTERR("Could not read file %s with err: %s\n", file_path, strerror(errno));
        RETURN_ERR(AK_FILE_NOT_FOUND, "Could not read file");
    }
    DPRINTF("Read %d bytes\n", *bytes_read);
    close(stream);

    *text = (char *) realloc(*text,  (size_t) *bytes_read + 2);
    (*text)[*bytes_read] = (*text)[*bytes_read-1] == '\n' ? '\0' : '\n';
    (*text)[*bytes_read] = (*text)[*bytes_read-1] == '\n' ? '\0' : '\n';
    (*text)[*bytes_read + 1] = '\0';

    return 0;
}


int parseNode(char** curPos, treeNode_t** cur) {
    treeLog("Parsing node with input: </p>%s", *curPos);
    if (**curPos == '(') {
        treeLog("Read '(' ");
        skipSpaces(curPos);
        (*curPos)++;
        skipSpaces(curPos);
        treeLog("skipped '('");
        treeLog("remaining string: </p>%s", *curPos);
        if (**curPos != '"') {
            treeLog("expected '\"'");
            RETURN_ERR(AK_FILE_NOT_FOUND, "expected '\"'");
        }

        char* end = strchr(*curPos + 1, '"');
        if (end == NULL) {
            treeLog("Havent found the end of the string");
            RETURN_ERR(AK_INVALID_INPUT, "invalid input tree");
        }

        *end = '\0';
        createNode(*curPos + 1, false, cur);
        treeLog("Created new node");
        TREE_DUMP(*cur, "Created node", AK_SUCCESS);

        *curPos = end + 1;

        skipSpaces(curPos);
        treeLog("skipped data");
        treeLog("remaining string: </p>%s", *curPos);

        treeLog("Parsing left subtree");
        SAFE_CALL(parseNode(curPos, &(*cur)->left));
        treeLog("Parsed left subtree");
        TREE_DUMP(*cur, "Parsed left subtree", AK_SUCCESS);


        treeLog("Parsing right subtree");
        SAFE_CALL(parseNode(curPos, &(*cur)->right));
        treeLog("Parsed right subtree");
        TREE_DUMP(*cur, "Parsed right subtree", AK_SUCCESS);

        skipSpaces(curPos);
        if (**curPos != ')') {
            RETURN_ERR(AK_INVALID_INPUT, "expected ')'");
        }
        (*curPos)++;
        skipSpaces(curPos);
        treeLog("skipped ')'");
        treeLog("remaining string: </p>%s", *curPos);

        return AK_SUCCESS;
    }
    else {
        if (strncmp(*curPos, NULL_NODE_STRING, 3) == 0) {
            treeLog("found nil node");
            *curPos += strlen(NULL_NODE_STRING);

            treeLog("Skipped nil, remaining str: %s", *curPos);
            return AK_SUCCESS;
        }
        else {
            RETURN_ERR(AK_INVALID_INPUT, "invalid input tree");
        }
    }
}

void readUserAnswer(char inp[MAX_LINE_LENGTH]) {
    scanf("%[^\n]", inp);
    getchar();
}

void writeNodeRec(treeNode_t* node, FILE* file) {
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

int saveAkinatorData(treeNode_t* root) {
    FILE* file = fopen(AKINATOR_FILE_PATH, "w");
    if (file == NULL) {
        RETURN_ERR(AK_NULL_PTR, "unable to open file");
    }
    writeNodeRec(root, file);

    fclose(file);
    return AK_SUCCESS;
}