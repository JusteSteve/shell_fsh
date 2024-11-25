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

#define PATH_MAX 4096

typedef struct command {  //tailles des tableaux à moduler au cas où
    char *nom;  //nom de l'exécutable pour la commande
    char *args;  //arguments de la commande
    char *options;  //pour toutes les options associées
    char *ligne; //toute la ligne de commande
} command;

// déclaration des fonctions

command *initialiseCommand();  //initialise la structure command
int lengthPointer(char* pointer);  //calcul de la longueur d'un tableau pointé
command *fillCommand(char **argv);  //remplit les champs selon les infos
void clearCommands(command *com);  //free tous les champs de la structure type command

#endif