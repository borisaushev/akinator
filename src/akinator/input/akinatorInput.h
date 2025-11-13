#ifndef AKINATOR_AKINATORINPUT_H
#define AKINATOR_AKINATORINPUT_H
#include "akinator.h"
#include "treeSctruct.h"


long get_file_size(const char* filename);

void skipSpaces(char** curPos);

int read_file(const char *file_path, char** text, int* bytes_read);

int parseNode(char** curPos, treeNode_t** cur);

void readUserAnswer(char inp[MAX_LINE_LENGTH]);

void writeNodeRec(treeNode_t* node, FILE* file);

int saveAkinatorData(treeNode_t* root);

#endif //AKINATOR_AKINATORINPUT_H