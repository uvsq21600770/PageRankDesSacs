#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define SIZE_ARRAY 15
#define VERTEX_FIRST true

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
    Converts one number from the file into an int
*/
int getNumberM(FILE* fp, bool *reachEOLF)
{         
    char stringInt[SIZE_ARRAY];
    fpos_t pos;
        for(int i = 0; i < SIZE_ARRAY; i++)
            stringInt[i] = '\0';
        
        int result = -1;
        
        int i = 0;
        char ch = '\0';
        
        //Fill up the array with the int
        ch = fgetc(fp);
        //printf("ch:<%c>\n", ch);
        while( !isspace(ch) && ch != '\n' && ch != EOF && i < SIZE_ARRAY)
        {
            isNumber(ch);
            stringInt[i] = ch;
            ch = fgetc(fp);
            //printf("ch:<%c>\n", ch);
            i++;
        }
        
        //Checks if we reached the end of the line or EOF

            fgetpos(fp, &pos);
            ch = fgetc(fp);
            //printf("ch:<%c>\n", ch);
            while( isspace(ch) || ch == '\n')
            {
                fgetpos(fp, &pos);
                ch = fgetc(fp);
                //printf("ch:<%c>\n", ch);
            }
            fsetpos(fp, &pos);
            
        if(ch == '\n' || ch == EOF)
        {
            *reachEOLF = true;
        }
    //printf("Number = %d from string <%s> and ch:<%c>\n", atoi(stringInt), stringInt, ch);
        //Ignores whitechars at the end of the line -- MIGHT COME BACK TO FUCK MY ASS
        if(atoi(stringInt) == 0 && stringInt[0] != 0)
            {/*printf("Number = %d from string <%s> and ch:<%c>\n", atoi(stringInt), stringInt, ch); */return -1;}
            
    result = atoi(stringInt);
    
    //printf("Number = %d from string %s\n", result, stringInt);
    return result;
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

/*
    Converts one number from the file into a float
*/
float getFloatM(FILE* fp, bool *reachEOLF)
{
    char stringFloat[SIZE_ARRAY];
    char* tailptr;
    //Init the array
    for(int i = 0; i < SIZE_ARRAY; i++)
        stringFloat[i] = '\0';
    
    float result = -1.0;
    
    int i = 0;
    char ch = '\0';

    //Fill up the array with the float
    ch = fgetc(fp);
    while(ch != ' ' && ch != '\n' && ch != '\r' && ch != EOF && i < SIZE_ARRAY)
    {
        isNumber(ch);
        stringFloat[i] = ch;
        ch = fgetc(fp);
        
        i++;
    }
    
    //Checks if we reached th end of the line or EOF
    if(ch == '\n' || ch == '\r' || ch == EOF)
        *reachEOLF = true;
        
    //Ignores Whitechars at the end of the line
    result = strtof(stringFloat, &tailptr);
    if(result == 0)
    {
        if(strcmp(stringFloat, tailptr) == 0)
        {
            return -1;
        }
    }
    printf("Le float lu est <%.6f> et le string lu est %s\n", result, stringFloat);
    
    return result;  
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
    //surveille si on atteind la fin d'une ligne
    bool reachEOLF = false;
    
    //Read Vertex ID
    vertexRead = parseInt(fp, &reachEOLF);
    if(vertexRead != currentVertex)
    {
        printf("Vertex Mismatch: Lu <%d> VS Attendu <%d>\n", vertexRead, currentVertex);
        exit(EXIT_FAILURE);
    }
    
    //Read Arc Amount
    amArcToRead = parseInt(fp, &reachEOLF);
    
    //Read the what's left of the line
    //Then create a new Arc from the data read
    while(reachEOLF == false && amArcToRead != 0)
    {
        //Vertex First
        if(VERTEX_FIRST)
        {
            destVertex = parseInt(fp, &reachEOLF);
            if(destVertex != -1)
            {
                weight = parseDouble(fp, &reachEOLF);
                addArc(currentVertex, destVertex, weight, T);
                //printf("Arc added was: Orig<%d> Dest<%d> Weight<%f>\n", currentVertex, destVertex, weight);
                amArcRead++;
            }
        }
        else{
            //Weight First
            weight = parseDouble(fp, &reachEOLF);
            if(weight != -1)
            {
                destVertex = parseInt(fp, &reachEOLF);
                addArc(currentVertex, destVertex, weight, T);
                amArcRead++;
            }
        }
        
    }
    //Checks the amount of Arcs read VS the amount of Arcs that the matrix announced
    if(amArcRead != amArcToRead)
    {
        printf("Arc amount Mismatch: Lu <%d> VS Attendu <%d>\n", amArcRead, amArcToRead);
        exit(EXIT_FAILURE);
    }
    
    return amArcRead;
}


/*
    Builds the entire Hollow Matrix
*/
void buildHollowMatrix(FILE* fp, int vertexAmm, int arcAmm, Arc** T)
{
    int arcRead = 0;
    for(int i = 0; i < vertexAmm ; i++)
    {
        //printf("I = %d\n", i);
        if(i%50000 == 0)
            printf("I = %d\n", i);
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
    
    fp = fopen("DOS/WIN_Stanford_10K.txt", "r"); //Je hardcode le file car j'ai la flemme de le saisir à chaque fois
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