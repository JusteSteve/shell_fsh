/**
 * @file redir_stderr_trunc.c
 * Contient l'implémentation de CMD 2>| FIC
 */
#include "../../../headers/redir.h"
#include "../../../headers/str-cmd.h"

int redir_stderr_trunc(char *line) {
    command *cmd = NULL;
    char *redir_pos;
    int fd = -1;
    int fd_stderr = -1;

    // Recherche de "2>|"
    redir_pos = strstr(line, "2>|");
    if (redir_pos == NULL) {
        dprintf(STDERR_FILENO, "Error: redir_stderr_trunc shouldn't be called at all\n");
        goto error;
    }

    fd_stderr = dup(STDERR_FILENO);
    if (fd_stderr == -1) {
        dprintf(STDERR_FILENO, "Error saving stderr: %s\n", strerror(errno));
        goto error;
    }

    char *error_file = strtok(redir_pos + 3, " "); // Décale de 3 pour ignorer "2>|"
    if (error_file == NULL) {
        dprintf(STDERR_FILENO, "Error: no error file specified\n");
        goto error;
    }

    // TRUNC du coup
    fd = open(error_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
    if (fd == -1) {
        dprintf(STDERR_FILENO, "Error opening %s: %s\n", error_file, strerror(errno));
        goto error;
    }

    if (dup2(fd, STDERR_FILENO) == -1) {
        dprintf(STDERR_FILENO, "Error redirecting stderr: %s\n", strerror(errno));
        goto error;
    }

    close(fd); 

    // Retirer la partie de redirection de la ligne de commande
    *redir_pos = '\0';

    cmd = fillCommand(line);
    if (cmd == NULL) {
        dprintf(STDERR_FILENO, "Error: failed to parse command\n");
        goto error;
    }

    pid_t pid = fork();
    if (pid == -1) {
        dprintf(STDERR_FILENO, "Error: fork failed: %s\n", strerror(errno));
        goto error;
    }

    if (pid == 0) {
        execvp(cmd->nom, cmd->args);
        dprintf(STDERR_FILENO, "Error: execvp failed: %s\n", strerror(errno));
        _exit(1); 
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        dprintf(STDERR_FILENO, "Error: waitpid failed: %s\n", strerror(errno));
        goto error;
    }

    if (dup2(fd_stderr, STDERR_FILENO) == -1) {
        dprintf(STDERR_FILENO, "Error restoring stderr: %s\n", strerror(errno));
        goto error;
    }

    close(fd_stderr);
    clearCommands(cmd);
    return WEXITSTATUS(status);

error:
    if (fd != -1) {
        close(fd);
    }
    if (fd_stderr != -1) {
        dup2(fd_stderr, STDERR_FILENO);
        close(fd_stderr);
    }
    if (cmd != NULL) {
        clearCommands(cmd);
    }
    return 1;
}
