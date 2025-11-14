#ifndef AKINATOR_AKINATOR_H
#define AKINATOR_AKINATOR_H
#include "common.h"

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


const char* const AKINATOR_LINKERS[] = {
    "also", "additionally", "furthermore", "moreover",
    "whats more", "on top of that", "besides", "as well",
    "likewise", "similarly", "in addition", "not to mention"
};
const int LINKERS_COUNT = sizeof(AKINATOR_LINKERS) / sizeof(AKINATOR_LINKERS[0]);

const char* const AKINATOR_OPTIONS = "options: \n"
        "\t'guess' - play akinator guess game\n"
        "\t'describe' - give a definition of a character\n"
        "\t'compare' - compare characters\n"
        "\t'exit' - end program\n";

const char AK_RESTRICTED_CHARACTERS[] = {'(', ')', '\'', '"'};


const int MAX_LINE_LENGTH = 100;
const int MAX_INPUT_SIZE = 100000;
const char* const AKINATOR_FILE_PATH = "../files/akinator/akinator.txt";

int runAkinator();

#endif //AKINATOR_AKINATOR_H