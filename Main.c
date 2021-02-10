#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define SIZE_ARRAY 15
#define MAX_SIZE 4096

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

void fetchLine(FILE* fp, char** line, size_t* len)
{
    char chunk[128];
    if(line == NULL || len == NULL || fp == NULL)
    {
        printf("You fucked up somewhere in ReadLineArc\n");
        exit(EXIT_FAILURE);
    }
    
    printf("STEP 1\n");
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
    printf("STEP 2\n");
    *line[0] = '\0';
    printf("STEP 3\n");
    size_t len_used =strlen(*line);
    size_t chunk_used =strlen(chunk);
    printf("STEP 4\n");
    while(fgets(chunk, sizeof chunk, fp) != NULL)
    {
        len_used =strlen(*line);
        chunk_used =strlen(chunk);
        
        printf("STEP 6\n");    
        if(*len - len_used < chunk_used)
        {
            if(*len > SIZE_MAX / 2)
            {
                printf("Stop trying to show 2K+ chars in the memory you wonderful person (just go to the top and change MAX_SIZE)\n");
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
        printf("STEP 7\n");    
            memcpy(*line + len_used, chunk, chunk_used);
            len_used += chunk_used;
            (*line)[len_used] = '\0';
        printf("STEP 7.5\n");    
            if((*line)[len_used - 1] == '\n')
            {
                printf("CRLF has been found\n");
                break;
            }
        printf("STEP 8\n");
    }
        printf("STEP 5\n");
        printf("[IN]String read <%s>\n", *line);
        
        
}

/*
    Reads one line of the TXT Matrix
    Creates and place all the new arcs inside the hollow matrix
*/
int readLineArc(FILE* fp, int currentVertex, Arc** T)
{

    int amArcRead = 0;
    int amArcToRead = 0;
    int vertexRead = 0;
    int destVertex = 0;
    double weight = 0.0;
    
    char* line = NULL;
    size_t len = 0;
    fetchLine(fp, &line, &len);
    printf("[OUT]String read <%s>\n", line);
    
    free(line);
    if(line == NULL)
        printf("[POST FREE]String read NULL\n");
    return amArcRead;
}


/*
    Builds the entire Hollow Matrix
*/
void buildHollowMatrix(FILE* fp, int vertexAmm, int arcAmm, Arc** T)
{
    int arcRead = 0;
    for(int i = 0; i < /*vertexAmm*/ 1 ; i++)
    {
        //printf("I = %d\n", i);
        arcRead += readLineArc(fp, i+1, T);
    }
    if(arcRead != arcAmm)
        {
            printf("Total Arc amount Mismatch: Lu <%d> VS Attendu <%d>\n", arcRead, arcAmm);
            exit(EXIT_FAILURE);
        }
    
    //Just checks if everyone is linked together properly
    /*
    for(int i = 0; i < vertexAmm ; i++)
        {
            followLinks(T[i], i);
        }*/

}

void displayVect(double* vect, int vertexAmm)
{
    printf("Vect:<");
    for(int i = 0; i < vertexAmm; i++)
    {
        printf("%f ", vect[i]);
    }
    printf(">\n");
}

double lineMult(Arc* A, double* vect)
{
    double res = 0.0;
    Arc* tmp = A;
        while(tmp != NULL)
        {
            res += tmp->prob * vect[tmp->Id -1];
            //printf("res(%f) = prob(%f) * vect(%f)\n", res, tmp->prob,vect[tmp->Id -1]);
            tmp = tmp->next;
        }
        
    return res;
}

double* leftMultMatrix(Arc** T, double* vect, int vertexAmm)
{
    double* newVect;
    double* tmp;
    newVect = malloc(sizeof(double) * vertexAmm);
        
    for(int i = 0; i < vertexAmm; i++)
    {
        newVect[i] = lineMult(T[i], vect);
    }
    
    tmp = vect;
    free(tmp);
    
    return newVect;
}

int main(){
    
    printf("Taille Arc*= %d\n", sizeof(Arc*));
    
    char ch, file_name[25];
    FILE *fp;
    bool reachEOLF;
    
    printf("C'est quoi le nom de ton putain de fichier\n");
    //gets(file_name);
    
    fp = fopen("DOS/WIN_tst.txt", "r"); //Je hardcode le file car j'ai la flemme de le saisir à chaque fois
    if (fp == NULL)
       {
          perror("Error while opening the file.\n");
          exit(EXIT_FAILURE);
       }
    
    //Gets the amount of Vertexes in the matrix
    int vertexAmm = parseInt(fp, &reachEOLF);
    printf("vertexAmm = %d\n", vertexAmm);
    
    //La tringle du rideau
    Arc** T = malloc(sizeof(Arc*) * vertexAmm);
    for(int i = 0; i < vertexAmm; i++)
    {
        T[i] = NULL;
    }
    
    //Gets the amount of Arcs in the matrix
    int arcAmm = parseInt(fp, &reachEOLF);
    printf("arcAmm = %d\n", arcAmm);
    
    //Build the hollow matrix from the file
    buildHollowMatrix(fp, vertexAmm, arcAmm, T);
    
    double* vect;
    vect = malloc(sizeof(double) * vertexAmm);
    for(int i = 0; i < vertexAmm; i++)
    {
        vect[i] = 1.0/vertexAmm;
    }
    
    vect = leftMultMatrix(T, vect, vertexAmm);
    displayVect(vect, vertexAmm);

    //Si j'étais pas un sac ici y aurait des free, lol
    
    fclose(fp);
    return 0;
}