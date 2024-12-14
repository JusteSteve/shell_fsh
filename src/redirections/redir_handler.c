/**
 * @file redir.handler
 * Fichier qui permet d'appeler les fonctions de redirections
 */

#include "../../headers/redir.h"

int is_redirection(char *line){
    // Vérification des différents types de redirection (faudrait que j'améliore le système de détection)
    if (strstr(line, " > ") != NULL || strstr(line, " >| ") != NULL || 
        strstr(line, " < ") != NULL || strstr(line, " >> ") != NULL ||
        strstr(line, " 2> ")!= NULL || strstr(line, " 2>| ")!= NULL ||
        strstr(line, " 2>> ")!= NULL) {
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
    if (strstr(line, " < ") != NULL) {
        printf("redir_stdin\n");
        return redir_stdin(line);
    }
    else if (strstr(line, " > ") != NULL) {
        printf("redir_stdout\n");
        return redir_stdout(line);
    }
    else if (strstr(line, " >| ") != NULL) {
        printf("redir_stdout_trunc\n");
        return redir_stdout_trunc(line);
    }
    else if (strstr(line, " >> ") != NULL) {
        printf("redir_stdout_append\n");
        return redir_stdout_append(line);
    }
    else if (strstr(line, " 2> ") != NULL) {
        printf("redir_stderr\n");
        return redir_stderr(line);
    }
    else if (strstr(line, " 2>| ") != NULL) {
        printf("redir_stderr_trunc\n");
        return redir_stderr_trunc(line);
    }
    else if (strstr(line, " 2>> ") != NULL) {
        printf("redir_stderr_append\n");
        return redir_stderr_append(line);
    }
    else {
        return 1;
    }
}