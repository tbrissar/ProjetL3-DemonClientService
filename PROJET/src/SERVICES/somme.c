#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../MYASSERT/myassert.h"

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
void traitementSomme(const int fd[2])
{
	float somme = 0.0;
	int nbMembres; 	// nbMembres information pour faire somme � plus que 2 nombres
	float n;
    // réception des donnees en provenance du client
	lire(fd[0], &nbMembres);
    // traitement des données et calcul du résultat
	for(int i = 0; i < nbMembres; ++i) {
		lire(fd[0], &n);
		somme+=n;
	}
    // envoi du résultat au client
	ecrire(fd[1], &somme, sizeof(float));
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
    traitementSomme(fd);

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
