/**
 * @file redir_stderr.c
 * Contient l'implémentation de CMD 2> FIC
 */
#include "../../../headers/redir.h"
#include "../../../headers/str-cmd.h"

int redir_stderr(char *line) {
    command *cmd = NULL;
    char *redir_pos;
    int fd = -1;
    int fd_stderr = -1;

    // Recherche de "2>"
    redir_pos = strstr(line, "2>");
    if (redir_pos == NULL) {
        dprintf(STDERR_FILENO, "Error: redir_stderr shouldn't be called at all\n");
        goto error;
    }

    // Sauvegarder STDERR
    fd_stderr = dup(STDERR_FILENO);
    if (fd_stderr == -1) {
        dprintf(STDERR_FILENO, "Error saving stderr: %s\n", strerror(errno));
        goto error;
    }

    // Prendre le nom du fichier après "2>"
    char *error_file = strtok(redir_pos + 2, " ");
    if (error_file == NULL) {
        dprintf(STDERR_FILENO, "Error: no error file specified\n");
        goto error;
    }

    // EXCL 
    fd = open(error_file, O_WRONLY | O_CREAT | O_EXCL, 0644); 
    if (fd == -1) {
        dprintf(STDERR_FILENO, "Error opening %s: %s\n", error_file, strerror(errno));
        goto error;
    }

    // Rediriger STDERR vers le fichier
    if (dup2(fd, STDERR_FILENO) == -1) {
        dprintf(STDERR_FILENO, "Error redirecting stderr: %s\n", strerror(errno));
        goto error;
    }

    close(fd); // Le descripteur de fichier n'est plus nécessaire

    // Retirer la partie de redirection de la ligne de commande
    *redir_pos = '\0';

    // Remplir la structure command avec la commande restante
    cmd = fillCommand(line);
    if (cmd == NULL) {
        dprintf(STDERR_FILENO, "Error: failed to parse command\n");
        goto error;
    }

    // Créer un processus enfant pour exécuter la commande
    pid_t pid = fork();
    if (pid == -1) { 
        dprintf(STDERR_FILENO, "Error: fork failed: %s\n", strerror(errno));
        goto error;
    }

    if (pid == 0) {
        execvp(cmd->nom, cmd->args);
        dprintf(STDERR_FILENO, "Error: execvp failed: %s\n", strerror(errno));
        _exit(1); // on oublie pas de sortir de l'enfant
    }

    // Attendre que le processus enfant termine
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        dprintf(STDERR_FILENO, "Error: waitpid failed: %s\n", strerror(errno));
        goto error;
    }

    // Restaurer STDERR
    if (dup2(fd_stderr, STDERR_FILENO) == -1) {
        dprintf(STDERR_FILENO, "Error restoring stderr: %s\n", strerror(errno));
        goto error;
    }

    close(fd_stderr);
    clearCommands(cmd);
    return WEXITSTATUS(status);

error:
    // comme d'habitude avec les if par précaution
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
