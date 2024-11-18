/**
 * Fichier contenant l'implémentation de la commande exit
 * @file exit.c
 */
#include "../headers/internal_cmds.h"

extern int prev_status; 

// on va remplacer atoi() car je sais pas trop si c'est un appel système
/*L'idée sera juste de faire pour tout chiffre à la nth position, chiffre*10^n-1*/
int atoi_alternative(char *val) { // issue d'un exo des TP de C de L2 (j'crois)
    int sum = 0;
    int i = 0;
    int isPos = 1;

    if (val[0] == '-') {
        isPos = -1;
        i++;
    }

    while (val[i] != '\0') {
        sum = sum * 10 + val[i] - '0';
        i++;
    }
    return sum*isPos; 
}

int cmd_exit(char *val) {
    int status;

    // Si val est non NULL, on convertit val en entier car on a des char car line 
    if (val != NULL) {
        //status = atoi(val); // Converti(ssai)t la chaîne en entier
        status = atoi_alternative(val);
    } 
    else {
        status = prev_status; // Sinon on prend le statut précédent
    }
    
    // Libération des ressources et sortie avec status
    dprintf(STDOUT_FILENO, "Exiting fsh with code %d\n", status); // Remplace printf par dprintf
    exit(status);
}