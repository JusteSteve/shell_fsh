/**
 * Fichier d'en-tête
 * Contient les prototypes des fonctions et les inclusions nécessaires.
 * Permet de centraliser les inclusions et les prototypes.
 */
#ifndef FSH_H
#define FSH_H

// ***=== Inclusions ===***
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>

// ===***=== Prototypes ===***===

// ***=== prompt.c ===***
#define PROMPT_MAX_LENGTH 54
#define PATH_MAX 4096
#define MAX_CMDS 1024 // nombre maximal d'arguments dans une commande


#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RED "\033[91m"
#define COLOR_DEFAULT "\033[00m"

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

#endif
