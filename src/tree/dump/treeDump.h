#ifndef TREE_TREEDUMP_H
#define TREE_TREEDUMP_H
#include "common.h"
#include "treeSctruct.h"


error_t treeDump(treeNode_t* node, const char* desc, const char* file, const int line, const char* func, error_t code);

#ifdef DEBUG_TREE
    #define TREE_DUMP(node, desc, error_code) \
        treeDump(node, desc, __FILE__, __LINE__, __func__, error_code)
    #define TREE_VALID(node) \
        BEGIN \
            error_t valid ## __LINE__ = validateTree(node); \
            if (valid ## __LINE__ != SUCCESS) { \
            TREE_DUMP(node, "ERROR", valid ## __LINE__); \
            PRINTERR("ERROR [%s:%d] (code %d)\n", __FILE__, __LINE__, valid ## __LINE__); \
            return valid ## __LINE__;\
            } \
        END
#else
    #define TREE_VALID(list) ;
    #define TREE_DUMP(list, desc, error_code) ;
#endif


#endif //TREE_TREEDUMP_H