// headers/pwd.h
#ifndef INTERNAL_CMDS_H
#define INTERNAL_CMDS_H

// ***=== Inclusions ===***
//#include <linux/limits.h> // on va le define nous même car sur macos ça marche pas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

// ***=== pwd.c ===***
#define PATH_MAX 4096

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

#endif
