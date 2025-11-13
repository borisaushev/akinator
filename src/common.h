#ifndef TREE_COMMON_H
#define TREE_COMMON_H

#include <stdio.h>
#include <io.h>
#include <math.h>
#include <cassert>
#include <string.h>
#include <sys/stat.h>
#include <filesystem>




const char* const HTML_FILE_PATH = "..\\files\\logs\\tree\\tree_dump.html";
const char* const DOT_FILE_PATH = "..\\files\\logs\\tree\\images\\tree.dot";
const char* const SVG_FORMAT = "..\\files\\logs\\tree\\images\\tree_%zu.svg";

const int MAX_TREE_DEPTH = 10000;

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
        int sf_call ## __LINE__ = (func); \
        if (sf_call ## __LINE__ != 0) { \
            return sf_call ## __LINE__; \
        } \
    END

#define      BB_TRY                  int __tx_error = 0; { goto __tx_try; } __tx_try: {
#define      BB_CHECKED( cmd )       { if ((__tx_error = (cmd)) != 0) {goto __tx_catch;}}
#define      BB_CATCH                goto __tx_finally; __tx_catch: {
#define      BB_ENDCATCH             return __tx_error;}
#define      BB_FAIL( error_code )   { __tx_error = error_code; goto __tx_catch; }
#define      BB_FINALLY              __tx_finally:
#define      BB_ENDTRY               }

#define FAIL_ERR(code, desc) \
    BEGIN \
        PRINTERR("ERROR [%s:%d]: %s (code %d)\n", __FILE__, __LINE__, desc, code); \
        BB_FAIL(code); \
    END

#endif //TREE_COMMON_H