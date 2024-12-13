/**
 * @file redir_stdin.c
 * Implémentation de CMD < FIC
 */

#include "../../../headers/redir.h"

int redir_stdin(char *line) {
    char *input_file = NULL;
    char *redir_pos;
    int fd_pipe;
    int fd_stdin;

    // Recherche de la redirection " < "
    redir_pos = strstr(line, " < ");
    if (redir_pos == NULL) {
        // affiche un message d'erreur comme quoi redir_stdin n'aurait pas dû être appelé
        dprintf(STDERR_FILENO, "Error : redir_stdin shouldn't be called at all\n");
        goto error;
    }

    // Sauvegarder l'entrée standard actuelle
    fd_stdin = dup(STDIN_FILENO);
    if (fd_stdin == -1) {
        // si l'entrée standard n'est pas sauvegardée, affiche un message d'erreur
        dprintf(STDERR_FILENO, "Error : fd_stdin");
        goto error;
    }

    // Extraire le nom du fichier après " < "
    input_file = strtok(redir_pos + 3, " ");
    if (input_file == NULL) {
        // ne devrait pas arriver mais au cas où
        dprintf(STDERR_FILENO, "Error : input_file\n");
        goto error;
    }

    // Ouvrir le fichier ou le tube nommé en lecture
    fd_pipe = open(input_file, O_RDONLY);
    if (fd_pipe == -1) {
        dprintf(STDERR_FILENO, "Error opening %s: %s\n", input_file, strerror(errno));
        goto error;
    }

    // Rediriger l'entrée standard vers le fichier
    if (dup2(fd_pipe, STDIN_FILENO) == -1) {
        dprintf(STDERR_FILENO, "Error redirecting stdin: %s\n", strerror(errno));
        goto error;
    }

    close(fd_pipe); // Le descripteur du fichier est maintenant associé à STDIN_FILENO

    // Retirer la partie de redirection de la ligne de commande
    *redir_pos = '\0';

    // Exécuter la commande avec l'entrée redirigée
    int result = system(line);

    // Restaurer l'entrée standard pour le shell
    if (dup2(fd_stdin, STDIN_FILENO) == -1) {
        dprintf(STDERR_FILENO, "Error restoring stdin: %s\n", strerror(errno));
        goto error;
    }

    close(fd_stdin); // Fermer le descripteur sauvegardé
    return result;

    error :

    // on oublie pas de fermer les descripteurs, mais on vérifie tout
    // de même si ils sont ouverts avant de les fermer 
    // (j'prendrai pas option sécurité informatique mais pas une raison pour pas le faire)
    if (fd_pipe != -1) {
        close(fd_pipe);
    }
    if (fd_stdin != -1) {
        dup2(fd_stdin, STDIN_FILENO);
        close(fd_stdin);
    }

    return 1;
}
