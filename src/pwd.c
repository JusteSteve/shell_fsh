/**
 * Fichier contenant l'implémentation de la commande pwd
 * @file pwd.c
 */

#include "../headers/internal_cmds.h"

int cmd_pwd() {
    char *cwd_buffer = malloc(PATH_MAX);

    // ça devrait pas arriver mais au cas où
    if (cwd_buffer == NULL || getcwd(cwd_buffer, PATH_MAX) == NULL) {
        perror("pwd : malloc failed");
        goto error;
    }

    if (getcwd(cwd_buffer, PATH_MAX) != NULL) {
        dprintf(STDOUT_FILENO, "%s\n", cwd_buffer);  // Utilisation de dprintf à la place de printf
        free(cwd_buffer);
        return 0;
    } else {
        perror("print_path");
        goto error;
    }

    // Gestion d'erreur
error:
    free(cwd_buffer);
    return 1;
}
