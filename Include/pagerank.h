#ifndef __PAGERANK_H
#define __PAGERANK_H
#include "matrixVectorCalc.h"
#include "display.h"


//Structure pour conserver l'Id des sommets et leur pertinence, utile lors du tri
//qui sert a déterminer la target
typedef struct Id_Pert
{
    int Id;
    double pertinence;

} Id_Pert;


Arc** allocateVertexRail(int size);
int* allocateFVector(int size);
int compareId_Pert(const void* a, const void* b);  //Fonction de comparaison d'une struct Id_Pert pour le qsort
double* pageRank(Arc** T, int* f, int vertexAm);
void getMaxMin(double* vector, int* category, int category_i, int* max, int* min);
void getMaxMinAveragePert(double* vector, int size);


#endif 
