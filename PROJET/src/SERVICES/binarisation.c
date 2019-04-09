#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "../MYASSERT/myassert.h"

// si vous retenez cette option (cf. fichier ../utils_services.h)
#include "../utils_services.h"

#include "../IMAGES/Image.h"

// structure param�tre thread
typedef struct
{
	int seuil;
	short* pixels;
	int depart;
	int nbPixels;
} ThreadData;

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <tubeIn> <tubeOut>\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}


void *supportFunction(void *arg)
{
	//r�cup�ration et cast de nos donn�es
	ThreadData *data = (ThreadData *) arg;

	//on fait les op�rations de modifications de l'image
	for(int i = data->depart; i <= data->nbPixels+data->depart; i++) {
		data->pixels[i] = (data->pixels[i] < data->seuil) ? 0 : (data->pixels[i] == data->seuil) ? 127 : 255;
	}

    return(NULL);
}

// code spécique au service visé
void traitementBinarisation(const int fd[2])
{
	short* pixels;
	int nbPixels;
	int seuil;
	int nbThread;
	pthread_t* pThreadTab;
	ThreadData* data;

    // réception des donnees en provenance du client
	// allocation des tableaux de pixels (des images non-trait�e et trait�e)
	lire(fd[0], &nbPixels);
	pixels = calloc(nbPixels, sizeof(short));
	myassert(pixels != NULL, "");

	// r�cup�ration seuil
	lire(fd[0], &seuil);

	// allocation des threads
	lire(fd[0], &nbThread);
	data = calloc(nbThread, sizeof(ThreadData));
	myassert(data != NULL,"");
	pThreadTab = calloc(nbThread, sizeof(pthread_t));
	myassert(pThreadTab != NULL, "");

	// r�cup�ration image
	lire(fd[0], pixels);

	//je s�pare l'image en plusieurs morceaux pour les threads
	int reste = (int)(((double)nbPixels / (double)nbThread - nbPixels/nbThread)*nbThread); //nbPixels en trop � traiter par le dernier thread
	nbPixels = nbPixels / nbThread;

	//init data
	for(int i = 0; i < nbThread; i++) {
		data[i].seuil = seuil;
		data[i].pixels = pixels;
		data[i].depart = nbPixels*i; //pour savoir o� d�marre le ieme thread
		data[i].nbPixels = (i == nbThread-1) ? nbPixels+reste : nbPixels; // le dernier thread prend le surplus de pixels s'il y en a
	}

    // traitement des données et calcul du résultat
	// cr�ations des threads
	for (int i = 0; i < nbThread; i++) {
	    int retPthread_create = pthread_create(&pThreadTab[i], NULL, supportFunction, &data[i]);
	    myassert(retPthread_create == 0,"");
	}

	//attente de tous les threads
	for (int i = 0; i < nbThread; i++) {
	    int retPthread_join=pthread_join(pThreadTab[i],NULL);
	    myassert(retPthread_join==0,"");
	}

  // envoi du résultat au client
	ecrire(fd[1], pixels, sizeof(short)*(nbPixels*nbThread+reste));

	// d�sallocation
	free(pixels);
	free(pThreadTab);
	free(data);
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
    traitementBinarisation(fd);

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
