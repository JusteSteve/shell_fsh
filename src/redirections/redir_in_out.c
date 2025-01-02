/**
 * @file redir_stdin.c
 * Implémentation de CMD < FIC avec gestion spéciale pour "exit"
 */

#include "../../headers/redir.h"

//! FIXME: régler problème du exit < fichier
int redir_in(redirection *redir)
{
    int flags = extraire_flags(redir);
    int fd = open(redir->fichier, flags);
    if (fd == -1) {
        if(errno == EEXIST)
        {
            dprintf(STDERR_FILENO, "Error opening file %s: %s\n", redir->fichier, strerror(errno));
        }else{
            dprintf(STDERR_FILENO, "Error opening file %s: %s\n", redir->fichier, strerror(errno));
        }
        return 1;
    } 

    if (dup2(fd, redir->fd) == -1) {
        dprintf(STDERR_FILENO, "Error redirecting stdin: %s\n", strerror(errno));
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}

int redir_out(command *cmd, redirection *redir) 
{
    int flags = extraire_flags(redir);
    int fd = open(redir->fichier, flags, 0644);
    if (fd == -1) 
    {
        if(errno == EEXIST)
        {
            dprintf(STDERR_FILENO, "Error: %s already exists\n", redir->fichier);
        }
        else
        {
            dprintf(STDERR_FILENO, "Error opening %s: %s\n", redir->fichier, strerror(errno));
        }
        clearCommands(cmd);
        return 1;
    }
    // rediriger la sortie  vers le fichier
    if (dup2(fd, redir->fd) == -1) 
    {
        dprintf(STDERR_FILENO, "Error redirecting stdout: %s\n", strerror(errno));
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}



