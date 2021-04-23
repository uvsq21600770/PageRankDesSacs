#include <math.h>

#ifndef __STD_LIB
#define __STD_LIB
#include <stdio.h>
#include <stdlib.h>
#endif

#define ALPHA 0.85
#define ONE_MINUS_ALPHA 0.15
#define EPSILON 0.000001

#ifndef __ARC_STRUCT
#define __ARC_STRUCT
typedef struct Arc
{
    int Id;
    double prob;
    struct Arc* next;

} Arc;
#endif

/*
  Vector X Vector multiplication
  The first vector is actually one of our list of predecessor
*/
double lineMult(Arc* A, double* vect);

/*
  Vector X Matrix
  I'll probably take lineMult out of the loop and produce a vector
  That should help speed up the exec a bit
*/
double* leftMultMatrix(Arc** T, double* vect, int vertexAm, double perturbator);
double multVectors(double* vect, int* f, int vertexAm);
double detlaVector(double* previousVector, double* currentVector, size_t vertexAm);
