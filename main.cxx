#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ctime>
#include "terminalReader.cxx"

const char* mkHerror1="Format Error: makeHero n (-Y a)(-y a)(-U b)-u b)(-B c)(-b c)(-R d)(-r d)(-G e)(-g e)\n";
const char* ucError="Error: Unknown Command\n";
const char* uError="Unknown Error\n";
int totHeros,YCHeros,UCHeros,BCHeros,RCHeros,GCHeros;

typedef struct Hero_Entry{
  char* name;
  int values[5];
} Hero;

void pHero(Hero h){
  fprintf(stdout, "%s\nY=%d,U=%d,B=%d,R=%d,G=%d\n", h.name,h.values[0],h.values[1],h.values[2],h.values[3],h.values[4]);
}

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

bool eqHero(Hero a, Hero b){
  bool r=strcmp(a.name,b.name)==0;
  return r;
}

void makeHeros(char** comm){
  //fprintf(stdout,"checking args\n");
  //check arg count
  for(int i=1;i<7;i++){
    if(comm[i]==NULL){
      fprintf(stdout, "Error: Missing Arg %d\nSyntax is \"Heros numHeros minY minU minB minR minG\" where each value is an integer.\n",i);
      return;
    }
  }
  if(comm[7]!=NULL){
    fprintf(stdout,"Error: Too many Args\n");
    return;
  }

  //fprintf(stdout,"loading nH\n");
  //load the amount of heros to make the deck with
  int nH = atoi(comm[1]);
  if(nH<1||nH>totHeros){//reject if 0 or less or exceeds the total amount of heroes
    fprintf(stdout,"Error: %d out of %d Heros is an impossible amount of Heros to add to the deck\n",nH,totHeros);
    return;
  }
  //fprintf(stdout,"nH=%d\n",nH);

  //fprintf(stdout,"loading cM\n");
  //load the min colors
  int cM[5];
  for(int i=0;i<5;i++){
    cM[i]=atoi(comm[i+2]);
    if(cM[i]<0){//regect if negative
      fprintf(stdout,"Error: %d is an impossible minimum color value.\n",cM[i]);
      return;
    }
  }

  //fprintf(stdout,"finding heros\n");
  int eM=3;
  //find the list of heroes with color value >= min value - eM
  CHero **consideredHeros=(CHero**)malloc(sizeof(CHero*)*5);
  int ych,uch,bch,rch,gch; //amount of heroes considered in each color
  ych=uch=bch=rch=gch=0;
  for(int i=0;i<YCHeros;i++){
    if(CHeros[0][i].hero->values[0]>cM[0]-eM){ych++;}//count up eligble Y heros
  }
  for(int i=0;i<UCHeros;i++){
    if(CHeros[1][i].hero->values[1]>cM[1]-eM){uch++;}//count up eligble U heros
  }
  for(int i=0;i<BCHeros;i++){
    if(CHeros[2][i].hero->values[2]>cM[2]-eM){bch++;}//count up eligble B heros
  }
  for(int i=0;i<RCHeros;i++){
    if(CHeros[3][i].hero->values[3]>cM[3]-eM){rch++;}//count up eligble R heros
  }
  for(int i=0;i<GCHeros;i++){
    if(CHeros[4][i].hero->values[4]>cM[4]-eM){gch++;}//count up eligble G heros
  }
  //fprintf(stdout,"ych=%d,uch=%d,bch=%d,rch=%d,gch=%d\n",ych,uch,bch,rch,gch);
  int tempc=0;
  consideredHeros[0]=(CHero*)malloc(sizeof(CHero)*ych);
  for(int i=0;i<YCHeros;i++){
    if(CHeros[0][i].hero->values[0]>cM[0]-eM&&tempc<ych){//collect Y heros
      consideredHeros[0][tempc]=CHeros[0][i];
      tempc++;
    }
  }
  tempc=0;
  consideredHeros[1]=(CHero*)malloc(sizeof(CHero)*uch);
  for(int i=0;i<UCHeros;i++){
    if(CHeros[1][i].hero->values[1]>cM[1]-eM&&tempc<uch){//collect U heros
      consideredHeros[1][tempc]=CHeros[1][i];
      tempc++;
    }
  }
  tempc=0;
  consideredHeros[2]=(CHero*)malloc(sizeof(CHero)*bch);
  for(int i=0;i<BCHeros;i++){
    if(CHeros[2][i].hero->values[2]>cM[2]-eM&&tempc<bch){//collect B heros
      consideredHeros[2][tempc]=CHeros[2][i];
      tempc++;
    }
  }
  tempc=0;
  consideredHeros[3]=(CHero*)malloc(sizeof(CHero)*rch);
  for(int i=0;i<RCHeros;i++){
    if(CHeros[3][i].hero->values[3]>cM[3]-eM&&tempc<rch){//collect R heros
      consideredHeros[3][tempc]=CHeros[3][i];
      tempc++;
    }
  }
  tempc=0;
  consideredHeros[4]=(CHero*)malloc(sizeof(CHero)*gch);
  for(int i=0;i<GCHeros;i++){
    if(CHeros[4][i].hero->values[4]>cM[4]-eM&&tempc<gch){//collect G heros
      consideredHeros[4][tempc]=CHeros[4][i];
      tempc++;
    }
  }

  //fprintf(stdout,"selecting heros\n");
  //select heros
  int t=1;
  double timeout=10.0;
  double rep=1.0;
  time_t ts=time(0);
  time_t tn=time(0);
  double dur=difftime(tn,ts);
  while(dur<timeout){
    int aH=0;//accepted heros
    int cV[5];//current total values
    cV[0]=cV[1]=cV[2]=cV[3]=cV[4]=0;
    Hero *aHeros=(Hero*)malloc(sizeof(Hero)*nH);//list of accepted heros to prevent repeats
    int rnd;//random Number
    bool ne=true;//did i find a new hero yet or no
    bool placed=false;//did i place it yet
    //fprintf(stdout,"first picking a Y\n");
    if(aH<nH&&ych>0){//I have room for and will try find a Y hero
      int tt=ych;//only try as much as unique heros i have
      while(tt>0&&!placed){
        rnd=rand()%ych;
        //fprintf(stdout, "rnd=%d\n", rnd);
        for(int i=0;i<nH;i++){
          if(aHeros[i].name+NULL&&eqHero(*consideredHeros[0][rnd].hero,aHeros[i])){
            ne=false;
          }
        }
        if(ne){
          for(int i=0;i<nH;i++){
            if(!placed&&aHeros[i].name==NULL){
              aHeros[i]=*consideredHeros[0][rnd].hero;
              placed=true;
              aH++;
              for(int j=0;j<5;j++){
                  cV[j]+=consideredHeros[0][rnd].hero->values[j];
              }
                //fprintf(stdout, "added ");
                //pHero(aHeros[i]);
                //fprintf(stdout,"aH=%d\n",aH);
            }
          }
        }
        if(!placed){tt--;}
      }
    }
    //fprintf(stdout,"first picking a U\n");
    placed=false;
    ne=true;
    if(aH<nH&&uch>0){//I have room for and will try find a U hero
      int tt=uch;//only try as much as unique heros i have
      while(tt>0&&!placed){
        rnd=rand()%uch;
        //fprintf(stdout, "rnd=%d\n", rnd);
        for(int i=0;i<nH;i++){
          if(aHeros[i].name+NULL&&eqHero(*consideredHeros[1][rnd].hero,aHeros[i])){
            ne=false;
          }
        }
        if(ne){
          for(int i=0;i<nH;i++){
            if(!placed&&aHeros[i].name==NULL){
              aHeros[i]=*consideredHeros[1][rnd].hero;
              placed=true;
              aH++;
              for(int j=0;j<5;j++){
                  cV[j]+=consideredHeros[1][rnd].hero->values[j];
              }
                //fprintf(stdout, "added ");
                //pHero(aHeros[i]);
                //fprintf(stdout,"aH=%d\n",aH);
            }
          }
        }
        if(!placed){tt--;}
      }
    }
    //fprintf(stdout,"first picking a B\n");
    placed=false;
    ne=true;
    if(aH<nH&&bch>0){//I have room for and will try find a B hero
      int tt=bch;//only try as much as unique heros i have
      while(tt>0&&!placed){
        rnd=rand()%bch;
        //fprintf(stdout, "rnd=%d\n", rnd);
        for(int i=0;i<nH;i++){
          if(aHeros[i].name+NULL&&eqHero(*consideredHeros[2][rnd].hero,aHeros[i])){
            ne=false;
          }
        }
        if(ne){
          for(int i=0;i<nH;i++){
            if(!placed&&aHeros[i].name==NULL){
              aHeros[i]=*consideredHeros[2][rnd].hero;
              placed=true;
              aH++;
              for(int j=0;j<5;j++){
                  cV[j]+=consideredHeros[2][rnd].hero->values[j];
              }
                //fprintf(stdout, "added ");
                //pHero(aHeros[i]);
                //fprintf(stdout,"aH=%d\n",aH);
            }
          }
        }
        if(!placed){tt--;}
      }
    }
    //fprintf(stdout,"first picking a R\n");
    placed=false;
    ne=true;
    if(aH<nH&&rch>0){//I have room for and will try find a R hero
      int tt=rch;//only try as much as unique heros i have
      while(tt>0&&!placed){
        rnd=rand()%rch;
        //fprintf(stdout, "rnd=%d\n", rnd);
        for(int i=0;i<nH;i++){
          if(aHeros[i].name+NULL&&eqHero(*consideredHeros[3][rnd].hero,aHeros[i])){
            ne=false;
          }
        }
        if(ne){
          for(int i=0;i<nH;i++){
            if(!placed&&aHeros[i].name==NULL){
              aHeros[i]=*consideredHeros[3][rnd].hero;
              placed=true;
              aH++;
              for(int j=0;j<5;j++){
                  cV[j]+=consideredHeros[3][rnd].hero->values[j];
              }
                //fprintf(stdout, "added ");
                //pHero(aHeros[i]);
                //fprintf(stdout,"aH=%d\n",aH);
            }
          }
        }
        if(!placed){tt--;}
      }
    }
    //fprintf(stdout,"first picking a G\n");
    placed=false;
    ne=true;
    if(aH<nH&&gch>0){//I have room for and will try find a G hero
      int tt=gch;//only try as much as unique heros i have
      while(tt>0&&!placed){
        rnd=rand()%gch;
        //fprintf(stdout, "rnd=%d\n", rnd);
        for(int i=0;i<nH;i++){
          if(aHeros[i].name+NULL&&eqHero(*consideredHeros[4][rnd].hero,aHeros[i])){
            ne=false;
          }
        }
        if(ne){
          for(int i=0;i<nH;i++){
            if(!placed&&aHeros[i].name==NULL){
              aHeros[i]=*consideredHeros[4][rnd].hero;
              placed=true;
              aH++;
              for(int j=0;j<5;j++){
                  cV[j]+=consideredHeros[4][rnd].hero->values[j];
              }
                //fprintf(stdout, "added ");
                //pHero(aHeros[i]);
                //fprintf(stdout,"aH=%d\n",aH);
            }
          }
        }
        if(!placed){tt--;}
      }
    }
    //fprintf(stdout,"filling rest\n");
    //fill in empty spots
    while(aH<nH){
      rnd=rand()%totHeros;
      //fprintf(stdout, "rnd=%d\n", rnd);
      placed=false;
      ne=true;
      for(int i=0;i<nH;i++){
        if(aHeros[i].name+NULL&&eqHero(Heros[rnd],aHeros[i])){
          ne=false;
        }
      }
      if(ne){
        for(int i=0;i<nH;i++){
          if(!placed&&aHeros[i].name==NULL){
            aHeros[i]=Heros[rnd];
            placed=true;
            aH++;
            for(int j=0;j<5;j++){
                cV[j]+=Heros[rnd].values[j];
            }
              //fprintf(stdout, "added ");
              //pHero(aHeros[i]);
              //fprintf(stdout,"aH=%d\n",aH);
          }
        }
      }
    }

    //fprintf(stdout,"checking results\n");
    //check results
    bool success=true;
    for(int i=0;i<5;i++){
      if(cV[i]<cM[i]){success=false;}
    }
    if(success){
      fprintf(stdout,"The team is:\n");
      for(int i=0;i<nH;i++){
        fprintf(stdout,"%s\n",aHeros[i].name);
      }
      int tot=nH*14;
      fprintf(stdout,"With Y count = %d/%d, U count = %d/%d, B count %d/%d, R count = %d/%d, G count = %d/%d.\n",cV[0],tot,cV[1],tot,cV[2],tot,cV[3],tot,cV[4],tot);
      fprintf(stdout,"With Y ratio = %f, U ratio = %f, B ratio %f, R ratio = %f, G ratio = %f.\n",(float)cV[0]/(float)tot,(float)cV[1]/(float)tot,(float)cV[2]/(float)tot,(float)cV[3]/(float)tot,(float)cV[4]/(float)tot);
      fprintf(stdout,"This took %f seconds to make and %d tries.\n",dur,t);
      for(int i=0;i<5;i++){free(consideredHeros[i]);}
      free(consideredHeros);
      return;
    }
    //fprintf(stdout,"failure\n");
    t++;
    tn=time(0);
    dur=difftime(tn,ts);
    if(rep==dur){//for some reason this if statement prevents termination of function ;A;
      fprintf(stdout,"%f seconds have passed and %d tries were attempted.\n",dur,t);
      rep+=1.0;
    }
    fprintf(stdout,"");// this will still allow termination i don't understand ;A;
  }
  fprintf(stdout,"Failed to generate heros in %f seconds and %d tries.\n",timeout,t);
  for(int i=0;i<5;i++){free(consideredHeros[i]);}
  free(consideredHeros);
  return;
}

int main(){
  //important vars
  FILE *data=NULL;

  //load library
  fprintf(stdout,"Command \"exit\" quits the program.\n");
  char **dataname=NULL;
  while(dataname==NULL){
    fprintf(stdout,"Enter the file name of the database to use.\n");
    dataname=getLine();
    if(strcmp(dataname[0],"exit")==0){free(dataname);return 1;}
    data=fopen(dataname[0],"r");
    if(data==NULL){
      fprintf(stdout, "File \"%s\" couldn't be opened!\n",dataname[0]);
      free(dataname);
      dataname=NULL;
    }
    else{
      char *buff=(char *)malloc(sizeof(char)*maxBuff);
      char c;
      int i=0;
      //read int;
      c=fgetc(data);
      while(c!='\n'&&c!='\r'&&i<maxBuff){
        buff[i]=c;
        i++;
        c=fgetc(data);
      }
      while(c=='\n'||c=='\r'){c=fgetc(data);}
      //fprintf(stdout,"buffed\n");
      totHeros=atoi(buff);
      //fprintf(stdout,"tot=%d\n",totHeros);
      Heros=(Hero *)malloc(sizeof(Hero)*totHeros);
      //read Heros
      for(int j=0;j<totHeros;j++){
        memset(buff,0,maxBuff);
        i=0;
        //buff line
        while(c!='\n'&&c!='\r'&&i<maxBuff){
          buff[i]=c;
          i++;
          c=fgetc(data);
        }
        while(c=='\n'||c=='\r'){c=fgetc(data);}
        //fprintf(stdout,"buffed\n");
        int s,e;
        s=e=0;

        //get name
        while(buff[e]!=','){e++;}
        Heros[j].name=(char *)malloc(sizeof(char)*e);
        strncpy(Heros[j].name,buff,e);
        e++;
        s=e;
        char* num=(char *)malloc(sizeof(char)*10);
        //fprintf(stdout,"name got\n");
        //get values
        for(int col=0;col<5;col++){
          while(buff[e]!=','){
            e++;
          }
          strncpy(num,buff+s,e);
          //fprintf(stdout,"num=%s\n",num);
          e++;
          s=e;
          Heros[j].values[col]=atoi(num);
          memset(num,0,10);
          //fprintf(stdout,"value %d got\n",col);
        }
        //free(num);
        //fprintf(stdout,"got hero %d\n",j);
        //pHero(Heros[j]);
      }
      free(buff);
    }
  }
  free(dataname);

  //make sub arrays
  for(int i=0;i<totHeros;i++){
    if(Heros[i].values[0]>0){
      //y
      YCHeros++;
    }
    if(Heros[i].values[1]>0){
      //u
      UCHeros++;
    }
    if(Heros[i].values[2]>0){
      //b
      BCHeros++;
    }
    if(Heros[i].values[3]>0){
      //r
      RCHeros++;
    }
    if(Heros[i].values[4]>0){
      //g
      GCHeros++;
    }
  }
  //fprintf(stdout,"yc=%d,uc=%d,bc=%d,rc=%d,gc=%d\n",YCHeros,UCHeros,BCHeros,RCHeros,GCHeros);
  CHeros=(CHero **)malloc(sizeof(CHero *)*(YCHeros+UCHeros+BCHeros+RCHeros+GCHeros));
  CHeros[0]=(CHero *)malloc(sizeof(CHero)*YCHeros);
  CHeros[1]=(CHero *)malloc(sizeof(CHero)*UCHeros);
  CHeros[2]=(CHero *)malloc(sizeof(CHero)*BCHeros);
  CHeros[3]=(CHero *)malloc(sizeof(CHero)*RCHeros);
  CHeros[4]=(CHero *)malloc(sizeof(CHero)*GCHeros);
  int yi,ui,bi,ri,gi;
  yi=ui=bi=ri=gi=0;
  for(int i=0;i<totHeros;i++){
    //pHero(Heros[i]);
    if(Heros[i].values[0]>0){
      //y
      CHeros[0][yi].hero=&Heros[i];
      yi++;
    }
    if(Heros[i].values[1]>0){
      //u
      CHeros[1][ui].hero=&Heros[i];
      ui++;
    }
    if(Heros[i].values[2]>0){
      //b
      CHeros[2][bi].hero=&Heros[i];
      bi++;
    }
    if(Heros[i].values[3]>0){
      //r
      CHeros[3][ri].hero=&Heros[i];
      ri++;
    }
    if(Heros[i].values[4]>0){
      //g
      CHeros[4][gi].hero=&Heros[i];
      gi++;
    }
  }

  //wait for commands
  fprintf(stdout,"\"help\" for commands.\nCommand?\n");
  char** comm;
  comm=getLine();
  while(strcmp(comm[0],"exit")!=0){

    if(strcmp(comm[0],"Hero")==0){
        makeHeros(comm);
    }
    if(strcmp(comm[0],"t")==0){
      comm[0]="Hero";
      comm[1]="6";
      comm[2]="17";
      comm[3]="17";
      comm[4]="17";
      comm[5]="17";
      comm[6]="17";
      makeHeros(comm);
    }
    if(strcmp(comm[0],"help")==0){
      fprintf(stdout,"\"exit\" quits the program.\n\"Hero numHeros minY minU minB minR minG\" tries to print out a list of numHeros where the resulting deck has all the min values.\n\"help\" for commands.\n");
    }

    //for(int i=0;i<maxArg;i++){free(comm[i]);}
    if(comm!=NULL){free(comm);}
    fprintf(stdout,"Next command?\n");
    comm=getLine();
  }
    //generate list of n heros with min Y,U,B,R,G values of a,b,c,d,e

  //close
  fprintf(stdout, "closing\n");
  //for(int i=0;i<maxArg;i++){free(comm[i]);}
  free(comm);
  fclose(data);
  //for(int i=0;i<YCHeros;i++){free(CHeros[0][i].hero);}
  //for(int i=0;i<UCHeros;i++){free(CHeros[1][i].hero);}
  //for(int i=0;i<BCHeros;i++){free(CHeros[2][i].hero);}
  //for(int i=0;i<RCHeros;i++){free(CHeros[3][i].hero);}
  //for(int i=0;i<GCHeros;i++){free(CHeros[4][i].hero);}
  for(int i=0;i<totHeros;i++){free(Heros[i].name);}
  //free(CHeros[0]);
  //free(CHeros[1]);
  //free(CHeros[2]);
  //free(CHeros[3]);
  //free(CHeros[4]);
  //free(CHeros);
  free(Heros);
  fprintf(stdout, "closed\n");
  return 1;
}
