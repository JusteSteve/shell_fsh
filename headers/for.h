#ifndef INTERNAL_CMDS_H
#define INTERNAL_CMDS_H

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

#define PATH_MAX 4096

typedef struct comFor {  //structure pour les commandes structurées type for ... in .. {commandes}
    char *command;  // renseigne le nom de la commande
    int *F;  //renseigne la variable de boucle
    char *dir;  //renseigne le répertoire courant
    char *options;  //donne les options de boucle
    char *ligne;  //donne toute la ligne de commande de l'input
} comFor;


comFor *initialiseCommandFor();  //initialise la structure command
int lengthPointer(char* pointer);  //calcul de la longueur d'un tableau pointé
comFor *fillCommandFor(comFor *com, char **argv);  //remplit les champs selon les infos
void clearCommandFor(comFor *com);  //free tous les champs de la strcut type comFor (commande for)

int parcoursFor(comFor* cm);

#endif