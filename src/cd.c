/**
 * Fichier contenant l'implémentation de la commande cd
 * @file cd.c
 */
#include "../headers/internal_cmds.h"

static char previous_dir[PATH_MAX] = ""; // Pour stocker le répertoire précédent

int cmd_cd(char *path){
    char *current_dir = malloc(PATH_MAX);

    // Vérifie si l'allocation de mémoire a échoué (qui ne devrait pas arriver)
    // ou bien si getcwd a échoué
    if (current_dir == NULL || getcwd(current_dir, PATH_MAX) == NULL){
        perror("cd : malloc failed");
        goto error;
    }

    // Si le paramètre est NULL ou vide, on va au répertoire $HOME
    if (path == NULL || strlen(path) == 0){
        // pas dans la liste des systems calls mais on a le droit 
        // d'après la conversation qu'il y a eu sur Discord du groupe SY5s
        path = getenv("HOME");
        if (path == NULL){
            fprintf(stderr, "cd : Error HOME (sdf)\n");
            goto error;
        }
    }
    // Si le paramètre est "-", on va au répertoire précédent
    else if (strcmp(path, "-") == 0){
        if (strlen(previous_dir) == 0){
            fprintf(stderr, "cd : previous directory not set\n");
            goto error;
        }
        path = previous_dir; // opla tour de magie on conserve le path
    }

    // Changer de répertoire
    if (chdir(path) == -1){
        perror("cd");
        goto error;
    }

    // Met à jour le répertoire précédent puis on libère la mémoire
    strncpy(previous_dir, current_dir, PATH_MAX);
    free(current_dir);

    return 0;

    // gestion d'erreur
    error:
        /*Pour éviter double/free ou bien un free sur quelque chose qui n'a pas 
          été alloué, le chargé de TP/TD en a parlé dans l'un des exercices du 
          TP8 (je crois), donc un if au cas où  
        */
        if (current_dir != NULL){
            free(current_dir);
        }
        return 1;
}
