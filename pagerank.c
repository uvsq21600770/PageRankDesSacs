#include "pagerank.h"

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

double* pageRank(Arc** T, int* f, int vertexAm)
{
    double* previousVector;
    double* currentVector; //vecteur en sortie
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
