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

void getMaxMinAveragePert(double* vector, int size)
{
  double max = 0.0; int   max_i = -1;
  double min = 1.0; int   min_i = -1;
  double average = 0.0;

  for(int i = 0; i < size; i++)
  {
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
  }

  average = average/(double)size;
  double init = 1.0/(double)size;
  printf("Max Pert is <%.24lf> at i = <%d> for the vertex <%d>\n", max, max_i, max_i+1);
  printf("Min Pert is <%.24lf> at i = <%d> for the vertex <%d>\n", min, min_i, min_i+1);
  printf("Average Pert is <%.24lf>\n", average);
  printf("Initial value is <%.24lf>\n", init);
  printf("--------------------------\n");

  printf("Max-Average = <%.24lf> and <%lf>%%\n", max-average, (average/max)*100);
  printf("Min-Average = <%.24lf> and <%lf>%%\n", min-average, (average/min)*100);
  printf("Init-Average = <%.24lf> and <%lf>%%\n", init-average, (average/init)*100);

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

    fp = fopen("wb-edu.txt", "r"); //Je hardcode le file car j'ai la flemme de le saisir à chaque fois
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
    if(targetMode != Custom_Pertinence)
    {
      //On lance PageRank une première fois pour obtenir la pertinence de tous les sommets (hotmis les attaquants)
      //On détermine ensuite un sommet qui correspond à notre type de cible

      currentVector = pageRank(T, f, vertexAm/*Oui c'est normal*/);
      getMaxMinAveragePert(currentVector, vertexAm);
      /*DETERMINATION DE LA CIBLE*/
    }

    /*printf("Target Before:\n");
    followLinks(T[target-1], target-1);
    printf("Continue ?\n");
    scanf("%*c %*c");*/
    printf("---------------BEFORE---------------\n");
    /*for(int i = 0; i < vertexAm; i++)
    {
      followLinks(T[i], i);
    }*/
    printf("---------------AFTER---------------\n");
    setupBombsGenesis(T, vertexAm, totalVertexAm, target, bombStructure);

    /*printf("Target After:\n");
    followLinks(T[target-1], target-1);
    printf("Continue ?\n");
    scanf("%*c %*c");*/
    /*for(int i = 0; i < totalVertexAm; i++)
    {
      followLinks(T[i], i);
    }*/


    //Lance PageRank avec les attaquants (même si ils sont pas encore là lol)
    //currentVector = pageRank(T, f/*MODIFIER F POUR PRENDRE LES ATTAQUANTS EN COMPTE*/, totalVertexAm);


    //Si j'étais pas un sac ici y aurait des free, lol
    //displayVect(currentVector, vertexAm);
    //displayVect(currentVector, vertexAm);
    end = clock();
    printf("Completion took <%f>s\n", (double)(end - start) / (double)(CLOCKS_PER_SEC));
    fclose(fp);
    return 0;
}
