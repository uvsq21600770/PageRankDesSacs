#include <time.h>

#include "modes.h"
#include "parser.h"
#include "matrixVectorCalc.h"
#include "bombs.h"


clock_t start, end;
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

    printf("Nom du fichier:\n");
    //gets(file_name);

    fp = fopen("Files/wb-edu.txt", "r"); //File to process
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

    bombStructure = chooseBombStructure();
    bombAmount = chooseBombAmount(bombStructure); //On devrait peut-être faire de la vérif sur ça un jour

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

    //Des free ici sûrement.
    //displayVect(currentVector, vertexAm);
    //displayVect(currentVector, vertexAm);
    end = clock();
    printf("Completion took <%f>s\n", (double)(end - start) / (double)(CLOCKS_PER_SEC));
    fclose(fp);
    return 0;
}
