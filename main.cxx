#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "terminalReader.cxx"

const char* mkHerror1="Format Error: makeHero n (-Y a)(-y a)(-U b)-u b)(-B c)(-b c)(-R d)(-r d)(-G e)(-g e)\n";
const char* ucError="Error: Unknown Command\n";
const char* uError="Unknown Error\n";
int totHeros,YCHeros,UCHeros,BCHeros,RCHeros,GCHeros;
int maxArg=10;
int maxBuff=100;

typedef struct Hero_Entry{
  char* name;
  int values[5];
} Hero;

void freeHero(Hero* h){
  free(h->name);
  free(h);
}

typedef struct Color_Hero_Entry{
  Hero* hero;
} CHero;

void freeCHero(CHero* h){
  free(h->hero);
  free(h);
}

CHero** CHeros;
Hero* Heros;

bool eqHero(Hero *a, Hero *b){
  return if(strcmp(a,b)==0);
}

void makeHeros(char** comm){
  //check arg count
  for(int i=1;i<7;i++){
    if(comm[i]==NULL){
      fprintf(stdout, "Error: Missing Arg %d\nSyntax is \"Heros numHeros minY minU minB minR minG\" where each value is an integer.\n",i);
    }
  }
  if(comm[8]!=NULL){
    fprintf(stdout,"Error: Too many Args\n");
    return
  }

  //load the amount of heros to make the deck with
  int nH = std::stoi(comm[1]);
  if(nH<1||nH>totHeros){//reject if 0 or less or exceeds the total amount of heroes
    fprintf(stdout,"Error: %d out of %d Heros is an impossible amount of Heros to add to the deck\n",nH,totHeros);
    return;
  }

  //load the min colors
  int cM[5];
  for(int i=0;i<5;i++){
    cM[i]=std::stoi(comm[i+2]);
    if(cM[i]<0){//regect if negative
      fprintf(stdout,"Error: %d is an impossible minimum color value.\n",mC[i]);
      return;
    }
  }

  int eM=3;
  //find the list of heroes with color value >= min value - eM
  CHero **consideredHeros=(CHero**)malloc(sizeof(CHero*)*5);
  int ych,uch,bch,rch,gch; //amount of heroes considered in each color
  ych=uch=bch=rch=gch=0;
  for(int i=0;i<YCHeros;i++){
    if(CHeros[0][i]->hero.values[0]>cM[0]-eM){ych++;}//count up eligble Y heros
  }
  for(int i=0;i<UCHeros;i++){
    if(CHeros[1][i]->hero.values[1]>cM[1]-eM){uch++;}//count up eligble U heros
  }
  for(int i=0;i<BCHeros;i++){
    if(CHeros[2][i]->hero.values[2]>cM[2]-eM){bch++;}//count up eligble B heros
  }
  for(int i=0;i<RCHeros;i++){
    if(CHeros[3][i]->hero.values[3]>cM[3]-eM){rch++;}//count up eligble R heros
  }
  for(int i=0;i<GCHeros;i++){
    if(CHeros[4][i]->hero.values[4]>cM[4]-eM){gch++;}//count up eligble G heros
  }
  int tempc=0;
  consideredHeros[0]=(CHero*)malloc(sizeof(CHero)*ych);
  for(int i=0;i<YCHeros;i++){
    if(CHeros[0][i]->hero.values[0]>cM[0]-eM&&tempc<ych){//collect Y heros
      consideredHeros[0][tempc]=CHeros[0][i];
      tempc++;
    }
  }
  tempc=0;
  consideredHeros[1]=(CHero*)malloc(sizeof(CHero)*uch);
  for(int i=0;i<UCHeros;i++){
    if(CHeros[1][i]->hero.values[1]>cM[1]-eM&&tempc<uch){//collect U heros
      consideredHeros[1][tempc]=CHeros[1][i];
      tempc++;
    }
  }
  tempc=0;
  consideredHeros[2]=(CHero*)malloc(sizeof(CHero)*bch);
  for(int i=0;i<BCHeros;i++){
    if(CHeros[2][i]->hero.values[2]>cM[2]-eM&&tempc<bch){//collect B heros
      consideredHeros[2][tempc]=CHeros[2][i];
      tempc++;
    }
  }
  tempc=0;
  consideredHeros[3]=(CHero*)malloc(sizeof(CHero)*rch);
  for(int i=0;i<RCHeros;i++){
    if(CHeros[3][i]->hero.values[3]>cM[3]-eM&&tempc<rch){//collect R heros
      consideredHeros[3][tempc]=CHeros[3][i];
      tempc++;
    }
  }
  tempc=0;
  consideredHeros[4]=(CHero*)malloc(sizeof(CHero)*gch);
  for(int i=0;i<GCHeros;i++){
    if(CHeros[4][i]->hero.values[4]>cM[4]-eM&&tempc<gch){//collect G heros
      consideredHeros[4][tempc]=CHeros[4][i];
      tempc++;
    }
  }

  //select heros
  int t=100;//i have 100 tries
  while(t>0){
    int aH=0;//accepted heros
    int cV[5];//current total values
    cV[0]=cV[1]=cV[2]=cV[3]=cV[4]=0;
    Hero *aHeros=(Hero*)malloc(sizeof(Hero)*nH);//list of accepted heros to prevent repeats
    int rnd;//random Number
    bool found=false;//did i find a new hero yet or no
    if(aH<nH&&ych>0){//I have room for and will try find a Y hero
      int tt=ych;//only try as much as unique heros i have
      while(tt>0&&!found){
        rnd=rand()%ych;
        for(int i=0;i<nH;i++){
          if(!eqHero(consideredHeros[0][rnd]->hero.name,aHeros[i].name)){
            aHeros[i]=consideredHeros[0][rnd];
            found=true;
            aH++;
            for(int j=0;j<5;j++){
                cV[j]+=consideredHeros[0][rnd]->hero.values[j];
            }
          }
        }
        if(!found){tt--;}
      }
    }
    found=false;
    if(aH<nH&&uch>0){//I have room for and will try find a U hero
      int tt=uch;//only try as much as unique heros i have
      while(tt>0&&!found){
        rnd=rand()%uch;
        for(int i=0;i<nH;i++){
          if(!eqHero(consideredHeros[1][rnd]->hero.name,aHeros[i].name)){
            aHeros[i]=consideredHeros[1][rnd];
            found=true;
            aH++;
            for(int j=0;j<5;j++){
                cV[j]+=consideredHeros[1][rnd]->hero.values[j];
            }
          }
        }
        if(!found){tt--;}
      }
    }
    found=false;
    if(aH<nH&&bch>0){//I have room for and will try find a B hero
      int tt=bch;//only try as much as unique heros i have
      while(tt>0&&!found){
        rnd=rand()%bch;
        for(int i=0;i<nH;i++){
          if(!eqHero(consideredHeros[2][rnd]->hero.name,aHeros[i].name)){
            aHeros[i]=consideredHeros[2][rnd];
            found=true;
            aH++;
            for(int j=0;j<5;j++){
                cV[j]+=consideredHeros[2][rnd]->hero.values[j];
            }
          }
        }
        if(!found){tt--;}
      }
    }
    found=false;
    if(aH<nH&&rch>0){//I have room for and will try find a R hero
      int tt=rch;//only try as much as unique heros i have
      while(tt>0&&!found){
        rnd=rand()%rch;
        for(int i=0;i<nH;i++){
          if(!eqHero(consideredHeros[3][rnd]->hero.name,aHeros[i].name)){
            aHeros[i]=consideredHeros[3][rnd];
            found=true;
            aH++;
            for(int j=0;j<5;j++){
                cV[j]+=consideredHeros[3][rnd]->hero.values[j];
            }
          }
        }
        if(!found){tt--;}
      }
    }
    found=false;
    if(aH<nH&&gch>0){//I have room for and will try find a G hero
      int tt=gch;//only try as much as unique heros i have
      while(tt>0&&!found){
        rnd=rand()%gch;
        for(int i=0;i<nH;i++){
          if(!eqHero(consideredHeros[4][rnd]->hero.name,aHeros[i].name)){
            aHeros[i]=consideredHeros[4][rnd];
            found=true;
            aH++;
            for(int j=0;j<5;j++){
                cV[j]+=consideredHeros[4][rnd]->hero.values[j];
            }
          }
        }
        if(!found){tt--;}
      }
    }

    //fill in empty spots
    while(ah<nH){
      rnd=rand()%totHeros;
      for(int i=0;i<nH;i++){
        if(!eqHero(Heros[rnd].name,aHeros[i].name)){
          aHeros[i]=Heros[rnd];
          aH++;
          for(int j=0;j<5;j++){
              cV[j]+=Heros[rnd]->hero.values[j];
          }
        }
      }
    }

    //check results
    bool success=true;
    for(int i=0;i<5;i++){
      if(cV[i]<cM[i]){success=false;}
    }
    if(success){
      for(int i=0;i<nH;i++){
        fprintf(stdout,"%s\n",aHeros[i].name)
      }
    }
    t--;
  }
  fprintf(stdout,"Failed to generate heros in %d tries.\n",t);
  return;
}

int main(){
  //important vars
  FILE* data==NULL;

  //load library
  fprintf(stdout,"Command \"exit\" quits the program.\n")
  char* dataname=NULL;
  while(dataname==NULL){
    fprintf(stdout,"Enter the file name of the database to use.\n")
    dataname=getLine();
    if(dataname=="exit"){return 1;}
    data=fopen(dataname);
    if(data==NULL){
      fprintf(stdout, "File \""+dataname+"\" couldn't be opened!\n");
      free(dataname);
      dataname=NULL;
    }
    try{
      char* buff=(char *)malloc(sizeof(char)*maxBuff);
      char c;
      int i;
      //read int;
      while((c=fgetc(data))!='\n'){
        buff[i]=c;
        i++;
      }
      totHeros=std::stoi(buff);
      Heros=(Hero *)malloc(sizeof(Hero)*totHeros);
      //read Heros
      for(int j=0;j<totHeros;j++){
        memset(buff,0,maxBuff);
        i=0;
        //buff line
        while((c=fgetc(data))!='\n'){
          buff[i]=c;
          i++;
        }
        int s,e;
        s=e=0;
        //get name
        while(buff[e]!=','){e++;}
        Heros[j]->name=(char *)malloc(sizeof(char)*e);
        strncpy(Heros[j]->name,buff,e)
        s=e;
        char* num=(char *)malloc(sizeof(char)*10);
        //get values
        for(int col=0;col<5;col++){
          while(buff[e]!=','&&buff[e]!='.'){
            e++;
            if(e>10){throw "Number too long.\n";}
          }
          strncpy(num,buff+s,e);
          Heros[j]->values[col]=stoi(num);
          memset(num,0,10);
        }
        free(num);
      }
      free(buff);
    }
    catch(char s){
      fprintf(stdout,"File \""+dataname+"\" is corrupted! Error"+s+"\n");
      free(dataname);
      dataname=NULL;
    }
  }
  free(dataname);

  //make sub arrays
  for(int i=0;i<totHeros;i++){
    if(Heros[i]->values[0]>0){
      //y
      YCHeros++;
    }
    if(Heros[i]->values[1]>0){
      //u
      UCHeros++;
    }
    if(Heros[i]->values[2]>0){
      //b
      BCHeros++;
    }
    if(Heros[i]->values[3]>0){
      //r
      RCHeros++;
    }
    if(Heros[i]->values[4]>0){
      //g
      GCHeros++;
    }
  }
  CHeros=(CHero **)malloc(sizeof(CHero *)*(YCHeros+UCHeros+BCHeros+RCHeros+GCHeros));
  CHeros[0]=(CHero *)malloc(sizeof(CHero)*YCHeros);
  CHeros[1]=(CHero *)malloc(sizeof(CHero)*UCHeros);
  CHeros[2]=(CHero *)malloc(sizeof(CHero)*BCHeros);
  CHeros[3]=(CHero *)malloc(sizeof(CHero)*RCHeros);
  CHeros[4]=(CHero *)malloc(sizeof(CHero)*GCHeros);
  for(int i=0;i<totHeros;i++){
    if(Heros[i]->values[0]>0){
      //y
      CHeros[0][i]->hero=Heros[i];
    }
    if(Heros[i]->values[1]>0){
      //u
      CHeros[1][i]->hero=Heros[i];
    }
    if(Heros[i]->values[2]>0){
      //b
      CHeros[2][i]->hero=Heros[i];
    }
    if(Heros[i]->values[3]>0){
      //r
      CHeros[3][i]->hero=Heros[i];
    }
    if(Heros[i]->values[4]>0){
      //g
      CHeros[4][i]->hero=Heros[i];
    }
  }

  //wait for commands
  char** comm;
  comm=getLine();
  while(strcomp(comm[0],'exit')!=0){

    switch (comm[0]) {
      case 'Heros':
        makeHeros(comm);
      break;
    }

    for(int i=0;i<maxArg;i++){free(comm[i]);}
    free(comm);
    comm=getLine();
  }
    //exit
    //generate list of n heros with min Y,U,B,R,G values of a,b,c,d,e

  //execute commands
  free(comm);
  free(data);
  for(int i=0;i<YCHeros;i++){freeCHero(CHeros[0][i]);}
  for(int i=0;i<UCHeros;i++){freeCHero(UHeros[1][i]);}
  for(int i=0;i<BCHeros;i++){freeCHero(BHeros[2][i]);}
  for(int i=0;i<RCHeros;i++){freeCHero(RHeros[3][i]);}
  for(int i=0;i<GCHeros;i++){freeCHero(GHeros[4][i]);}
  for(int i=0;i<totHeros;i++){freeHero(Heros[i]);}
  free(CHeros[0]);
  free(CHeros[1]);
  free(CHeros[2]);
  free(CHeros[3]);
  free(CHeros[4]);
  free(CHeros);
  free(Heros);
}
