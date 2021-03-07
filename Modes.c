#include "Modes.h"

int chooseTargetMode()
{
  int mode = -1;
  printf("Choisissez la pertinence du sommet à attaquer\n");
  printf("1 - Haute Pertinence\n");
  printf("2 - Moyenne Pertinence\n");
  printf("3 - Faible Pertinence\n");
  printf("4 - Sommet au Choix\n");
  printf("[1/2/3/4]\n");

  scanf("%1d", &mode); //On lit 1 et 1 seul character en ignorant les characters blancs

  if(mode < 1 || mode > 4)
  {
    printf("Option non reconnue\n");
    exit(EXIT_FAILURE);
  }
  return mode;
}

int ChooseCustomTarget(int vertexAm)
{
  int target = -1;
  printf("Choisir le sommet à attaquer\n");
  printf("[0 - %d]\n", vertexAm);
  scanf("%d", &target);

  if(target < 0 || target > vertexAm)
  {
    printf("Sommet non présent dans le graphe\n");
    exit(EXIT_FAILURE);
  }

  return target;

}
