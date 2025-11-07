#ifndef TREE_COMMON_H
#define TREE_COMMON_H

#include <stdio.h>
#include <io.h>
#include <math.h>
#include <cassert>
#include <string.h>
#include <sys/stat.h>
#include <filesystem>


typedef enum errors {
    SUCCESS = 0,
    NULL_PTR,
    INVALID_CAPACITY,
    INVALID_INDEX,
    INVALID_PTR,
    CONTENTS_MODIFIED,
    CANT_OPEN_FILE,
    FILE_NOT_FOUND,
    FILE_NOT_READABLE,
    INVALID_INPUT
} error_t;


const char* const HTML_FILE_PATH = "..\\files\\logs\\tree\\tree_dump.html";
const char* const DOT_FILE_PATH = "..\\files\\logs\\tree\\images\\tree.dot";
const char* const SVG_FORMAT = "..\\files\\logs\\tree\\images\\tree_%zu.svg";

const int MAX_REASONABLE_SIZE = 10000;

#define BEGIN do {
#define END   } while (0)

#ifdef DEBUG
#define DPRINTF(...) \
    BEGIN \
        printf("\033[33m"); \
        printf(__VA_ARGS__); \
        printf("\033[0m"); \
        fflush(stdout); \
    END
#else
    #define DPRINTF(...) ;
#endif //DEBUG

//define for printing in stderr
#define PRINTERR(...) \
    BEGIN \
        fprintf(stderr, "\033[31m"); \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\033[0m"); \
        fflush(stderr); \
    END

#define RETURN_ERR(code, desc) \
    BEGIN \
        PRINTERR("ERROR [%s:%d]: %s (code %d)\n", __FILE__, __LINE__, desc, code); \
        return code; \
    END

#define SAFE_CALL(func) \
    BEGIN \
        error_t sf_call ## __LINE__ = (func); \
        if (sf_call ## __LINE__ != SUCCESS) { \
            return sf_call ## __LINE__; \
        } \
    END

#endif //TREE_COMMON_H