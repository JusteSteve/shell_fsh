/**
 * Fichier d'en-tête
 * Contient les définitions de contantes, les prototypes des fonctions et les inclusions nécessaires.
 * Permet de centraliser les inclusions et le prototypes
 */

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

#define PATH_MAX 4096
int prev_status;

/**
 * Affiche le chemin absolu du répertoire courant
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int cmd_pwd(void);

/**
 * Change le répertoire courant
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int cmd_cd(char *path);

/**
 * quitte fsh avec un code de sortie
 * @return val, sinon prev_status
 */
int cmd_exit(char *val);

/**
 * Affiche le type de fichier
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int cmd_ftype(const char *ref);

/**
 * Affiche le contenu répertoire
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int cmd_ls(char *path);

/**
 * Affiche le contenu du fichier
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int cmd_cat(char *path);

// ***=== Fonctions auxiliaires ===***

/**
 * @param cmd : nom de commade
 * @return 1 si c'est une commande interne 0 sinon
 */
int is_internal_cmd(char *cmd);

/**
 * Exécute la commande passée en paramètre
 * @param line la commande à exécuter
 * @return le code de retour de la commande 0 ou 1
 */
int exec_internal_cmds(char *line);

#endif
