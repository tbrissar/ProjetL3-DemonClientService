#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "myassert.h"
#include "fichiers.h"

//créé un fichier et l'ouvre en écriture
int ouvrirCreerFichierEcriture(const char* nomFichier)
{
  int fd=open(nomFichier,O_WRONLY,O_CREAT|O_EXCL|0644);
  myassert(fd!=-1,NULL);
  return(fd);
}

//créé un fichier et l'ouvre en lecture
int ouvrirCreerFichierLecture(const char* nomFichier)
{
  int fd=open(nomFichier,O_RDONLY,O_CREAT|O_EXCL|0644);
  myassert(fd!=-1,NULL);
  return(fd);
}

//ouvre un fichier déjà créé en écriture
int ouvrirFichierEcriture(const char* nomFichier)
{
  int fd=open(nomFichier,O_WRONLY,0);
  myassert(fd!=-1,NULL);
  return(fd);
}

//ouvre un fichier déjà créé en lecture
int ouvrirFichierLecture(const char* nomFichier)
{
  int fd=open(nomFichier,O_RDONLY,0);
  myassert(fd!=-1,NULL);
  return(fd);
}

//ecrit dans un fichier déjà ouvert
void ecrireFichier(const int fd, const void *buffer, unsigned int bufferSize)
{
  long int retWrite=write(fd,&bufferSize,sizeof(unsigned int));
  myassert(retWrite!=-1,NULL);
  retWrite=write(fd,buffer,bufferSize);
  myassert(retWrite!=-1,NULL);
}

//lit dans un fichier déjà ouvert
void lireFichier(const int fd, void *buffer)
{
  unsigned int bufferSize;
  long int retRead=read(fd,&bufferSize,sizeof(unsigned int));
  myassert(retRead!=-1,NULL);
  retRead=read(fd,buffer,bufferSize);
  myassert(retRead!=-1,NULL);
}

//ferme un fichier ouvert
void fermerFichier(const int fd)
{
  int retClose=close(fd);
  myassert(retClose!=-1,NULL);
}
