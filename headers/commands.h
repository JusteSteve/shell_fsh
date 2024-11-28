#ifndef COMMAND_H
#define COMMAND_H

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

#define PATH_MAX 4096

typedef struct command
{
    char *nom;   // nom de l'exécutable pour la commande
    char **args; // arguments de la commande
    char *ligne; // toute la ligne de commande
    int taille;  // nombre d'éléments dans le tableau args
} command;

// déclaration des fonctions

command *initialiseCommand();     // initialise la structure command
command *fillCommand(char *line); // remplit les champs selon les infos
void clearCommands(command *com); // free tous les champs de la structure type command

#endif