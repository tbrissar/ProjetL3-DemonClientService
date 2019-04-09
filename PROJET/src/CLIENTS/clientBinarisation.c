#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "myassert.h"

// si vous retenez cette option (cf. fichier ../utils_demon.h)
#include "../utils_demon.h"
// si vous retenez cette option (cf. fichier ../utils_services.h)
#include "../utils_services.h"

#include "../IMAGES/Image.h"

#define TAILLE 100
#define NBMEMBRES 3
// doit être en accord avec le fichier de configuration
static const int NUM_SERVICE = 4;

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <paramètres propres au service>\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

// code spécique au client visé
void traitement(const int fd[2], const char *nomImage, int nbThreads, int seuil)
{
  printf("Je commence le traitement\n");
  Image img=image_lire(nomImage);
  int nbPixels=img.nbColonnes*img.nbLignes;

  ecrire(fd[0],&nbPixels,sizeof(int));
  ecrire(fd[0],&seuil,sizeof(int));
  ecrire(fd[0],&nbThreads,sizeof(int));
  ecrire(fd[0],img.pixels,sizeof(short)*nbPixels);

  lire(fd[1],img.pixels);

  char *newName=calloc(strlen(nomImage)+4,sizeof(char));
  myassert(newName!=NULL,"");
  unsigned int retSprintf=sprintf(newName,"%s%s",nomImage,"bis");
  myassert(retSprintf==strlen(nomImage)+3,"");

  printf("%s\n",newName);

  image_ecrire255(&img,newName);
  image_detruire(&img);

  printf("J'ai fini le traitement\n");

  free(newName);
}

int main(int argc, char * argv[])
{
    if (argc != 4)
        usage(argv[0], NULL);

    printf("Init\n");

    int numService=NUM_SERVICE;
    // files descriptors des 2 tubes vers le démon
    //[0]=demonversclient
    //[1]=clientversdemon
    int fdDemon[2];
    // files descriptors des 2 tubes vers le service
    int fdService[2];

    // connexion au mutex
    int semidMutex1 = connexionMutex(PROJ_IDMUTEX1);
    // connexion au sémaphore
    int semidMutex2 = connexionMutex(PROJ_IDMUTEX2);

    // entrée en SC (Section Critique)
    lockMutex(semidMutex1);
    lockMutex(semidMutex2);
    printf("Entré en SC\n");

    // ouverture tubes avec le démon
    ouvertureTubesClientDemon(fdDemon);

    // envoi numéro service au démon
    ecrire(fdDemon[0],&numService,sizeof(int));
    // réception (et analyse) de la réponse du démon
    int reponse;
    lire(fdDemon[1],&reponse);
    //       . le service demandé peut être ok, ferme ou inconnu

    char nomTubeClientVersService[TAILLE]={0};
    char nomTubeServiceVersClient[TAILLE]={0};

    switch(reponse){

      case OK:
      printf("Service OK\n");
        // si service ouvert
        // - récupération noms tubes avec le service
        // - ouverture de ces tubes
        receptionTubes(fdDemon[1],nomTubeClientVersService,nomTubeServiceVersClient);
        printf("Reception tubes ok\n");
        ouvertureTubesClientService(fdService,nomTubeClientVersService,nomTubeServiceVersClient);
        printf("Ouverture tubes ok\n");
        break;

      case FERME:
          printf("Service non disponible\n");
          break;

      case INCONNU:
          printf("Service inconnue\n");
          break;

      default:
          printf("Reponse inconnue\n");
          break;
    }

    printf("J'en ai fini avec le demon\n");

    // fermeture tubes client vers démon
    fermetureTube(fdDemon[0]);
    fermetureTube(fdDemon[1]);

    // débloque le démon pour qu'il se mette en attente de quelqu'un d'autre
    unlockMutex(semidMutex2);
    // sortie SC (pour que d'autre clients puissent communiquer avec le démon
    unlockMutex(semidMutex1);

    // déconnexion du sémaphore et du mutex

    // Maintenant que la communication avec le démon est finie,
    // on communique avec le service

    // si service ouvert
    // - appel fonction traitement ci-dessus
    // - envoi message de fin au service
    // - fermenure des tubes entres le client et le service
    if(reponse==OK){
      traitement(fdService,(const char*)argv[1],strtol(argv[2],NULL,10),strtol(argv[3],NULL,10));
      envoiMessageFin(fdService[0],TRANSACTIONTERMINEE);
    }

    return EXIT_SUCCESS;
}
