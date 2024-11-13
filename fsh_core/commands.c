//fichier contenant la structure des commandes

#include <stdio.h>

struct command {  //tailles des tableaux à moduler au cas où
    char exe[20];  //nom de l'exécutable pour la commande
    char arg1[100];  //1er argument de la commande
    char arg2[100];  //2ème argument de la commande
    char args[200];  //si plus de 2 arguments
    char options[40];  //pour toutes les options associées
}