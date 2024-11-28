#ifndef FOR_H
#define FOR_H

#ifdef DEBUG
#define DEBUG_LOG(fmt, args...) dprintf(STDOUT_FILENO, fmt, ##args)
#else
#define DEBUG_LOG(fmt, args...)
#endif


// ***=== Inclusions ===***
// #include <linux/limits.h> // on va le define nous même car sur macos ça marche pas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "debug.h"

#include "commands.h"

#define PATH_MAX 4096

typedef struct comFor
{                  // structure pour les commandes structurées type for ... in .. {commandes}
    char *command; // renseigne le nom de la commande
    char *dir;     // renseigne le répertoire courant
    char *path;    // renseigne le path du fichier dont l'on désire l'exécution
    char *options; // donne les options de boucle
    char *ligne;   // donne toute la ligne de commande de l'input

    // C POUR LES OPTIONS PLUSTARDS
    int fic_caches;
    int recursive;
    char *extention;
    int type;
    int max_parallel;
} comFor;

comFor *initialiseCommandFor();      // initialise la structure command
comFor *fillCommandFor(command *cmd); // remplit les champs selon les infos
void clearCommandFor(comFor *com);   // free tous les champs de la strcut type comFor (commande for)
int parcoursFor(comFor *cm);         // fait le parcours du répertoire courant et y applique la commande à chaque fichier trouvé
char *remplacer_variable(char *str, char *var, char *valeur);
#endif