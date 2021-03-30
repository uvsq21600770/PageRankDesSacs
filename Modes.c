#include "Modes.h"

int chooseTargetMode()
{
  int mode = -1;
  printf("---Choisissez la pertinence du sommet à attaquer---\n");
  printf("[1] - Haute Pertinence\n");
  printf("[2] - Moyenne Pertinence\n");
  printf("[3] - Faible Pertinence\n");
  printf("[4] - Sommet au Choix\n");
  printf("[1/2/3/4]\n");

  scanf("%1d", &mode); //On lit 1 et 1 seul character en ignorant les characters blancs
  printf("\n");
  if(mode < 1 || mode > 4)
  {
    printf("Option non reconnue\n");
    exit(EXIT_FAILURE);
  }
  return mode;
}

int chooseCustomTarget(int vertexAm)
{
  int target = -1;
  printf("---Choisir le sommet à attaquer---\n");
  printf("[1 - %d]\n", vertexAm);
  scanf("%d", &target);
  printf("\n");

  if(target < 1 || target > vertexAm)
  {
    printf("Sommet non présent dans le graphe\n");
    exit(EXIT_FAILURE);
  }

  return target;
}

int chooseBombAmount()
{
  int bombAmount = -1;
  printf("---Choisissez le nombre d'attaquants---\n");

  scanf("%d", &bombAmount); //On lit 1 et 1 seul character en ignorant les characters blancs
  printf("\n");

  if(bombAmount < 0)
  {
    printf("Impossible d'avoir un nombre d'attaquants négatif\n");
    exit(EXIT_FAILURE);
  }
  return bombAmount;
}

int chooseBombStructure()
{
  int bombStructure = -1;
  printf("---Choisissez la structure du réseau attaquant---\n");
  printf("[1] - Graphe Complet d'attaquants (tous interconnectés)\n");
  printf("[2] - Graphe Anneau\n");
  printf("[3] - Attaquants isolé, la distanciation sociale\n");
  printf("[1/2/3]\n");

  scanf("%1d", &bombStructure); //On lit 1 et 1 seul character en ignorant les characters blancs
  printf("\n");

  if(bombStructure < 1 || bombStructure > 3)
  {
    printf("Option non reconnue\n");
    exit(EXIT_FAILURE);
  }
  return bombStructure;
}
