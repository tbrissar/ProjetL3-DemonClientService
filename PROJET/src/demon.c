#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "lireecrire.h"
#include "myassert.h"
#include "config.h"
#include "utils_demon.h"

#define TAILLE 100

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <fichier config>\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}


static void lanceService(int numService, char *const argv[4], pid_t *pidTab)
{
    // fork
    // le père sort tout de suite de la fonction
    // le fils fait un exec sur l'exécutable du service choisi
    printf("Je lance le service\n");
    pid_t pid = fork();
    myassert(pid != -1,"");
    if(pid == 0){
      int retExec = execv(config_getNomExecutable(numService),argv);
      myassert(retExec != -1,"");
    }
    pidTab[numService-1] = pid;
    printf("Service lancé\n");
}

static void attenteFils(pid_t *pidTab)
{
  for(int i=0;i<config_getNombre();i++){
    printf("Ah que ");
    pid_t p=pidTab[i];
    if(p != 0){
      printf("coucou %d",p);
      pid_t retWaitPid=waitpid(p,NULL,0);
      myassert(retWaitPid != -1,"");
      pidTab[i]=0;
    }
    printf("\n");
  }
}


int main(int argc, char * argv[])
{
    if (argc != 2)
        usage(argv[0], NULL);

    printf("INIT\n");

    // initialisations diverses
    config_init(argv[1]);
    pid_t *pidTab = calloc((long unsigned int)config_getNombre,sizeof(int));
    myassert(pidTab!=NULL,"");
    // créer mutex
    int semidMutex1 = creationMutex(PROJ_IDMUTEX1);
    int semidMutex2 = creationMutex(PROJ_IDMUTEX2);
    // créer sémaphore
    //A QUOI CA SERT
    // créer tubes démon vers client
    creationTube(NOMTUBECLIENTVERSDEMON);
    creationTube(NOMTUBEDEMONVERSCLIENT);

    // boucle d'attente
    while (true)
    {
        printf("J'attend\n");
        // files descritors des 2 tubes vers le démon
        // d2c : démon to client ; c2d : l'inverse
        int tubesFd[2];

        // ouverture des tubes pour attendre un client
        ouvertureTubesDemon(tubesFd);

        // recevoir numéro de service d'un client
        int numService;
        lire(tubesFd[0],&numService);
        // analyse du numéro : existant ? fermé ? ordre de fin ? actif ?
        // envoi de la réponse au client

        if(numService==ORDREDEFIN){
          // si c'est demande d'arrêt
          // - sortir de la boucle
          printf("Je m'arrête\n");
          break;
        }
        else if(numService>config_getNombre() || numService<1)
          envoiReponse(tubesFd[1],INCONNU);
        else if(!config_isOuvert(numService))
          envoiReponse(tubesFd[1],FERME);
        else{
          envoiReponse(tubesFd[1],OK);
          printf("Envoi reponse OK\n");
          // si actif
          // - générer deux noms de tubes uniques pour la liaison client-service
          // - créer les tubes (rappel : c'est le service qui les détruira)
          // - envoyer les noms au client
          // - lancer le service (cf. fonction lanceService ci-dessus)
          //   (rq : le service est géré par un fork, donc la main est
          //         rendu immédiatement au démon)
          char nomTubeClientVersService[TAILLE] = {0};
          char nomTubeServiceVersClient[TAILLE] = {0};
          int retSprintf = sprintf(nomTubeClientVersService,"tubeClientVers%d", numService);
          myassert(retSprintf>=0,"Erreur sprintf");
          retSprintf = sprintf(nomTubeServiceVersClient,"tube%dVersClient", numService);
          myassert(retSprintf>=0,"Erreur sprintf");

          creationTube(nomTubeClientVersService);
          creationTube(nomTubeServiceVersClient);
          printf("Creation tubes service ok\n");

          envoiNomsTubes(tubesFd[1], nomTubeClientVersService, nomTubeServiceVersClient);
          printf("Envoi des tubes ok\n");

          char *const argService[4] = {(char *)config_getNomExecutable(numService), nomTubeClientVersService, nomTubeServiceVersClient, NULL};
          lanceService(numService,argService,pidTab);

        }

        // se déconnecter des tubes démon vers client
        fermetureTube(tubesFd[0]);
        fermetureTube(tubesFd[1]);

        // attendre que le client nous débloque
        lockMutex(semidMutex2);
        unlockMutex(semidMutex2);

        // idéalement il faudrait gérer les fils qui se sont terminés
        printf("Arret des fils terminés\n");
        attenteFils(pidTab);
    }

    lockMutex(semidMutex2);
    unlockMutex(semidMutex2);

    // désinitialisations
    // détruire tubes démon vers client
    destructionTubes();
    // détruire sémaphore
    // détruire mutex
    destructionMutex(semidMutex1);
    destructionMutex(semidMutex2);

    config_exit();

    // attendre la fin des fils restants
    printf("Attente des fils restants\n");
    attenteFils(pidTab);

    free(pidTab);

    return EXIT_SUCCESS;
}
