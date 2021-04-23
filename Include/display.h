#ifndef __DISPLAY_H
#define __DISPLAY_H
#include "pagerank.h"
#include "modes.h"

void followLinks(Arc* A, int i); // Parcours une liste, affiche les prédécesseurs
void followLinksWithPertinence(Arc* A, int i, double* currentVector);
void displayGraph(Arc** T, int size, double* currentVector);
void displayGraphB(Arc** T, int size);
void displayVect(double* vect, int vertexAm); //Prints a Vector
void displayCategory(char* level, double* vector, int* category, int category_i, double category_AVG, int vertexAm);
int acquireTargetId(int vertexAm, double* currentVector, int targetMode); ////Retourne la cible de l'attaque
void analysis(int target, double targetPertinence, int targetMode, int bombAmount, int bombStructure, double* currentVector);




#endif 