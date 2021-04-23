#ifndef __BOMBS_H
#define __BOMBS_H

#include "modes.h"
#include "parser.h"

void attachBombsToTarget(Arc** T, int origVertexAm, int totalVertexAm, int targetID, double proba);

void setupCompleteBombGraph(Arc** T, int origVertexAm, int totalVertexAm, double proba);
void setupRingBombGraph(Arc** T, int origVertexAm, int totalVertexAm, double proba);

void setupBombsGenesis(Arc** T, int origVertexAm, int totalVertexAm, int targetID, int bombStruct);

#endif
