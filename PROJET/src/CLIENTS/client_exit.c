#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "myassert.h"
#include "lireecrire.h"
// si vous retenez cette option (cf. fichier ../utils_demon.h)
#include "utils_demon.h"

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char * argv[])
{
    if (argc != 1)
        usage(argv[0], NULL);

    int numService=ORDREDEFIN;
    // c'est une version simplifiée d'un client
    // - on discute normalement avec le démon
    // - c'est un code de fin (au lieu d'un numéro de service) qu'on envoie
    // - donc pas de tubes avec un service à gérer

    // files descriptors des 2 tubes vers le démon
    //[0]=demonversclient
    //[1]=clientversdemon
    int fdDemon[2];

    // connexion au mutex
    int semidMutex1 = connexionMutex(PROJ_IDMUTEX1);
    // connexion au sémaphore
    int semidMutex2 = connexionMutex(PROJ_IDMUTEX2);

    lockMutex(semidMutex1);
    lockMutex(semidMutex2);

    ouvertureTubesClientDemon(fdDemon);

    // envoi numéro service au démon
    ecrire(fdDemon[0],&numService,sizeof(int));

    // fermeture tubes client vers démon
    fermetureTube(fdDemon[0]);
    fermetureTube(fdDemon[1]);

    unlockMutex(semidMutex1);
    // débloque le démon pour qu'il se mette en attente de quelqu'un d'autre
    unlockMutex(semidMutex2);
    // sortie SC (pour que d'autre clients puissent communiquer avec le démon


    return EXIT_SUCCESS;
}
