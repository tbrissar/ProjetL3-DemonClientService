#ifndef UTILS_DEMON_H
#define UTILS_DEMON_H

// Ce n'est qu'une proposition pour organiser le code
// vous êtes libre de :
//   - ne pas vous en servir
//   - de vous en inspirer
//   - de la suivre à la lettre
//
// Ce fichier pourrait contenir les fonctions qui permettent
// à un client et le démon de communiquer (avant de passer la main
// au service)

#define PATHNAME "utils_demon.h"
#define CLIENTPATHNAME "../utils_demon.h"
#define PROJ_IDMUTEX2 1
#define PROJ_IDMUTEX1 2
#define DROITS 0641

#define NOMTUBEDEMONVERSCLIENT "d2c"
#define NOMTUBECLIENTVERSDEMON "cvd"
#define CLIENTNOMTUBEDEMONVERSCLIENT "../d2c"
#define CLIENTNOMTUBECLIENTVERSDEMON "../cvd"

#define FERME 1
#define OK 2
#define INCONNU 3

#define ORDREDEFIN -1

/*-----------------------------------------------------------------*
 * Utilitaires pour le demon
 *-----------------------------------------------------------------*/
// il faut un seul client à la fois qui échange avec le démon
// 1. création mutex
// 2. destruction mutex

int creationMutex();
void destructionMutex(int semid);

// le démon ne peut passer au client suivant que lorsque le client
// courant a terminé sa conversation avec le démon ; on fait
// une précédence
// 3. création sémaphore
// 4. destruction sémaphore
// 5. -1 sur le sémpahore


// 6. création tubes démon vers client
// 7. destruction tubes
// 8. ouverture des tubes
// 9. fermeture des tubes

void creationTube();
void destructionTubes();
void ouvertureTubesDemon(int fd[2]);
void fermetureTube(int fd);

// 10. réception numéro service du client
int receptionNum(int fd);
// 11. envoi de la réponse (i.e. de la validité de la demande)
void envoiReponse(int fd, int reponse);

// 12. envoi des noms des tubes (pour la communication client/service)
void envoiNomsTubes(int fd, const char *nomTubeClientVersService, const char *nomTubeServiceVersClient);

/*-----------------------------------------------------------------*
 * Utilitaires pour le client
 *-----------------------------------------------------------------*/
// 1. connexion au mutex
// 2. déconnexion du mutex
// 3. lock sur le mutex
// 4. unlock sur le mutex
int connexionMutex();
void lockMutex(int semid);
void unlockMutex(int semid);


// 5. connexion sémaphore
// 6. déconnexion sémpahore
// 7. +1 sur le sémaphore


// 8. ouverture des tubes démon vers client
// 9. fermeture des tubes
void ouvertureTubesClientDemon(int fd[2]);

// 10. envoi numéro du service au démon
// 11. réception de la réponse
void envoiNum(int fd, int num);
int receptionReponse(int fd);

// 12. réception des noms des tubes (pour la communication client/service)
void receptionTubes(int fd, char *nomTubeClientVersService, char *nomTubeServiceVersClient);

#endif
