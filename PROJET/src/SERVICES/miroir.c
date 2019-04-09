#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "myassert.h"

// si vous retenez cette option (cf. fichier ../utils_services.h)
#include "../utils_services.h"

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <tubeIn> <tubeOut>\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

// code spécique au service visé
void traitementMiroir(const int fd[2])
{
  int taille;
  lire(fd[0],&taille);

	// j'alloue les chaines
	char* chaine=calloc(taille+1,sizeof(char));
	char* eniahc=calloc(taille+1,sizeof(char));
  // réception des donnees en provenance du client
	lire(fd[0],chaine);

  // traitement des données et calcul du résultat
	// j'inverse la chaine dans eniahc
	for(int i = taille, j = 0; i >= 0; i--, j++) {
		eniahc[j] = chaine[i-1];
	}
  // envoi du résultat au client
	ecrire(fd[1], eniahc, sizeof(char)*taille);
	// je lib�re les chaines allou�es
	free(chaine);
	free(eniahc);
}

int main(int argc, char * argv[])
{

    if (argc != 3)
        usage(argv[0], NULL);

    // file descriptors des deux tubes
    //   argv[1] == clientToService == fd[0]
    //   argv[2] == serviceToClient == fd[1]

    int fd[2];

    // connexions aux tubes déjà existants
    // todo
    //    ouvrir les tubes
    //    argv[1] == c2s
    //    argv[2] == s2c
    ouvertureTubesService(fd, argv[1], argv[2]);

    // partie propre au service
    traitementMiroir(fd);

    // attente de la fin du client (i.e. fin d'utilisation des tubes)
    // todo
    while(receptionMessageFinClient(fd[0]) != TRANSACTIONTERMINEE) {
    	sleep(1);
    }

    // fermeture et destruction des tubes
    // todo
    fermetureEtDestructionTubes(fd, argv[1], argv[2]);

    return EXIT_SUCCESS;
}
