/**
 * Fichier d'en-tête
 * Contient les prototypes des fonctions et les inclusions nécessaires.
 * Permet de centraliser les inclusions et les prototypes.
 */
#ifndef FSH_H
#define FSH_H

// ***=== Inclusions ===***
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

// ===***=== Prototypes ===***===

// ***=== prompt.c ===***
#define PROMPT_MAX_LENGTH 54

/**
 * Ajoute la chaîne src à la chaîne dst
 * @param dst pointeur vers la chaîne de destination
 * @param src pointeur vers la chaîne source
 * @return le nombre de caractères ajoutés
 */
int add_to_prompt(char *dst, char *src);

/**
 * Tronque le chemin passé en paramètre
 * @param path le chemin à tronquer
 * @param max_length la taille maximale du chemin
 * @return le chemin tronqué
 */
char *truncate_path(char *path, int max_length);

/**
 * Génère une chaine de caractères représentant le prompt
 * @return la chaîne de caractères représentant le prompt
 */
char *display_prompt(int last_return_value);

/**
 * Analyse et exécute la ligne de commande passée en paramètre
 * @param line : la ligne de commande
 * @return : 0 en cas de succès et 1 en cas d'échec
 */
int execute_commande(char *line);

#endif
