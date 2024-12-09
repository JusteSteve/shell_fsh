/**
 * @file cmd-utils.h
 * @brief Fichier contenant les prototypes des fonctions utilitaires pour exécuter des commandes.
 */

#ifndef CMD_UTILS_H
#define CMD_UTILS_H

// ***=== Inclusions ===***
#include "str-cmd.h"
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>



typedef struct commandsInfo {  // structure regroupant tous les infos d'une ligne: toutes les commandes classiques, toutes les commandes pour chaque for et leur nombre
	char ** commands;
	char ** commandsFor;
	size_t* com;
	size_t* comfor;
} commandsInfo;

commandsInfo *interm(char* line);
int execut_commande(commandsInfo * com);
char *trim(char *str);
char *creating_for_command(char *corps, char *core);

/**
 * Analyse et exécute la ligne de commande passée en paramètre
 * @param line : la ligne de commande
 * @return : 0 en cas de succès et 1 en cas d'échec
 */
int execute_commande(commandsInfo * com);

/**
 * @param line : ligne de commande
 * @return 1 si la ligne est vide, 0 sinon
 */
int is_line_empty(char *line);

// ***=== Fonctions utilitaires pour les commandes internes ===***

/**
 * @param cmd : nom de commade
 * @return 1 si c'est une commande interne 0 sinon
 */
int is_internal_cmd(char *cmd);

/**
 * Exécute la commande interne passée en paramètre
 * @param line la commande à exécuter
 * @return le code de retour de la commande 0 ou 1
 */
int exec_internal_cmds(char *line);

// ***=== Fonctions utilitaires pour les commandes externes ===***

/**
 * Exécute la commande externe passée en paramètre
 * @param args : tableau de chaines de caracteres
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int exec_external_cmds(command *cmd);

// ***=== Fonctions utilitaires pour les commandes structurées ===***

/**
 * @brief Exécute la commande for passée en paramètre
 * @param cmd : pointeur sur la structure command
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int exec_for_cmds(command *cmd);

/*
  @brief Exécute la commande structurée passée en paramètre
  @param line : ligne de commande
  @return 0 si tout s'est bien passé, 1 sinon
int exec_structured_cmds(char *line); */

#endif