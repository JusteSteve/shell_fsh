/**
 * @file cmd-utils.h
 * @brief Fichier contenant les prototypes des fonctions utilitaires pour exécuter des commandes.
 */

#ifndef CMD_UTILS_H
#define CMD_UTILS_H

// ***=== Inclusions ===***
#include "str-cmd.h"

/**
 * Analyse et exécute la ligne de commande passée en paramètre
 * @param line : la ligne de commande
 * @return : 0 en cas de succès et 1 en cas d'échec
 */
int execute_commande(char *line);

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
 * @param args : tableau de chaines de caracteres
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int exec_external_cmds(command *cmd);

#endif