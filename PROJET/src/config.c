#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "fichiers.h"
#include "myassert.h"

#include "config.h"

/* =================================================================
 * structure de données et données permanentes
 */

// Ici les types et les variables permanentes pour
// stocker les services
// données pour un service :
// - le nom de l'exécutable
// - s'il est ouvert
// - son numéro (numérotation commence à 1)

  struct service{
    char *exec;
    bool open;
    int num;
  };

  struct service *serviceList=NULL;
  static int nbServices=-1;

/* =================================================================
 * initialisation : lecture du fichier de configuration
 */

 static void nextLine(int fd)
 {
   char c='a';
   int retRead;

   do{
     retRead=(int)read(fd,&c,sizeof(char));
     myassert(retRead!=-1,"");
   }while(c!='\n' && retRead==1);
 }

 static void getLineWithoutSpaces(int fd, char *line)
 {
   char c;
   int retRead,retSprintf,nbChar=0;

   do{
     retRead=(int)read(fd,&c,sizeof(char));
     myassert(retRead!=-1,"");
     if(c!=' '){
       retSprintf=sprintf(line,"%s%c",line,c);
       myassert(retSprintf>-1,"");
     }
     nbChar++;
     myassert(nbChar<256,"Ligne trop longue");
   }while(c!='\n' && retRead==1);
 }

 static void checkStruct(char *line, struct service *currentservice,int cptNumService)
 {
   myassert(line[0]>='0' && line[0]<='9',"Pas de numéro de service");

   char stringNumService[255]={0};
   char *ptrStringNumService=stringNumService;
   char *ptrLine=line;
   int lineLen=(int)strlen(line);

   //checking service number
   for(;(*ptrLine!='\n') && (*ptrLine>='0'&& *ptrLine<='9');ptrLine++){
     *ptrStringNumService=*ptrLine;
     ptrStringNumService++;
   }

   int numService=(int)strtol(stringNumService,NULL,10);
   myassert(numService==cptNumService,"Numéro de service incorrect");
   currentservice->num=numService;

   //checking ouvert or ferme
   if(strncmp("ouvert\0",ptrLine,6)==0){
     currentservice->open=true;
     ptrLine+=6;
   }else if(strncmp("ferme\0",ptrLine,5)==0){
     currentservice->open=false;
     ptrLine+=5;
   }else myassert(false,"etat incorrect");

   //getting path, no checking on it
   char stringPath[255]={0};
   for(int i=0;ptrLine-line<lineLen-1;ptrLine++,i++){
     stringPath[i]=*ptrLine;
   }

   int pathLen=(int)strlen(stringPath);
   currentservice->exec=calloc((long unsigned int)(pathLen+1),sizeof(char));
   myassert(currentservice->exec!=NULL,"");
   for(int i=0;i<pathLen;i++){
     currentservice->exec[i]=stringPath[i];
   }
   //stringPath[i]='\0';

 }

void config_init(const char *configFile)
{
    // TODO
    // lecture du fichier de configuration
    int fd=ouvrirFichierLecture(configFile);

    char firstCharInLine;
    char line[256]={0};
    int retRead=1;
    int retSprintf;
    bool nbServicesSet=false;
    int cptNumService=0;

    while(retRead!=0){
      retRead=(int)read(fd,&firstCharInLine,sizeof(char));
      myassert(retRead!=-1,"");

      switch(firstCharInLine){
        case '#' : nextLine(fd);
        case '\n' : break;
        default :
          retSprintf=sprintf(line,"%c",firstCharInLine);
          myassert(retSprintf>-1,"");
          getLineWithoutSpaces(fd,line);

          if(!nbServicesSet){
            nbServices=(int)strtol(line,NULL,10);
            myassert(nbServices>=1,"Nombre de services incorrect (doit être >=1)");
            serviceList=malloc(sizeof(struct service)*(long unsigned int)nbServices);
            myassert(serviceList!=NULL,"");
            nbServicesSet=true;
          }else{
            cptNumService++;
            myassert(cptNumService<=nbServices,"Liste de services trop longue");
            checkStruct(line,&serviceList[cptNumService-1],cptNumService);
          }
      }
    }
    myassert(cptNumService==nbServices,"Liste de services trop courte");
    fermerFichier(fd);
}


/* =================================================================
 * libération des ressources
 */
void config_exit()
{
    // TODO
    for(int i=0;i<nbServices;i++){
      free(serviceList[i].exec);
    }
    free(serviceList);
    serviceList=NULL;
}


/* =================================================================
 * accesseurs
 */
int config_getNombre()
{
    // TODO
    return(nbServices);
}

bool config_isOuvert(int numService)
{
    // TODO
    return serviceList[numService-1].open;
}

const char * config_getNomExecutable(int numService)
{
    // TODO
    return serviceList[numService-1].exec;
}
