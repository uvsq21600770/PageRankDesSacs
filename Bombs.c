#include "Bombs.h"

void attachBombToTarget(Arc** T, int origVertexAm, int totalVertexAm, int targetID, double proba)
{
  for(int currentBombID = origVertexAm+1; currentBombID <= totalVertexAm; currentBombID++)
  {
    addArc(currentBombID, targetID, proba, T);
  }
}

/*
    Que les esprits de la compléxité me pardonnent pour cette boucle for imbriquée
*/
void setupCompleteBombGraph(Arc** T, int origVertexAm, int totalVertexAm, double proba)
{

  for(int currentBombID = origVertexAm+1; currentBombID <= totalVertexAm; currentBombID++)
  {
    for(int currentNeighborID = origVertexAm+1; currentNeighborID <= totalVertexAm; currentNeighborID++)
    {
      if(currentNeighborID != currentBombID)
      {
        addArc(currentBombID, currentNeighborID, proba, T); //Ca devrait marcher lol, sinon team fauteil roulant chromé
      }
    }
  }
}

void setupRingBombGraph(Arc** T, int origVertexAm, int totalVertexAm, double proba)
{

  for(int currentBombID = origVertexAm+1; currentBombID < totalVertexAm; currentBombID++)
  {
    addArc(currentBombID, currentBombID + 1, proba, T);
  }

  addArc(totalVertexAm, origVertexAm + 1, proba, T);
}

void setupBombsGenesis(Arc** T, int origVertexAm, int totalVertexAm, int targetID, int bombStruct)
{
  double proba = -1.0;

  switch(bombStruct){

    case Graphe_Complet:
      proba = 1.0/(double)(totalVertexAm-origVertexAm); // 1.0/Nbr_Bombs
      setupCompleteBombGraph(T, origVertexAm, totalVertexAm, proba);
      break;

    case Graphe_Anneau:
      proba = 0.5;
      setupRingBombGraph(T, origVertexAm, totalVertexAm, proba);
      break;

    case Sommets_Isoles:
      proba = 1.0;
      break;
  }

  attachBombToTarget(T, origVertexAm, totalVertexAm, targetID, proba);
}
