#ifndef UTILS_SERVICES_H
#define UTILS_SERVICES_H

// Ce n'est qu'une proposition pour organiser le code
// vous êtes libre de :
//   - ne pas vous en servir
//   - de vous en inspirer
//   - de la suivre à la lettre
//
// Ce fichier pourrait contenir les fonctions qui permettent
// à un client et un service de communiquer (pour la partie
// commune à tous les services

#define TRANSACTIONTERMINEE 3

void ecrire(const int fd, const void *buffer, unsigned int bufferSize);
void lire(const int fd, void *buffer);

/*-----------------------------------------------------------------*
 * Utilitaires pour le service
 *-----------------------------------------------------------------*/
// 1. ouverture des deux tubes (rappel : ils existent déjà)
// 2. fermeture et destruction physique des tubes
// 3. réception du message de fin envoyé par le client

void ouvertureTubesService(int fd[2], const char *tubeClientVersService, const char *tubeServiceVersClient);
void fermetureEtDestructionTubes(int fd[2], const char *tubeClientVersService, const char *tubeServiceVersClient);
int receptionMessageFinClient(int fd);

/*-----------------------------------------------------------------*
 * Utilitaires pour le client
 *-----------------------------------------------------------------*/
// 1. ouverture des deux tubes (rappel : ils existent déjà)
// 2. fermeture des tubes (ils sont détruits par le service)
// 3. envoi du message de fin envoyé au service
void ouvertureTubesClientService(int fd[2], char *nomTubeClientVersService, char *nomTubeServiceVersClient);
void fermetureTubes(int fd[2]);
void envoiMessageFin(int fd, int message);

#endif
