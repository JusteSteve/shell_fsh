/**
 * @file redir.handler
 * Fichier qui permet d'appeler les fonctions de redirections
 */

#include "../../headers/redir.h"

/*
int is_redirection(char *line){
    if (strstr(line, " > ") != NULL || strstr(line, " >| ") != NULL || 
        strstr(line, " < ") != NULL || strstr(line, " >> ") != NULL ||
        strstr(line, " 2> ")!= NULL || strstr(line, " 2>| ")!= NULL ||
        strstr(line, " 2>> ")!= NULL) {
        return 1;
    }
    return 0;
}
*/

int is_redirection(char *line) {
    if (strstr(line, " < ") != NULL) {
        return REDIR_STDIN;
    }
    else if (strstr(line, " > ") != NULL) {
        return REDIR_STDOUT;
    }
    else if (strstr(line, " >| ") != NULL) {
        return REDIR_STDOUT_TRUNC;
    }
    else if (strstr(line, " >> ") != NULL) {
        return REDIR_STDOUT_APPEND;
    }
    else if (strstr(line, " 2> ") != NULL) {
        return REDIR_STDERR;
    }
    else if (strstr(line, " 2>| ") != NULL) {
        return REDIR_STDERR_TRUNC;
    }
    else if (strstr(line, " 2>> ") != NULL) {
        return REDIR_STDERR_APPEND;
    }
    else {
        return -1;
    }
}

int redir_handler (char *line, int type) {
    switch (type) {
        case REDIR_STDIN:
            //printf ("redir stdin\n");
            return redir_stdin(line);
        case REDIR_STDOUT:
            //printf ("redir stdout\n");
            return redir_stdout(line);
        case REDIR_STDOUT_TRUNC:
            //printf ("redir stdout trunc\n");
            return redir_stdout_trunc(line);
        case REDIR_STDOUT_APPEND:
            //printf ("redir stdout append\n");
            return redir_stdout_append(line);
        case REDIR_STDERR:
            //printf ("redir stderr\n");
            return redir_stderr(line);
        case REDIR_STDERR_TRUNC:
            //printf ("redir stderr trunc\n");
            return redir_stderr_trunc(line);
        case REDIR_STDERR_APPEND:
            //printf ("redir stderr append\n");
            return redir_stderr_append(line);
        default:
            // ne devrait pas être appelé mais ça devrait pas arriver normalement
            dprintf (STDERR_FILENO, "Error: invalid redirection type\n");
            return 1;
    }
}