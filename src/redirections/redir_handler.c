/**
 * @file redir.handler
 * Fichier qui permet d'appeler les fonctions de redirections
 */

#include "../../headers/redir.h"

int is_redirection(char *line){
    // Vérification des différents types de redirection
    if (strstr(line, " > ") != NULL || strstr(line, " >> ") != NULL || 
        strstr(line, " < ") != NULL || strstr(line, " << ") != NULL){
        return 1;
    }
    return 0;
}

int redir_handler (char *line) {
    /*  plus besoin, redondance, je sais pas encore parfaitement comment ça va être géré
    if (is_redirection(line)) {
        return redir(line);
    }
    return 0;
    */
    // vérifie quel type de redirection est demandé
    if (strstr(line, " < ") != NULL || strstr(line, " < ") != NULL) {
        return redir_stdin(line);
    }
    else if (strstr(line, " > ") != NULL || strstr(line, " >> ") != NULL) {
        return redir_stdout(line);
    }
    else {
        return 1;
    }
}