#ifndef TREE_TREEDUMP_H
#define TREE_TREEDUMP_H
#include "common.h"
#include "treeSctruct.h"

const char* const TREE_POINTER_COLORS[] = {
    "red",
    "blue",
    "purple",
    "brown",
    "darkcyan",
    "white",
    "black",
    "yellow",
    "darkred",
    "navy",
    "magenta",
    "orange",
    "darkviolet",
    "gold",
    "crimson",
    "indigo",
    "maroon",
    "teal",
    "deeppink",
    "darkorange"
};

int treeDump(treeNode_t* node, const char* desc, const char* file, const int line, const char* func, int code, const char* fillColor);
int treeLog(const char* message, ...);

#ifdef DEBUG_TREE
    #define TREE_DUMP_COLORED(node, desc, error_code, fillColor) \
        treeDump(node, desc, __FILE__, __LINE__, __func__, error_code, fillColor)
    #define TREE_DUMP(node, desc, error_code) \
        TREE_DUMP_COLORED(node, desc, error_code, "#56db70")
    #define TREE_VALID(node) \
        BEGIN \
            int valid ## __LINE__ = validateTree(node); \
            if (valid ## __LINE__ != 0) { \
            TREE_DUMP(node, "ERROR", valid ## __LINE__); \
            PRINTERR("ERROR [%s:%d] (code %d)\n", __FILE__, __LINE__, valid ## __LINE__); \
            return valid ## __LINE__;\
            } \
        END
#else
    #define TREE_DUMP_COLORED(node, desc, error_code, fillColor) ;
    #define TREE_VALID(list) ;
    #define TREE_DUMP(list, desc, error_code) ;
#endif


#endif //TREE_TREEDUMP_H