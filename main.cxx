#include <stdio.h>
#include <stdlib.h>
#include "terminalReader.cxx"

int main(){
  char* test=getLine();
  fprintf(stdout,test);
  free(test);
  return 1;
}
