/**
 * fichier contenant l'implémentation de la boucle for et ses options
 * @file for.c
 */
#include "../headers/for.h"
#include "../headers/commands.h"
#include "../headers/fsh.h"
#include "../headers/internal_cmds.h"
#include "../headers/external_cmds.h"


comFor *initialiseCommandFor(){
    comFor *com = malloc(sizeof(comFor));
    if (com == NULL){goto error;}
    
    com->command = NULL;
    com->options = NULL;
    com->path = NULL;
    com->dir = NULL;
    com->ligne = NULL;
    
    return com;

    error:
    clearCommandFor(com);
    return NULL;
}

/* int lengthPointerFor(char* pointer) {
    if (pointer == NULL) {
        return 0; // Si le pointeur est NULL, retourne une longueur de 0
    }

    int compt = 0;
    for (int i = 0; i < 100; i++) { // Parcourt les 100 premiers caractères (au maximum)
        if (pointer[i] != '\0') { // Vérifie si le caractère à l'index `i` est non nul
            compt += 1; // Incrémente le compteur
        } else {
            break; // Arrête la boucle si on atteint la fin de la chaîne
        }
    }
    return compt; // Retourne la longueur de la chaîne
} */

comFor *fillCommandFor(char **args){  //version ne prennant pas compte des options
    comFor *com = initialiseCommandFor();
    if (com == NULL) {goto error;}
    int i = 0;  
    char str[100];
    strcpy(str, args[4]);
    if (strcmp(str, "{") != 0) {  //savoir si l'utilisateur a imput "{CMD}" ou "{ CMD }" (version correcte)
        perror("User input");
        goto error;
    }

    //malloc des ressources

    com->command = malloc(sizeof(char) * (strlen(args[5]) + 1));
    if (com->command == NULL){goto error;}
    com->dir = malloc(sizeof(char) * (strlen(args[3]) + 1));
    if (com->dir == NULL){goto error;}
    com->ligne = malloc(sizeof(char) * MAX_CMDS);
    if (com->ligne == NULL){goto error;}

    com->command = strdup(args[5]);
    com->dir = strdup(args[3]);
    while(args[i] != NULL && i < MAX_CMDS - 1){
        strcpy(com->ligne + i, args[i]);
        i++;
    }
    return com;

    error:
    clearCommandFor(com);
    return NULL;
} 


void clearCommandFor(comFor *com){
    if(com->command!= NULL){free(com->command);}
    if(com->ligne != NULL){free(com->ligne);}
    if(com->path != NULL){free(com->path);}
    if(com->options != NULL){free(com->options);}
    if(com->dir != NULL){free(com->dir);}
    if(com != NULL){free(com);}
}

int parcoursFor(comFor* cm){
    DIR* parent = NULL;
    struct dirent* entry;
    // struct stat st_entry; // Supprimé car inutilisé
    // char *paths; // Supprimé car inutilisé

    parent = opendir(cm->dir);
    if (parent == NULL) {goto error;}

    while ((entry = readdir(parent))){
        errno = 0;
        if (entry == NULL && errno != 0) {goto error;}  //si readdir rencontre une erreur, errno est modif avec une valeur non nulle, et si fin de fichiers à lire, errno ne change pas de valeur
        // car si on ne trouve pas le répertoire que l'on cherche dans son parent, c'est une erreur
        else if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {continue;}
        char entry_path [PATH_MAX]; //hypothèse que le chemin fasse au moins PATH_MAX, ce n'est pas judicieux, mais sans c'est compliqué
        snprintf(entry_path, PATH_MAX, "../%s", cm->path);

        if (is_internal_cmd(cm->command) == 0){
            exec_internal_cmds(cm->ligne);  //remplacer le paramètre
        }
        else{
            exec_external_cmdsFor(cm);  //cm->command, cm->command, entry_path, NULL
        }
    }
    return 0;

    error:
    if (parent != NULL) {closedir(parent);}
    clearCommandFor(cm);
    return 1;
}