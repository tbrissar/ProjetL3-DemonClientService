#ifndef CONFIG_H
#define CONFIG_H

// Un service :
//   - est ouvert ou fermé
//   - lié à un fichier exécutable
// La numérotation des services commence à 1
// Une ligne d'un fichier de configuration de peut dépasser
// 255 caractères (retour chariot compris)

#include <stdbool.h>

// fonction devant être appelée au tout début du programme
// pour configurer les services ouverts ou non.
// le paramètre est le nom du fichier de configuration
void config_init(const char *configFile);

// et fonction à appeler en fin de programme
void config_exit();

// renvoie le nombre de services existants
int config_getNombre();

// indique si un service est ouvert
bool config_isOuvert(int numService);

// renvoie le nom du fichier exécutable lié à un service
// il ne faut pas modifier la chaine retournée
const char * config_getNomExecutable(int numService);

#endif
