//fichier contenant la structure des commandes, leur initialisation et son remplissage

#include "../headers/commands.h"


command *initialiseCommand(){
    command *com = malloc(sizeof(command));
    if (com == NULL){return NULL;}
    
    com->nom = NULL;
    com->args = NULL;
    com->options = NULL;
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
            printf("Problème d'initilisation");
            return -1;
        }
    }
    return compt;
    }


command *fillCommand(char **argv){
    int i = 0;
    int nbOptions = 0, nbArgs = 0;
    command *com = initialiseCommand();

    //malloc pour les champs

    com->nom = malloc(sizeof(char) * strlen(argv[0])); 
    if(com->nom == NULL){goto error;}
    com->args = malloc(sizeof(char) * PATH_MAX);
    if(com->args == NULL){goto error;}
    com->options = malloc(sizeof(char) * _MAX_DIR); 
    if(com->options == NULL){goto error;}
    com->ligne = malloc(sizeof(char) * PATH_MAX);
    if(com->ligne == NULL){goto error;}

    //Remplissage des champs

    com->nom = argv[0];
    while (argv[i] != NULL){
        (com->ligne + i) = argv[i];
        char str[] = argv[i];
        if (strcmp(str[0], "-") == 0 && i > 0){
            (com->options + i) = argv[i];
            nbOptions++;
        }
        else if (i > 0){
            (com->args + nbArgs) = argv[i];
            nbArgs++;
        }

    }

    return com;

    error:
    clearCommands(com);
    return NULL;
}

void clearCommands(command *com){
    if(com->nom != NULL){free(com->nom);}
    if(com->ligne != NULL){free(com->ligne);}
    if(com->args != NULL){free(com->args);}
    if(com->options != NULL){free(com->options);}
    if(com != NULL){free(com);}
}
