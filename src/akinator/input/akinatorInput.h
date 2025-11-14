#ifndef AKINATOR_AKINATORINPUT_H
#define AKINATOR_AKINATORINPUT_H
#include "akinator.h"
#include "treeSctruct.h"

const char* const NULL_NODE_STRING = "nil";

long get_file_size(const char* filename);

void skipSpaces(char** curPos);

int readFile(const char *file_path, char** text, int* bytes_read);

int parseNode(char** curPos, treeNode_t** cur, const char* buffer);

void readUserAnswer(char inp[MAX_LINE_LENGTH]);

void writeNodeRec(treeNode_t* node, FILE* file);

int saveAkinatorData(treeNode_t* root);

#endif //AKINATOR_AKINATORINPUT_H