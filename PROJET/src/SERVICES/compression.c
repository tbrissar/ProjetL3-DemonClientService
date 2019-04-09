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
void traitementCompression(const int fd[2])
{
  int taille;
  lire(fd[0],&taille);

	// j'alloue les chaines
	char *chaine = calloc(taille+1,sizeof(char));
  myassert(chaine!=NULL,"");
	char *chn = calloc(taille+1,sizeof(char));
  myassert(chn!=NULL,"");

  // réception des donnees en provenance du client
  lire(fd[0], chaine);
  // traitement des données et calcul du résultat
	// j'inverse la chaine dans chn
	int j = 0;
	for(int i = 0; i < taille; ++i) {
		if(chaine[i] != 'a' && chaine[i] != 'e' && chaine[i] != 'i' && chaine[i] != 'o' && chaine[i] != 'u' && chaine[i] != 'y') {
			chn[j] = chaine[i];
			j++;
		}
	}

  // envoi du résultat au client
	ecrire(fd[1], chn, sizeof(char)*strlen(chn));
	// je lib�re les chaines allou�es
	free(chaine);
	free(chn);
}

int main(int argc, char * argv[])
{

    if (argc != 3)
        usage(argv[0], NULL);

    // file descriptors des deux tubes
    //   argv[1] == clientToService == fd[0]
    //   argv[2] == serviceToClient == fd[1]

    printf("SERVICE COMPRESSION\n");

    int fd[2];

    // connexions aux tubes déjà existants
    // todo
    //    ouvrir les tubes
    //    argv[1] == c2s
    //    argv[2] == s2c
    ouvertureTubesService(fd, argv[1], argv[2]);
    printf("Service compression ouverture terminée\n");

    // partie propre au service
    traitementCompression(fd);
    printf("Traitement terminé\n");

    // attente de la fin du client (i.e. fin d'utilisation des tubes)
    // todo
    while(receptionMessageFinClient(fd[0]) != TRANSACTIONTERMINEE) {
        sleep(1);
    }

    // fermeture et destruction des tubes
    // todo
    printf("Je detruit les tubes\n");
    fermetureEtDestructionTubes(fd, argv[1], argv[2]);

    return EXIT_SUCCESS;
}
