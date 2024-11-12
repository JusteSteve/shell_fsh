/**
 * Fichier contenant l'implémentation de la commande exit
 * @file exit.c
 */
#include "../headers/internal_cmds.h"
#include <stdlib.h>

extern int prev_status; 

int cmd_exit(char *val) {
    int status;

    // Si val est non NULL, on convertit val en entier car on a des char car line 
    if (val != NULL) {
        status = atoi(val); // magie ! String into int
    } 
    else {
        status = prev_status; // Sinon on prend le status précédent
    }
    
    // Libération des ressources et sortie avec status
    printf("Exiting fsh with code %d\n", status);
    exit(status);
}
