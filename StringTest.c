#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main()
{

  /*char* str;
  str = malloc(128);
  int ent[10];

  for(int i = 0; i < 127; i++)
  {
    str[i] = (char)((i%10)+48);
    //ent[i] = i;
  }
  str[127] = '\0';
  //ent[9] = 9;

  char arr[] = "012345678";

  /*printf("str is <%s>\narr is <%s>\n\n", str, arr);

  printf("str at 4 offset is <%c>\narr at 4 offset is <%s>\n\n", (str+4)[0], arr+4);
  printf("ent is <");
  for(int i = 0; i < 10; i++)
  {
    printf("%d", ent[i]);
  }
  printf(">\n");
  printf("ent at 4 is <%d>, at 4sizeofINT <%d>\n", (ent + 4)[0], (ent + (4*sizeof(int)))[0]);*/
/*
  char* tmpString;
  tmpString = malloc(25);
  printf("ANTE sizeof tmpString <%d>\n",strlen(tmpString));
  strncpy(tmpString, str+4 , 24);
  tmpString[24] = '\0';
  printf("tmpString three cpy \n<%s>, sizeof tmpString <%d>\n\nstr\n<%s>\n\nstr+4\n<%s>\n", tmpString, strlen(tmpString), str, str+4);
  */

  size_t *startingPos;
  *startingPos = 0;
  size_t currentPos = *startingPos;

  char line[] = "1 2 2 0.5 3 0.5";
  size_t totalLen = strlen(line);

  while(currentPos < totalLen)
  {
    printf("New Loop\n");
    while(!isspace(line[currentPos]) && currentPos < totalLen)
    {
        currentPos++;
    }
    int len = currentPos - *startingPos;
    printf("len<%d> currentPos<%d> startingPos<%d>\n", len, currentPos, *startingPos);
    //TO be safe
    if(len == 0)
    {
      printf("currentPos = startingPos in fetchVertexID\n");
      exit(EXIT_FAILURE);
    }

    //We need one more byte to allow space for the terminating '\0'
    char* tmpString = malloc(len + 1);
    strncpy(tmpString, line + *startingPos, len);
    tmpString[len] = '\0';

    //set the pos to the next value
    while(isspace(line[currentPos]) && currentPos < totalLen)
    {
      currentPos++;
    }
    printf("Line is: <%s>\nsubLine is <%s>\n", line, tmpString);
    printf("currentPos= <%d>\n\n", currentPos);
    *startingPos = currentPos;
    free(tmpString);
  }
  return 9;
}
