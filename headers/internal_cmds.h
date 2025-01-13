/**
 * Fichier d'en-tête
 * Contient les définitions de contantes, les prototypes des fonctions et les inclusions nécessaires.
 * Permet de centraliser les inclusions et le prototypes
 */

#ifndef INTERNAL_CMDS_H
#define INTERNAL_CMDS_H

// ***=== Inclusions ===***
#include "fsh.h"

extern int prev_status; // pour stocker le status précédent

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

#endif
