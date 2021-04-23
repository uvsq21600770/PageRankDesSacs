#include "display.h"
/*
    Parcours une liste, affiche les prédécesseurs
*/
void followLinks(Arc* A, int i)
{
    Arc* tmp = A;
    printf("<%d>:\n", i+1);
    while(tmp != NULL)
    {
        printf("%d a pour predecesseur %d for a weight of %f\n", i+1, tmp->Id, tmp->prob);
        tmp = tmp->next;
    }
    printf("\n");
}

void followLinksWithPertinence(Arc* A, int i, double* currentVector)
{
    Arc* tmp = A;
    printf("<%d> -- <%lf>:\n", i+1, currentVector[i]);
    while(tmp != NULL)
    {
        printf("%d a pour predecesseur %d for a weight of %f\n", i+1, tmp->Id, tmp->prob);
        tmp = tmp->next;
    }
    printf("\n");
}

void displayGraph(Arc** T, int size, double* currentVector)
{
  for(int i = 0; i < size ; i++)
      {
          followLinksWithPertinence(T[i], i, currentVector);
      }
}

void displayGraphB(Arc** T, int size)
{
  for(int i = 0; i < size ; i++)
      {
          followLinks(T[i], i);
      }
}

/*
  Prints a Vector
*/
void displayVect(double* vect, int vertexAm)
{
    printf("Vect:<");
    double sum = 0.0;
    for(int i = 0; i < vertexAm; i++)
    {
        //printf("%0.6f ", vect[i]);
        sum += vect[i];
    }
    printf(">\nTotal <%f>\n", sum);
}

void displayCategory(char* level, double* vector, int* category, int category_i, double category_AVG, int vertexAm)
{
  int ammountToDisplay = 10;
  printf("----%s [%d] [%.0lf%%]----\n",level ,category_i+1, ((double)category_i/(double)vertexAm)*100.0);
  for(int i = 0; (i < ammountToDisplay) && (i <= category_i); i++)
  {
    printf("[%d]  %0.24lf\n", category[i], vector[category[i]]);
  }
  printf("[A] [%0.24lf]\n", category_AVG);
}


//Retourne la cible de l'attaque
int acquireTargetId(int vertexAm, double* currentVector, int targetMode)
{
  //Dump le vecteur de pertinence dans une structure qui associe sommet et pertinence
  //printf("Creating temp vector\n");
  Id_Pert* orderedVector = malloc(vertexAm * sizeof(Id_Pert));
  for(int i = 0; i < vertexAm; i++)
  {
    orderedVector[i].Id = i;
    orderedVector[i].pertinence = currentVector[i];
  }

  //Tri du vecteur
  //printf("Sorting vector\n");
  qsort(orderedVector, vertexAm, sizeof(Id_Pert), compareId_Pert);
  /*for(int i = 0; i < vertexAm; i++)
  {
    printf("[%d] linked to [%d]\n",i, orderedVector[i].Id);
  }*/
  //printf("Vector sorted\n");

  int target = -1;
  //Détermine l'ID de la target dans le vecteur trié en fonction de la pertinence recherchée
  //On utilise le premier quartile pour faible, mediane pour moyenne et troisième quartile pour forte
  switch (targetMode)
  {
    case Faible_pertinence :
      target = vertexAm/4;
      break;

    case Moyenne_Pertinence :
      target = vertexAm/2;
      break;

    case Haute_Pertinence :
      target = (3*vertexAm)/4;
      break;
  }

  printf("Target = %d -- %d\n", target, orderedVector[target].Id+1);
  return orderedVector[target].Id+1;

}

void analysis(int target, double targetPertinence, int targetMode, int bombAmount, int bombStructure, double* currentVector)
{
  printf("\n\n--- Settings ---\n");

  switch(targetMode)
  {
    case Haute_Pertinence:
      printf("Haute pertinence\n");
      break;

    case Moyenne_Pertinence:
      printf("Moyenne pertinence\n");
      break;

    case Faible_pertinence:
      printf("Faible pertinence\n");
      break;
  }

  printf("Bomb Amount <%d>\n", bombAmount);

  switch(bombStructure)
  {
    case Graphe_Complet:
      printf("Complet\n");
      break;

    case Graphe_Anneau:
      printf("Anneau\n");
      break;

    case Sommets_Isoles:
      printf("Singulier\n");
      break;
  }

  printf("\n--- Nouvelle Pertinence ---\n");
  printf("[%d] = <%.24lf>\n", target, currentVector[target-1]);

  if(targetMode != Custom_Pertinence)
  {
    printf("\n--- Original Pertinence ---\n");
    printf("[%d] = <%.24lf>\n", target, targetPertinence);
    printf("\nDelta = <%.24lf>\n", currentVector[target-1] - targetPertinence);
  }
}