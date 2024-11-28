/**
 * Fichier d'en-tête
 * Contient les prototypes des fonctions et les inclusions nécessaires.
 * Permet de centraliser les inclusions et les prototypes.
 */

#ifndef EXTERNAL_CMDS_H
#define EXTERNAL_CMDS_H

// ***=== Inclusions ===***

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "../headers/commands.h"
#include "../headers/for.h"

#define MAX_CMDS 100

/**
 * divise une ligne de commande en arguments
 * avec les espaces comme séparateurs.
 * @param cmd : ligne de commande à exécuter
 * @return tableau de pointeur de char contenant les arguments de la commande
 */
char **split_cmd(char *line);

/**
 * @param args : tableau de chaines de caracteres
 * libère la mémoire allouée pour args
 */
void free_args(char **args);

/**
 * @param args : tableau de chaines de caracteres
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int exec_external_cmds(command *cmd);

int exec_external_cmdsFor(comFor *cmd);

/**
 * @param args : tableau de chaines de caracteres
 * @return le nombre cases occupées de args
 */
int get_nb_args(char **args);

/**
 * @param line : ligne de commande
 * @return 1 si la ligne est vide, 0 sinon
 */
int is_line_empty(char *line);

#endif
