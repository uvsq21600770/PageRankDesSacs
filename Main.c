#include <time.h>

#include "Modes.h"
#include "Parser.h"
#include "MatrixVectorCalc.h"
#include "Bombs.h"

//I'm keeping all my defines here that way when I change them for real in the place where they're actually
//declared I'll feel extra dumb for believing these are the actual fucking values.

/*#define SIZE_ARRAY 15
#define MAX_SIZE 262144 //2^18 just enough for wb-edu, we could increase it if we need to load bigger lines
#define ALPHA 0.85
#define ONE_MINUS_ALPHA 0.15
#define EPSILON 0.000001*/


clock_t start, end;

/******
typedef struct Arc
{
    int Id;
    double prob;
    struct Arc* next;

} Arc;
*******/


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


 Arc** allocateVertexRail(int size)
 {
     Arc** T = malloc(sizeof(Arc*) * size);
     for(int i = 0; i < size; i++)
     {
         T[i] = NULL;
     }

     return T;
 }

 int* allocateFVector(int size)
 {
    int* f;
    f = malloc(sizeof(int) * size);
       for(int i = 0; i < size; i++)
        {
           f[i] = 0;
        }
    return f;
 }

double* pageRank(Arc** T, int* f, int vertexAm)
{
    double* previousVector;
    double* currentVector; //C'EST CELUI QU'ON VA SORTIR ESPECE DE SAC
    double* tmpVectorPointer;

    double median = 1.0/vertexAm;
    double xft = 0.0;
    double perturbator = 0.0;
    double delta = 10*EPSILON;

    //It will be free instantly when we enter the loop so we don't need to give an actual meaningful amount, it will soon take the value of currentVector anyway
    //That probably could have been avoided but I'm currently eating diner so you can go fuck right off a cliff
    previousVector = malloc(1);
    currentVector = malloc(sizeof(double) * vertexAm);

    for(int i = 0; i < vertexAm; i++)
    {
        currentVector[i] = 1.0/vertexAm;
        //currentVector[i] = 1.0/3.0;
    }

    int iteration = 0;
    while(delta > EPSILON)
    {

        iteration++;
        printf("Iteration <%d> -- ", iteration);

        xft = multVectors(currentVector, f, vertexAm);
        //printf("XFT = <%f>\n", xft);
        perturbator = (ONE_MINUS_ALPHA * median) + ((ALPHA * median) * xft);
//printf("perturbator = <%f> --- median <%f> A*M*XFT <%f>\n", perturbator, median, (ALPHA * median) * xft);

        tmpVectorPointer = leftMultMatrix(T, currentVector, vertexAm, perturbator);

        free(previousVector);
        previousVector = currentVector;
        currentVector = tmpVectorPointer;

        delta = detlaVector(previousVector, currentVector, vertexAm);
        printf("delta <%0.10f>\n", delta);
        //displayVect(currentVector, vertexAm);
        //displayVect(previousVector, vertexAm); printf("\n");
        //if(iteration > 2) return 0;
    }

    return currentVector;
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

void getMaxMin(double* vector, int* category, int category_i, int* max, int* min)
{

  *max = 0;
  *min = 0;

  for(int i = 0; i <= category_i; i++)
  {

    if(vector[category[i]] > vector[category[*max]])
    {
      *max = i;
    }

    if(vector[category[i]] < vector[category[*min]])
    {
      *min = i;
    }

  }
  //printf("min<%d> max<%d> cat<%d>\n", *min, *max, category_i);
}

void getMaxMinAveragePert(double* vector, int size)
{
  double max = 0.0; int   max_i = -1;
  double min = 1.0; int   min_i = -1;
  double average = 0.0;
  double init = 1.0/(double)size;

  double high_threshold = init * 0.70;
  double low_threshold = init * 0.30;

  int* low = malloc(size*sizeof(int));
  int* medium = malloc(size*sizeof(int));
  int* high = malloc(size*sizeof(int));

  int low_i = -1;
  int medium_i = -1;
  int high_i = -1;

  double low_AVG = 0.0;
  double medium_AVG = 0.0;
  double high_AVG = 0.0;

  for(int i = 0; i < size; i++)
  {

    //MAX MIN AVERAGE CALCUL

    if(vector[i] > max)
    {
      max = vector[i];
      max_i = i;
    }

    if(vector[i] < min)
    {
      min = vector[i];
      min_i = i;
    }
    average += vector[i];

    //LOW HIGH MEDIUM PERTINENCE ATTRIBUTION

    if(vector[i] > high_threshold)
    {
      high[++high_i] = i;
      high_AVG += vector[i];
    }
    else if(vector[i] < low_threshold)
    {
      low[++low_i] = i;
      low_AVG += vector[i];
    }
    else
    {
      medium[++medium_i] = i;
      medium_AVG += vector[i];
    }
  }

  average = average/(double)size;
  high_AVG = high_AVG/(double)size;
  low_AVG = low_AVG/(double)size;
  medium_AVG = medium_AVG/(double)size;

  printf("Max Pert is <%.24lf> at i = <%d> for the vertex <%d>\n", max, max_i, max_i+1);
  printf("Min Pert is <%.24lf> at i = <%d> for the vertex <%d>\n", min, min_i, min_i+1);
  printf("Average Pert is <%.24lf>\n", average);
  printf("Initial value is <%.24lf>\n", init);
  printf("Low Threshold  <%.24lf>\n", low_threshold);
  printf("High Threshold <%.24lf>", high_threshold);
  printf("--------------------------\n");
/*
  printf("Max-Average = <%.24lf> and <%lf>%%\n", max-average, (average/max)*100);
  printf("Min-Average = <%.24lf> and <%lf>%%\n", min-average, (average/min)*100);
  printf("Init-Average = <%.24lf> and <%lf>%%\n", init-average, (average/init)*100);*/

  //They're IDs of their respective vectors
  int low_min = 0, medium_min = 0, high_min = 0, low_max = 0, medium_max = 0, high_max = 0; //le compilo casse les couilles

  if(medium_i > 0)
  {
    displayCategory("LOW", vector, low, low_i, low_AVG, size);
    getMaxMin(vector, low, low_i, &low_max, &low_min);
    printf("min<%d> max<%d>, lmi<%d>, lma<%d>\n", low_min, low_max, low[low_min], low[low_max]);
    printf("LOW MINIMUM [%d]\n[%.24lf]\n", low[low_min], vector[low[low_min]]);
    printf("LOW MAXIMUM [%d]\n[%.24lf]\n\n", low[low_max], vector[low[low_max]]);
  }
  else printf("NO LOW VERTEX\n\n;");

  if(medium_i > 0)
  {
    displayCategory("MEDIUM", vector, medium, medium_i, medium_AVG, size);
    getMaxMin(vector, medium, medium_i, &medium_max, &medium_min);
    printf("MEDIUM MINIMUM [%d]\n[%.24lf]\n", medium[medium_min], vector[medium[medium_min]]);
    printf("MEDIUM MAXIMUM [%d]\n[%.24lf]\n\n",medium[medium_max], vector[medium[medium_max]]);
  }
  else printf("NO MEDIUM VERTEX\n\n;");


  if(high_i > 0)
  {
    displayCategory("HIGH", vector, high, high_i, high_AVG, size);
    getMaxMin(vector, high, high_i, &high_max, &high_min);
    printf("HIGH MINIMUM [%d]\n[%.24lf]\n", high[high_min], vector[high[high_min]]);
    printf("HIGH MAXIMUM [%d]\n[%.24lf]\n\n", high[high_max], vector[high[high_max]]);
  }
  else printf("No HIGH VERTEX\n\n;");
}


//Structure pour conserver l'Id des sommets et leur pertinence, utile lors du tri
//qui sert a déterminer la target
typedef struct Id_Pert
{
    int Id;
    double pertinence;

} Id_Pert;

//Fonction de comparaison d'une struct Id_Pert pour le qsort
int compareId_Pert(const void* a, const void* b)
{
    if(((Id_Pert*)a)->pertinence < ((Id_Pert*)b)->pertinence)
        return -1;
    else if(((Id_Pert*)a)->pertinence > ((Id_Pert*)b)->pertinence)
        return 1;
    else
        return 0;
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
    printf("\n--- Originial Pertinence ---\n");
    printf("[%d] = <%.24lf>\n", target, targetPertinence);
    printf("\nDelta = <%.24lf>\n", currentVector[target-1] - targetPertinence);
  }
}

int main(){

    //For exec time mesuring
    start = clock();

    char ch, file_name[25];
    FILE *fp;
    bool reachEOLF;

    int targetMode = -1;
    int target = -1;
    int bombAmount = -1;
    int bombStructure = -1;

    Arc** T;
    int* f;
    double* currentVector;

    printf("C'est quoi le nom de ton putain de fichier\n");
    //gets(file_name);

    fp = fopen("wiki2005.txt", "r"); //Je hardcode le file car j'ai la flemme de le saisir à chaque fois
    if (fp == NULL)
       {
          perror("Error while opening the file.\n");
          exit(EXIT_FAILURE);
       }

    //Gets the amount of Vertexes in the matrix
    int vertexAm = parseInt(fp, &reachEOLF);
    printf("vertexAm = %d\n", vertexAm);
    //median = 1.0/vertexAm; //We're moving it

    //Gets the amount of Arcs in the matrix
    int arcAm = parseInt(fp, &reachEOLF);
    printf("arcAm = %d\n", arcAm);

//+--------CHOIX DU MODE--------+//
    targetMode = chooseTargetMode();
    printf("Target Mode <%d>\n", targetMode);
    if(targetMode == Custom_Pertinence)
    {
      target = chooseCustomTarget(vertexAm);
      printf("Target <%d>\n", target);
    }

//+--------CHOIX DE L'ATTAQUE--------+//
    bombAmount = chooseBombAmount(); //On devrait peut-être faire de la vérif sur ça un jour
    bombStructure = chooseBombStructure();
    //return 420;

//+--------Réservation de la mémoire pour la Matrice + Les attaquants--------+//
    int totalVertexAm = vertexAm + bombAmount;
    //La tringle du rideau
    T = allocateVertexRail(totalVertexAm);

    //the "f" from XFT
    f = allocateFVector(totalVertexAm);

    //Construit la Matrice depuis le fichier
    //On se restraint à "vertexAm" sommets comme les attaquants n'entrent pas encore en jeu
    buildHollowMatrix(fp, vertexAm/*Oui c'est normal*/, arcAm, T, f);
    end = clock();
    printf("Loading the file into the structure took <%f>s\n", (double)(end - start) / (double)(CLOCKS_PER_SEC));

//+--------PageRank Préliminaire pour les Modes [1-3]--------+//

    double targetPertinence = -1;
    if(targetMode != Custom_Pertinence)
    {
      //On lance PageRank une première fois pour obtenir la pertinence de tous les sommets (hormis les attaquants)
      //On détermine ensuite un sommet qui correspond à notre type de cible
      currentVector = pageRank(T, f, vertexAm/*Oui c'est normal*/);

      /*DETERMINATION DE LA CIBLE*/
      target = acquireTargetId(vertexAm, currentVector, targetMode);

      //Pour la comparaison finale
      targetPertinence = currentVector[target-1];

      //On est pas des sauvages
      //printf("Before Bombs\n");
      //displayGraph(T, vertexAm, currentVector);
      free(currentVector);
    }

    setupBombsGenesis(T, vertexAm, totalVertexAm, target, bombStructure);

    for(int i = vertexAm; i < totalVertexAm; i++)
    {
      f[i] = 0;
    }


    //Lance PageRank avec les attaquants (même si ils sont pas encore là lol)
    //MODIFIER F POUR PRENDRE LES ATTAQUANTS EN COMPTE ? Pas sûr d'avoir besoin de mettre autre chose que des 0 sur les bombes au final
    currentVector = pageRank(T, f, totalVertexAm);
    //printf("After Bombs\n");
    //displayGraph(T, totalVertexAm, currentVector);

    analysis(target, targetPertinence, targetMode, bombAmount, bombStructure, currentVector);

    //Si j'étais pas un sac ici y aurait des free, lol
    //displayVect(currentVector, vertexAm);
    //displayVect(currentVector, vertexAm);
    end = clock();
    printf("Completion took <%f>s\n", (double)(end - start) / (double)(CLOCKS_PER_SEC));
    fclose(fp);
    return 0;
}
