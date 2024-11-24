/**
 * fichier contenant l'implémentation de la boucle for et ses options
 * @file for.c
 */
#include "../headers/for.h"


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
    clearCommands(com);
    return NULL;
}


int lengthPointer(char* pointer){
    int compt = 0;
    for (int i = 0; i < 100; i++){
        if (pointer+i != '\0' && pointer+i != NULL){
            compt+=1;
        }
        else if (pointer+i == NULL){
            printf("Problème d'initilisation");
            return -1;
        }
    }
    return compt;
}


comFor *fillCommandFor(char **args){  //version ne prennant pas compte des options
    comFor *com = initialiseCommandFor();
    int i = 0;  
    char str[] = args[4];
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

    com->command = args[5];
    com->dir = args[3];
    while(args[i] != NULL && i < MAX_CMDS - 1){
        (com->ligne + i) = args[i];
    }
    return com;

    error:
    clearCommands(com);
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
    struct stat st_entry;
    char *paths;


    parent = opendir(cm->dir);
    if (parent == NULL) {goto error;}

    while (entry = readdir(parent)){
        errno = 0;
        if (entry == NULL) {   //si readdir rencontre une erreur, errno est modif avec une valeur non nulle, et si fin de fichiers à lire, errno ne change pas de valeur
		    if (errno != 0) {goto error;}
		    else {goto error;} // car si on ne trouve pas le répertoire que l'on cherche dans son parent, c'est une erreur
	    }
        else if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {continue;}
        char entry_path [PATH_MAX]; //hypothèse que le chemin fasse au moins PATH_MAX, ce n'est pas judicieux, mais sans c'est compliqué
	    snprintf(entry_path, PATH_MAX, "../%s", cm->path);

    if (is_internal(cm->command) == 0){
        exec_internal_cmds(cm->ligne);  //remplacer le paramètre
    }
    else{
        exec_external_cmds(cm);  //cm->command, cm->command, entry_path, NULL
    }
    }
    return 0;

    error:
	if (parent != NULL) {closedir(parent);}
    clearCommandFor(com);
	return 1;
}


