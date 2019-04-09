#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "myassert.h"
#include "lireecrire.h"

#include "utils_demon.h"


/*-----------------------------------------------------------------*
 * Utilitaires pour le demon
 *-----------------------------------------------------------------*/
// implémentation des méthodes

int creationMutex(int proj_id)
{
  int cle=ftok(PATHNAME,proj_id);
  myassert(cle!=-1,"");

  int semid=semget(cle,1,IPC_CREAT|IPC_EXCL|DROITS);
  myassert(semid!=-1,"");

  int retSemctl=semctl(semid,0,SETVAL,1);
  myassert(retSemctl!=-1,"");

  return semid;
}

void destructionMutex(int semid)
{
  int retSemctl=semctl(semid,0,IPC_RMID,0);
  myassert(retSemctl!=-1,"");
}

void creationTube(char *nomTube)
{
  int retMkfifo = mkfifo(nomTube, O_CREAT|O_EXCL|DROITS);
  myassert(retMkfifo != -1,"");
}

void destructionTubes()
{
  int retRemove = remove(NOMTUBECLIENTVERSDEMON);
  myassert(retRemove!=-1,"");

  retRemove = remove(NOMTUBEDEMONVERSCLIENT);
  myassert(retRemove!=-1,"");
}

void ouvertureTubesDemon(int fd[2])
{
  fd[0]=open(NOMTUBECLIENTVERSDEMON,O_RDONLY,0);
  myassert(fd[0]!=-1,"");

  fd[1]=open(NOMTUBEDEMONVERSCLIENT,O_WRONLY,0);
  myassert(fd[1]!=-1,"");
}

void fermetureTube(int fd)
{
    int retClose=close(fd);
    myassert(retClose!=-1,"");
}

void envoiReponse(int fd, int reponse)
{
  ecrire(fd,&reponse,sizeof(int));
}

void envoiNomsTubes(int fd, const char *nomTubeClientVersService, const char *nomTubeServiceVersClient)
{
  ecrire(fd,nomTubeClientVersService,(unsigned int)(sizeof(char)*strlen(nomTubeClientVersService)));

  ecrire(fd,nomTubeServiceVersClient,(unsigned int)(sizeof(char)*strlen(nomTubeServiceVersClient)));
}

/*-----------------------------------------------------------------*
 * Utilitaires pour le client
 *-----------------------------------------------------------------*/
// implémentation des méthodes

int connexionMutex(int proj_id)
{
  int cle = ftok(CLIENTPATHNAME,proj_id);
  myassert(cle!=-1,"");
  int semid=semget(cle,1,0);
  myassert(semid!=-1,"");

  return semid;
}

void lockMutex(int semid)
{
  struct sembuf op = {0,-1,0};
  int retSemop=semop(semid,&op,1);
  myassert(retSemop!=-1,"");
}

void unlockMutex(int semid)
{
  struct sembuf op = {0,1,0};
  int retSemop=semop(semid,&op,1);
  myassert(retSemop!=-1,"");
}

void ouvertureTubesClientDemon(int fd[2])
{
  fd[0]=open(CLIENTNOMTUBECLIENTVERSDEMON,O_WRONLY,0);
  myassert(fd[0]!=-1,"");

  fd[1]=open(CLIENTNOMTUBEDEMONVERSCLIENT,O_RDONLY,0);
  myassert(fd[1]!=-1,"");
}

void receptionTubes(int fd, char *nomTubeClientVersService, char *nomTubeServiceVersClient)
{
  lire(fd,nomTubeClientVersService);
  lire(fd,nomTubeServiceVersClient);
}
