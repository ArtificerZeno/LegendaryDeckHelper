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
  char** res=(char**)malloc(sizeof(char*)*maxArg);
  res[0]=(char*)malloc(sizeof(char*)*maxBuff);
  for(int i=1;i<maxArg;i++){
    res[i]=NULL;
  }
  //make char storage and read counter
  char c;
  int i=0;
  int j=0;

  //while loop for reading
  while(true){
    c=getchar();
    if(c=='\n'||c=='\r'){
      return res;
    }
    else if(c==' '){
      if(j==maxArg){
        for(int k=0;k<maxArg;k++){free(res[k]);}
        free(res);
        fprintf(stdout,"Terminal Error: too many Args!\n");
        return NULL;
      }
      i=0;
      j++;
      res[j]=(char*)malloc(sizeof(char)*maxBuff);
    }
    else{
        if(i>=maxBuff){
          for(int k=0;k<maxArg;k++){free(res[k]);}
          free(res);
          fprintf(stdout,"Terminal Error: Arg %d is too long!\n",j+1);
          return NULL;
        }
        res[j][i]=c;
        i++;
    }
  }
}
