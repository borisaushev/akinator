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


const int MAX_LINE_LENGTH = 100;
const int MAX_INPUT_SIZE = 100000;
const char* const AKINATOR_FILE_PATH = "../files/akinator/akinator.txt";

int runAkinator();

#endif //AKINATOR_AKINATOR_H