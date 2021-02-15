#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define SIZE_ARRAY 15
#define MAX_SIZE 262144 //2^18 just enough for wb-edu, we could increase it if we need to load bigger lines
#define EPSILON 0.000001

clock_t start, end;

typedef struct Arc
{
    int Id;
    double prob;
    struct Arc* next;

} Arc;


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
    Adds an arc in the Arc** Array
    dest needs to be the vertexID of the destination, we sub 1 to offset it properly in the array
*/
void addArc(int orig, int dest, double weight, Arc** T)
{
    Arc* newArc = malloc(sizeof(Arc));
            newArc->Id = orig;
            newArc->prob = weight;
            newArc->next = T[dest-1];

    T[dest-1] = newArc;
}

/*
   Checks if a char is a number
*/
void isNumber(char ch)
{
    if((ch < '0' || ch > '9') && ch !=  '.')
            {
                printf("T'as rentre un mauvais fichier, charread: <%c>", ch);
                exit(EXIT_FAILURE);
            }
}

/*
  Legacy code, still used at the beginning of the main, it used to be extensively used in the Test parser
  It's fucking trash lol
*/
int parseInt(FILE* fp, bool *reachEOLF)
{
    char stringInt[SIZE_ARRAY];
    fpos_t pos;
        for(int i = 0; i < SIZE_ARRAY; i++)
            stringInt[i] = '\0';
            stringInt[0] = 'B';
    int result = -1;

    int i = 0;
    char ch = '\0';

    ch = fgetc(fp);
    //printf("ch:<%c> hex<%x>\n",ch, ch);
        while( !isspace(ch) && (ch != '\n' || ch != '\r') && ch != EOF && i < SIZE_ARRAY)
        {
            isNumber(ch);
            stringInt[i] = ch;

            ch = fgetc(fp);
            //printf("ch:<%c> hex<%x>\n",ch, ch);
            i++;
        }

        //This is the work of god
        fgetpos(fp, &pos);
        while(isspace(ch))
        {
            fgetpos(fp, &pos);
            if(ch == '\n' || ch == '\r')
                 {/*printf("[INT]TRUE\n");*/ *reachEOLF = true;}
            ch = fgetc(fp);
            //printf("ch:<%c> hex<<%x>>\n", ch, ch);
        }
        if(ch != EOF)
            fsetpos(fp, &pos);
        else
            *reachEOLF = true;


    //printf("[INT]String read was <%s> last  is <%c> hex<%x>\n\n", stringInt, ch, ch);
    result = atoi(stringInt);
    return result;
}

/*
  Useless in the NewParser
*/
double parseDouble(FILE* fp, bool *reachEOLF)
{

    //printf("Call DOUBLE\n");

    char stringDouble[SIZE_ARRAY];
    char* tailptr;
    fpos_t pos;
        for(int i = 0; i < SIZE_ARRAY; i++)
            stringDouble[i] = '\0';
            stringDouble[0] = 'B';
    double result = -1.0;

    int i = 0;
    char ch = '\0';

    ch = fgetc(fp);
    //printf("ch:<%c>\n",ch);
        while( !isspace(ch) && (ch != '\n' || ch != '\r') && ch != EOF && i < SIZE_ARRAY)
        {
            isNumber(ch);
            stringDouble[i] = ch;

            ch = fgetc(fp);
            //printf("ch:<%c>\n",ch);
            i++;
        }

        fgetpos(fp, &pos);
        while(isspace(ch))
        {
            fgetpos(fp, &pos);
            if(ch == '\n' || ch == '\r')
                {/*printf("[DOUBLE]TRUE\n");*/ *reachEOLF = true;}
            ch = fgetc(fp);
        }
        if(ch != EOF)
                fsetpos(fp, &pos);
        else
            *reachEOLF = true;


    //printf("[DOUBLE]String read was <%s> last char is <%c> hex<%x>\n\n", stringDouble, ch, ch);
    result = strtod(stringDouble, &tailptr);
        if(result == 0)
        {
            if(strcmp(stringDouble, tailptr) == 0)
            {
                return -1;
            }
        }
    return result;
}


/*
  Reads a line and dynamically allocate enough memory to store it in char** line
  We first try to make it fit in 128bits then 256 then 512 etc with a max size of MAX_SIZE (currently 4096)
*/
void fetchLine(FILE* fp, char** line, size_t* len)
{
    char chunk[256];
    if(line == NULL || len == NULL || fp == NULL)
    {
        printf("You fucked up somewhere in ReadLineArc\n");
        exit(EXIT_FAILURE);
    }

    if(*line == NULL || *len < sizeof chunk)
    {
        *len = sizeof chunk;
        *line = malloc(*len);
        if(*line == NULL)
            {
                printf("Failed to allocate memory\n");
                exit(EXIT_FAILURE);
            }
    }
    (*line)[0] = '\0';
    size_t len_used =strlen(*line);
    size_t chunk_used =strlen(chunk);
    while(fgets(chunk, sizeof chunk, fp) != NULL)
    {
        len_used =strlen(*line);
        chunk_used =strlen(chunk);

        if(*len - len_used < chunk_used)
        {
            if(*len > MAX_SIZE / 2)
            {
                printf("Stop trying to shove 131K+ chars in the memory you wonderful person (just go to the top and change MAX_SIZE)\n");
                exit(EXIT_FAILURE);
            }
            else
                *len *= 2;
            if((*line = realloc(*line, *len)) == NULL)
            {
                printf("Failed to [RE]allocate memory\n");
                exit(EXIT_FAILURE);
            }
        }
            memcpy(*line + len_used, chunk, chunk_used);
            len_used += chunk_used;
            (*line)[len_used] = '\0';
            if((*line)[len_used - 1] == '\n')
            {
                //printf("CRLF has been found\n");
                break;
            }
    }
        //printf("[IN]String read <%s>\n", *line);
}

/*
  Finds the first non white block of characters in a string and copy it to subString
  Then finds the beginning of the next non white block and returns the position of the indice
*/
size_t fetchString(char* line, size_t totalLen, size_t startingPos, char** subString)
{

  size_t currentPos = startingPos;
  while(!isspace(line[currentPos]) && currentPos < totalLen)
  {
      currentPos++;
  }
  int len = currentPos - startingPos;
  //printf("len<%d> currentPos<%d> startingPos<%d>\n", len, currentPos, startingPos);
  //TO be safe
  if(len == 0)
  {
    printf("currentPos = startingPos in fetchStringID\n");
    exit(EXIT_FAILURE);
  }
  //We need one more byte to allow space for the terminating '\0'
  *subString = malloc(len + 1);
  if(*subString == NULL){
      printf("Failed to allocate memory\n");
      exit(EXIT_FAILURE);
  }
  strncpy(*subString, line + startingPos, len);
  (*subString)[len] = '\0';
  //set the pos to the next value
  while(isspace(line[currentPos]) && currentPos < totalLen)
  {
    currentPos++;
  }
  //printf("Line is: <%s>\nsubLine is <%s>\n", line, *subString);
  //printf("currentPos= <%d>\n\n", currentPos);

  return currentPos;
}

/*
  Uses fetchString to get a block of chars and converts it to a double while progressing the readingHead startingPos
*/
double fetchDouble(char* line, size_t totalLen, size_t* startingPos)
{
    double result;
    char* subString;
    char* tailptr;

    *startingPos = fetchString(line, totalLen, *startingPos, &subString);
    result = strtod(subString, &tailptr);

    free(subString);
    return result;
}

/*
  Uses fetchString to get a block of chars and converts it to an int while progressing the readingHead startingPos
*/
int fetchInt(char* line, size_t totalLen, size_t* startingPos)
{
    int result;
    char* subString;

    *startingPos = fetchString(line, totalLen, *startingPos, &subString);
    result = atoi(subString);

    free(subString);
    return result;
}

/*
    Reads one line of the TXT Matrix
    Creates and place all the new arcs inside the hollow matrix
*/
int readLineArc(FILE* fp, int currentVertex, Arc** T, double* f)
{

    int amArcRead = 0;
    int amArcToRead = 0;
    int vertexRead = 0;
    int destVertex = 0;
    double weight = 0.0;

    char* line = NULL;
    size_t len = 0;
    size_t startingPos = 0;
    fetchLine(fp, &line, &len);
    //printf("[OUT]String read <%s>\n", line);

    //Reads the vertex ID
    vertexRead = fetchInt(line, len, &startingPos);
    //printf("VertexID <%d>\n", vertexRead);

    //Reads the Arc amount
    amArcToRead = fetchInt(line, len, &startingPos);
    if(amArcToRead == 0)
      f[currentVertex] = 1.0;
    //printf("Amount of Arcs to read <%d>\n", amArcToRead);

    //Reads and creates all the Arc of the line
    while(amArcRead < amArcToRead)
    {
        destVertex = fetchInt(line, len, &startingPos);
        //printf("destVertex <%d>", destVertex);
        weight = fetchDouble(line, len, &startingPos);
        //printf(" for a weight of <%f>\n", weight);

        addArc(currentVertex, destVertex, weight, T);

        amArcRead++;
    }

    free(line);
    //printf("Line Over\n\n");
    return amArcRead;
}


/*
    Builds the entire Hollow Matrix
*/
void buildHollowMatrix(FILE* fp, int vertexAm, int arcAm, Arc** T, double* f)
{
    int arcRead = 0;
    for(int i = 0; i < vertexAm /*1*/ ; i++)
    {
        /*
        //Used to see some stuff happen on the screen while we load big ass files
        if(i%50000  == 0) printf("I = %d\n", i);
        if(i%1000000 == 0)
        {
          end = clock();
          printf("Reaching %d took <%f>s\n",i ,(double)(end - start) / (double)(CLOCKS_PER_SEC));
        }*/
        arcRead += readLineArc(fp, i+1, T, f);
    }
    if(arcRead != arcAm)
        {
            printf("Total Arc amount Mismatch: Lu <%d> VS Attendu <%d>\n", arcRead, arcAm);
            exit(EXIT_FAILURE);
        }

    //Just checks if everyone is linked together properly
/*
    for(int i = 0; i < vertexAm ; i++)
        {
            followLinks(T[i], i);
        }
*/
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
        printf("%0.6f ", vect[i]);
        sum += vect[i];
    }
    printf(">\nTotal <%f>\n", sum);
}
/*
  Vector X Vector multiplication
  The first vector is actually one of our list of predecessor
*/
double lineMult(Arc* A, double* vect)
{
    double res = 0.0;
    Arc* tmp = A;
        while(tmp != NULL)
        {
            res += tmp->prob * vect[tmp->Id -1];
            //printf("res(%f) = prob(%f) * vect(%f) stepRes <%f> tmp->Id<%d>\n", res, tmp->prob,vect[tmp->Id -1], tmp->prob * vect[tmp->Id -1], tmp->Id - 1);
            tmp = tmp->next;
        }
        //printf("\n");
    return res;
}
/*
  Vector X Matrix
  I'll probably take lineMult out of the loop and produce a vector
  That should help speed up the exec a bit
*/
void xP(Arc** T, double* vect, int vertexAm, double* tmpVectorPointer)
{
    //printf("Alpha: >%f< perturbator <%f>\n", ALPHA, perturbator);
    for(int i = 0; i < vertexAm; i++)
    {
      //  printf("I<%d>:\n", i);
        tmpVectorPointer[i] = lineMult(T[i], vect);
        //printf("LM <%f>, newVect[%d] <%f>, calc <%f>\n", lineMult(T[i], vect),i ,newVect[i], ALPHA * (lineMult(T[i], vect)) + perturbator);
    }

}

void xG(double* currentVector, int vertexAm, double* xP, double perturbator)
{
    for(int i=0; i < vertexAm; i++)
    {
      currentVector[i] = 0.85 * xP[i] + perturbator;
    }
}

double multVectors(double* vect, double* f, int vertexAm)
 {
    double res = 0.0;
    for(int i = 0; i < vertexAm; i++)
    {
        res += vect[i] * (double)f[i];
        //printf("res<%f> += vect[%d]<%f> * (double)f[%d]<%f> --- F[%d]<%f\n", res,i ,vect[i],i ,(double)f[i],i,f[i]);
    }

    return res;
 }

double detlaVector(double* previousVector, double* currentVector, size_t vertexAm)
 {
   double result = 0.0;
   double PV = 0.0;
   double CV = 0.0;

   /*for(int i = 0; i < vertexAm; i++)
      {
         PV += previousVector[i];
         CV += currentVector[i];
      }
   result = CV - PV;
   printf("result <%f> CV <%f> PV <%f>\n", result, CV, PV);*/

   for(int i = 0; i < vertexAm; i++)
      {
         result += fabs(previousVector[i] - currentVector[i]);
         //printf("result <%f> CV <%f> PV <%f>\n", result, currentVector[i], previousVector[i]);
      }
   return result;
 }

 void copyVector(double* target, double* orig, int vertexAm)
 {
   for(int i=0; i < vertexAm; i++)
      target[i] = orig[i];
 }

int main(){

    //For exec time mesuring
    start = clock();

    char ch, file_name[25];
    FILE *fp;
    bool reachEOLF;

    printf("C'est quoi le nom de ton putain de fichier\n");
    //gets(file_name);

    fp = fopen("DOS/web4.txt", "r"); //Je hardcode le file car j'ai la flemme de le saisir à chaque fois
    if (fp == NULL)
       {
          perror("Error while opening the file.\n");
          exit(EXIT_FAILURE);
       }

    //Gets the amount of Vertexes in the matrix
    int vertexAm = parseInt(fp, &reachEOLF);
    printf("vertexAm = %d\n", vertexAm);

    //La tringle du rideau
    Arc** T = malloc(sizeof(Arc*) * vertexAm);
    for(int i = 0; i < vertexAm; i++)
    {
        T[i] = NULL;
    }
    double* f = malloc(sizeof(double)* vertexAm);
    for(int i=0; i<vertexAm; i++)
        f[i] = 0.0;

    //Gets the amount of Arcs in the matrix
    int arcAm = parseInt(fp, &reachEOLF);
    printf("arcAm = %d\n", arcAm);

    //Build the hollow matrix from the file
    buildHollowMatrix(fp, vertexAm, arcAm, T, f);

    end = clock();
    printf("Loading the file into the structure took <%f>s\n", (double)(end - start) / (double)(CLOCKS_PER_SEC));

    double* previousVector;
    double* currentVector;
    double* tmpVectorPointer;
    double val_1 = (1.0 - 0.85) * (1.0/vertexAm);
    double val_2 = 0.85 * (1.0/vertexAm);
    double delta = 10*EPSILON;
    double XFT = 0.0;
    double perturbator = 0.0;

    //It will be free instantly when we enter the loop so we don't need to give an actual meaningful amount, it will soon take the value of currentVector anyway
    //That probably could have been avoided but I'm currently eating diner so you can go fuck right off a cliff
    previousVector = malloc(sizeof(double) * vertexAm);
    currentVector = malloc(sizeof(double) * vertexAm);
    tmpVectorPointer = malloc(sizeof(double) * vertexAm);

    for(int i = 0; i < vertexAm; i++)
    {
        currentVector[i] = 1.0/vertexAm;
        //currentVector[i] = 1.0/3.0;
    }


    int iteration = 1;
    while(delta > EPSILON /*iteration < 100*/)
    {

        printf("-----------------------\nIteration <%d>\n", iteration);
        copyVector(previousVector, currentVector, vertexAm);
        XFT = multVectors(currentVector, f, vertexAm);
        perturbator = val_1 + val_2 * XFT;
        xP(T, currentVector, vertexAm, tmpVectorPointer);
        xG(currentVector, vertexAm, tmpVectorPointer, perturbator);

        delta = detlaVector(previousVector, currentVector, vertexAm);
        printf("delta <%0.10f>\n", delta);
        displayVect(currentVector, vertexAm); printf("\n");
        //displayVect(previousVector, vertexAm); printf("\n");
        //if(iteration > 2) return 0;
        iteration++;
    }


    //Si j'étais pas un sac ici y aurait des free, lol
    //displayVect(currentVector, vertexAm);
    end = clock();
    printf("Completion took <%f>s\n", (double)(end - start) / (double)(CLOCKS_PER_SEC));
    fclose(fp);
    return 0;
}
