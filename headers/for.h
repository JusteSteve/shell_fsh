#ifndef FOR_H
#define FOR_H

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

#include "../headers/commands.h"
#include "../headers/fsh.h"
#include "../headers/internal_cmds.h"
#include "../headers/external_cmds.h"

#define PATH_MAX 4096

typedef struct comFor {  //structure pour les commandes structurées type for ... in .. {commandes}
    char *nom;  // renseigne le nom de la commande
    char *dir;  //renseigne le répertoire courant
    char *path; //renseigne le path du fichier dont l'on désire l'exécution
    char *options;  //donne les options de boucle
    char *ligne;  //donne toute la ligne de commande de l'input
} comFor;


comFor *initialiseCommandFor();  //initialise la structure command
int lengthPointer(char* pointer);  //calcul de la longueur d'un tableau pointé
comFor *fillCommandFor(char **argv);  //remplit les champs selon les infos
void clearCommandFor(comFor *com);  //free tous les champs de la strcut type comFor (commande for)

int parcoursFor(comFor* cm);  //fait le parcours du répertoire courant et y applique la commande à chaque fichier trouvé

#endif