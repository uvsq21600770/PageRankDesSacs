#include "matrixVectorCalc.h"

/*
  Vector X Vector multiplication
  The first vector is actually one of our list of predecessor
*/
double lineMult(Arc* A, double* vect)
{
    double res = 0.0;
    Arc* tmp = A;
        while(tmp != NULL)
        {
            res += tmp->prob * vect[tmp->Id -1];
            //printf("res(%f) = prob(%f) * vect(%f) stepRes <%f> tmp->Id<%d>\n", res, tmp->prob,vect[tmp->Id -1], tmp->prob * vect[tmp->Id -1], tmp->Id);
            tmp = tmp->next;
        }
        //printf("\n");
    return res;
}
/*
  Vector X Matrix
  I'll probably take lineMult out of the loop and produce a vector
  That should help speed up the exec a bit
*/
double* leftMultMatrix(Arc** T, double* vect, int vertexAm, double perturbator)
{
    double* newVect;
    newVect = malloc(sizeof(double) * vertexAm);

    //printf("Alpha: >%f< perturbator <%f>\n", ALPHA, perturbator);
    for(int i = 0; i < vertexAm; i++)
    {
        //printf("I<%d>:\n", i);
        newVect[i] = ALPHA * (lineMult(T[i], vect)) + perturbator;
        //printf("LM <%f>, newVect[%d] <%f>, calc <%f>\n", lineMult(T[i], vect),i ,newVect[i], ALPHA * (lineMult(T[i], vect)) + perturbator);
    }

    return newVect;
}

double multVectors(double* vect, int* f, int vertexAm)
 {
    double res = 0.0;
    for(int i = 0; i < vertexAm; i++)
    {
        res += vect[i] * f[i]*1.0;
        //printf("res<%f> += vect[%d]<%f> * (double)f[%d]<%f> --- F[%d]<%f\n", res,i ,vect[i],i ,(double)f[i],i,f[i]);
    }

    return res;
 }

double detlaVector(double* previousVector, double* currentVector, size_t vertexAm)
 {
   double result = 0.0;
   for(int i = 0; i < vertexAm; i++)
      {
         result += fabs(previousVector[i] - currentVector[i]);
      }
   return result;
 }
