#ifndef AKINATOR_AKINATOR_H
#define AKINATOR_AKINATOR_H
#include "common.h"
#include "treeSctruct.h"

typedef enum akinator_error {
    AK_SUCCESS = 0,
    AK_NULL_PTR,
    AK_INVALID_CAPACITY,
    AK_INVALID_INDEX,
    AK_INVALID_PTR,
    AK_FILE_NOT_FOUND,
    AK_INVALID_INPUT,
    AK_CANT_OPEN_FILE
} akinator_error_t;

typedef struct akinatorInfo {
    treeNode* root;
    char* buffer;
} akinatorInfo_t;

const char* const AKINATOR_LINKERS[] = {
    "also",
    "additionally",
    "furthermore",
    "moreover",
    "whats more",
    "on top of that",
    "besides",
    "as well",
    "likewise",
    "similarly",
    "in addition",
    "not to mention"
};
const int LINKERS_COUNT = sizeof(AKINATOR_LINKERS) / sizeof(AKINATOR_LINKERS[0]);

const char* const AKINATOR_OPTIONS = "options: \n"
        "\t'guess' - play akinator guess game\n"
        "\t'describe' - give a definition of a character\n"
        "\t'compare' - compare characters\n"
        "\t'exit' - end program\n";

const char AK_RESTRICTED_CHARACTERS[] = {'(', ')', '\'', '"'};


const int MAX_LINE_LENGTH = 10000;
const int MAX_INPUT_SIZE = 100000;
const char* const AKINATOR_FILE_PATH = "../files/akinator/akinator.txt";

typedef int (*AkinatorCommandFunc)(treeNode *);

typedef enum akinatorCommand {
    AK_GUESS,
    AK_DESCRIBE,
    AK_COMPARE,
    AK_EXIT,
} akinatorCommand_t;

typedef struct akinatorCommandsInfo {
    const akinatorCommand_t command;
    const char* const commandName;
    AkinatorCommandFunc function;
} akinatorCommandsInfo_t;

int compareCharacters(treeNode_t* root);
int describeCharacter(treeNode_t* root);
int guessCharacter(treeNode_t* root);

const akinatorCommandsInfo_t AKINATOR_COMMANDS[] = {
    {AK_GUESS,   "guess",    guessCharacter},
    {AK_COMPARE, "describe", describeCharacter},
    {AK_EXIT,    "compare",  compareCharacters},
};
const size_t AKINATOR_COMMANDS_COUNT = sizeof(AKINATOR_COMMANDS) / sizeof(AKINATOR_COMMANDS[0]);


int runAkinator();

#endif //AKINATOR_AKINATOR_H