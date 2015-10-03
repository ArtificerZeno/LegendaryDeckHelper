#include <stdio.h>
#include <stdlib.h>
#include "terminalReader.hxx"

//globals
int maxLine=100;

//caller function must attach the result to pointer and free it
char* getLine(){
  //make buffer
  char* res=(char*)malloc(sizeof(char)*maxLine);
  //make char storage and read counter
  char c;
  int i=0;

  //while loop for reading
  while(true){
    if((c=getchar())!='\n'){
      res[i]=c;
      i++;
    }
    else{
      res[i]=c;
      i++;
      return res;
    }
    if(i>=maxLine){
      memset(res,0,maxLine);
      strcpy(res,"Error Too Long!\n");
      return res;
    }
  }
}
