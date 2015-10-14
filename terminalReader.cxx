#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include "terminalReader.hxx"

//globals
int maxBuff=100;
int maxArg=10;

//caller function must attach the result to pointer and free it
char** getLine(){
  //make buffer
  char** res=(char**)malloc(sizeof(char*)*maxArgs);
  for(int i=0;i<maxArgs;i++){
    res[i]=NULL;
  }
  //make char storage and read counter
  char c;
  int i=0;
  int j=0;

  //while loop for reading
  while(true){
    switch (c=getchar()){
      case '\n':
        res[j][i]=c;
        return res;
      break;
      case ' ':
        i=0;
        j++;
        if(j=maxArg){
          for(int k=0;k<maxArgs;k++){free(res[k]);}
          free(res);
          strcpy(res,"Error: too many Args!\n",j+1);
          return NULL;
        }
        res[j]=(char*)malloc(sizeof(char)*maxBuff)
      break;
      default:
        if(i>=maxBuff){
          for(int k=0;k<maxArgs;k++){free(res[k]);}
          free(res);
          strcpy(res,"Error: Arg %d is too long!\n",j+1);
          return NULL;
        }
        res[j][i]=c;
        i++;
      break;
    }
  }
}
