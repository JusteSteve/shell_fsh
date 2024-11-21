/**
 * Fichier contenant l'implémentation de la commande exit
 * @file exit.c
 */
#include "../headers/internal_cmds.h"

int cmd_exit(char *val) {
    int status;

    // Si val est non NULL, on convertit val en entier car on a des char car line 
    if (val != NULL) {
        status = atoi(val); // Convertit la chaîne en entier
        prev_status = status; // On met à jour le statut précédent
    } 
    else {
        status = prev_status; // Sinon on prend le statut précédent
    }
    
    // Libération des ressources et sortie avec status
    dprintf(STDOUT_FILENO, "DEBUG: Exiting fsh with code %d (prev_status=%d)\n", status, prev_status); // to remove besoin de le debug aussi -.-
    exit(status);
}
