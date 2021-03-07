#ifndef __MODES_H
#define __MODES_H
#include <stdlib.h>
#include <stdio.h>

enum PertinenceMode{Haute_Pertinence = 1, Moyenne_Pertinence, Faible_pertinence, Custom_Pertinence};

int chooseTargetMode();
int ChooseCustomTarget(int vertexAm);
#endif
