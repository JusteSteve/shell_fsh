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

/**
 * @brief Exécute la commande structurée passée en paramètre
 * @param line : ligne de commande
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int exec_structured_cmds(char *line);

/**
 * @brief reconstruit la commande for ... { ... } dans une seule case
 * si elle est dans une commande structurée
 * @param tokens : chaine de caractères
 * @return commande for ... { ... }
 */
char *build_for_cmd(char *tokens);

// ***=== Fonctions utilitaires pour les pipelines ===***


/**
 * @brief Exécute les commandes d'un pipeline
 * @param line : ligne de commande
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int exec_pipeline_cmds(char *line);

/**
 * @brief sépare les commandes d'un pipeline
 * @param line : ligne de commande
 * @return tableau de pointeurs de char contenant les commandes du pipeline
 */
char **split_pipeline_cmd(char *line);

char *build_for_pipe_cmd(char *tokens);

#endif