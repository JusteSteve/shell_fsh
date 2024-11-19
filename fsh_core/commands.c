//fichier contenant la structure des commandes, leur initialisation et son remplissage

#include "../headers/commands.h"


command *initialiseCommand(){
    command *com = malloc(sizeof(command));
    if (com == NULL){return 1;}
    
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
            printf("Prolbème d'initilisation");
            return -1;
        }
    }
    return compt;
    }


command *fillCommand(command *com, char **argv){
    com->nom = malloc(sizeof(char));  //à changer
    
    return com;
}

void clearCommands(command *com){
    free(com->nom);
    free(com->args);
    free(com->ligne);
    free(com->options);
    free(com);
}
