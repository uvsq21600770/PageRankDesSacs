#include <time.h>

#include "Modes.h"
#include "Parser.h"
#include "MatrixVectorCalc.h"

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

int main(){

    //For exec time mesuring
    start = clock();

    char ch, file_name[25];
    FILE *fp;
    bool reachEOLF;

    int targetMode = -1;
    int target = -1;

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
      target = ChooseCustomTarget(vertexAm);
      printf("Target <%d>\n", target);
    }
    //return 420;

    //On les sort du if pour faire plaisir au compilo, à voir si on les remet dedans
    Arc** T;
    int* f;
    double* currentVector;
    if(targetMode != Custom_Pertinence)
    {
      //La tringle du rideau
      T = allocateVertexRail(vertexAm);

      //the "f" from XFT
      f = allocateFVector(vertexAm);

      //Build the hollow matrix from the file
      buildHollowMatrix(fp, vertexAm, arcAm, T, f);
      end = clock();
      printf("Loading the file into the structure took <%f>s\n", (double)(end - start) / (double)(CLOCKS_PER_SEC));
      //return 69;

      //On lance PageRank une première fois pour obtenir la pertinence de tous les sommets
      //On détermine ensuite un sommet qui correspond à notre type de cible

      currentVector = pageRank(T, f, vertexAm);
    }



    //Lance PageRank avec les attaquants (même si ils sont pas encore là lol)
    //currentVector = pageRank(T, f/*MODIFIER F POUR PRENDRE LES ATTAQUANTS EN COMPTE*/, vertexAm/*+ NOMBRE D'ATTAQUANTS*/);


    //Si j'étais pas un sac ici y aurait des free, lol
    //displayVect(currentVector, vertexAm);
    displayVect(currentVector, vertexAm);
    end = clock();
    printf("Completion took <%f>s\n", (double)(end - start) / (double)(CLOCKS_PER_SEC));
    fclose(fp);
    return 0;
}
