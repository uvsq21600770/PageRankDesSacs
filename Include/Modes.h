#ifndef __MODES_H
#define __MODES_H
#include <stdlib.h>
#include <stdio.h>

enum PertinenceMode{Haute_Pertinence = 1, Moyenne_Pertinence, Faible_pertinence, Custom_Pertinence};
enum BombStructure{Graphe_Complet = 1, Graphe_Anneau, Sommets_Isoles};

int chooseTargetMode();
int chooseCustomTarget(int vertexAm);
int chooseBombAmount();
int chooseBombStructure();
#endif
