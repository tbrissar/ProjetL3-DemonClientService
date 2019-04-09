#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "myassert.h"
#include "lireecrire.h"

#include "utils_services.h"


/*-----------------------------------------------------------------*
 * Utilitaires pour le service
 *-----------------------------------------------------------------*/
// implémentation des méthodes

void ouvertureTubesService(int fd[2], const char *tubeClientVersService, const char *tubeServiceVersClient)
{
  fd[0]=open(tubeClientVersService,O_RDONLY,0);
  myassert(fd[0]!=-1,tubeClientVersService);

  fd[1]=open(tubeServiceVersClient,O_WRONLY,0);
  myassert(fd[1]!=-1,tubeServiceVersClient);
}

void fermetureEtDestructionTubes(int fd[2], const char *tubeClientVersService, const char *tubeServiceVersClient)
{
  int retClose=close(fd[0]);
  myassert(retClose!=-1,"");

  retClose=close(fd[1]);
  myassert(retClose!=-1,"");

  int retRemove = remove(tubeClientVersService);
  myassert(retRemove!=-1,"");

  retRemove = remove(tubeServiceVersClient);
  myassert(retRemove!=-1,"");
}

int receptionMessageFinClient(int fd)
{
  int reponse;

  lire(fd,&reponse);

  return reponse;
}

/*-----------------------------------------------------------------*
 * Utilitaires pour le client
 *-----------------------------------------------------------------*/
// implémentation des méthodes

void ouvertureTubesClientService(int fd[2], char *nomTubeClientVersService, char *nomTubeServiceVersClient)
{
  char *tmp=calloc(strlen(nomTubeClientVersService)+4,sizeof(char));

  tmp=strcpy(tmp,"../");
  myassert(tmp!=NULL,tmp);
  tmp=strcat(tmp,nomTubeClientVersService);
  myassert(tmp!=NULL,tmp);

  fd[0]=open(tmp,O_WRONLY,0);
  myassert(fd[0]!=-1,tmp);

  tmp=strcpy(tmp,"../");
  myassert(tmp!=NULL,tmp);
  tmp=strcat(tmp,nomTubeServiceVersClient);
  myassert(tmp!=NULL,tmp);

  fd[1]=open(tmp,O_RDONLY,0);
  myassert(fd[1]!=-1,tmp);

  free(tmp);
}

void fermetureTubes(int fd[2])
{
  int retClose=close(fd[1]);
  myassert(retClose!=-1,"");

  retClose=close(fd[0]);
  myassert(retClose!=-1,"");
}

void envoiMessageFin(int fd, int message)
{
  ecrire(fd,&message,sizeof(int));
}
