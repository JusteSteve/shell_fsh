/**
 * fichier contenant l'implémentation de la boucle for et ses options
 * @file for.c
 */
#include "../headers/for.h"


comFor *initialiseCommandFor(){
    comFor *com = malloc(sizeof(comFor));
    if (com == NULL){return 1;}
    
    com->command = NULL;
    com->F = NULL;
    com->options = NULL;
    com->dir = NULL;
    com->ligne = NULL;
    
    return com;
}


int lengthPointer(char* pointer){
    int compt = 0;
    for (int i = 0; i < 100; i++){
        if (pointer+i != '\0' && pointer+i != NULL){
            compt+=1;
        }
        else if (pointer+i == NULL){
            printf("Porlbème d'initilisation");
            return -1;
        }
    }
    return compt;
}


comFor *fillCommandFor(comFor *com, char **argv){

} 


void clearCommandFor(comFor *com){
    free(com->command);
    free(com->F);
    free(com->ligne);
    free(com->options);
    free(com->dir);
    free(com);
}



int parcoursFor(comFor* cm){
    DIR* parent = NULL;
    struct dirent* entry;
    struct stat st_entry;
    char *paths;

    int taille = cm->F;
    paths = malloc(taille*sizeof(char));
    if (paths == NULL) {goto error;}

    parent = opendir(cm->dir);
    if (parent == NULL) {goto error;}

    for (int i = 0; i < cm->F; i++){
        errno = 0;
        entry = readdir(parent);
        if (entry == NULL) {   //si readdir rencontre une erreur, errno est modif avec une valeur non nulle, et si fin de fichiers à lire, errno ne change pas de valeur
		    if (errno != 0) {goto error;}
		    else {goto error;} // car si on ne trouve pas le répertoire que l'on cherche dans son parent, c'est une erreur
	    }
        else if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {continue;}
        char entry_path [PATH_MAX]; //hypothèse que le chemin fasse au moins PATH_MAX, ce n'est pas judicieux, mais sans c'est compliqué
	    snprintf(entry_path, PATH_MAX, "../%s", entry->d_name);

        execlp(cm->command, cm->command, entry_path, NULL);
    }
    return 0;

    error:
	if (parent != NULL) {closedir(parent);}
	return 1;
}


