#ifndef __Fichiers__
#define __Fichiers__

  int ouvrirCreerFichierEcriture(const char* nomFichier);
  int ouvrirCreerFichierLecture(const char* nomFichier);
  int ouvrirFichierEcriture(const char* nomFichier);
  int ouvrirFichierLecture(const char* nomFichier);
  void lireFichier(const int fd, void *buffer);
  void ecrireFichier(const int fd, const void *buffer, unsigned int tailleBuffer);
  void fermerFichier(const int fd);

#endif
