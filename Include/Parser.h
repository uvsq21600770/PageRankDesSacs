#ifndef __PARSER_H
#define __PARSER_H

#ifndef __STD_LIB
#define __STD_LIB
  #include <stdio.h>
  #include <stdlib.h>
#endif

#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define SIZE_ARRAY 15
#define MAX_SIZE 262144 //2^18 just enough for wb-edu, we could increase it if we need to load bigger lines

#ifndef __ARC_STRUCT
#define __ARC_STRUCT
typedef struct Arc
{
    int Id;
    double prob;
    struct Arc* next;

} Arc;
#endif

void addArc(int orig, int dest, double weight, Arc** T);

void isNumber(char ch);
int parseInt(FILE* fp, bool *reachEOLF); //Will be removed once I stop being a lazy cunt

void fetchLine(FILE* fp, char** line, size_t* len);
size_t fetchString(char* line, size_t totalLen, size_t startingPos, char** subString);
double fetchDouble(char* line, size_t totalLen, size_t* startingPos);
int fetchInt(char* line, size_t totalLen, size_t* startingPos);

int readLineArc(FILE* fp, int currentVertex, Arc** T, int* f);
void buildHollowMatrix(FILE* fp, int vertexAm, int arcAm, Arc** T, int* f);
#endif
