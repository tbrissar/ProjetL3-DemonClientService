#include <unistd.h>

#include "myassert.h"
#include "lireecrire.h"

//ecrit dans un fichier déjà ouvert
void ecrire(const int fd, const void *buffer, unsigned int bufferSize)
{
  long int retWrite=write(fd,&bufferSize,sizeof(unsigned int));
  myassert(retWrite!=-1,"");
  retWrite=write(fd,buffer,bufferSize);
  myassert(retWrite!=-1,"");
}

//lit dans un fichier déjà ouvert
void lire(const int fd, void *buffer)
{
  unsigned int bufferSize;
  long int retRead=read(fd,&bufferSize,sizeof(unsigned int));
  myassert(retRead!=-1,"");
  retRead=read(fd,buffer,bufferSize);
  myassert(retRead!=-1,"");
}
