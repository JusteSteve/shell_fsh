/**
 * @file redir_stdout_trunc.c
 * Contient l'implémentation de CMD >| FIC
 */
#include "../../../headers/redir.h"
#include "../../../headers/str-cmd.h"

int redir_stdout_trunc(char *line) {
    command *cmd = NULL;
    char *redir_pos;
    int fd = -1;
    int fd_stdout = -1;

    // Recherche de la redirection ">|"
    redir_pos = strstr(line, ">|");
    if (redir_pos == NULL) {
        // comme d'habitude, ça ne devrait pas arriver car si c'est appelé c'est qu'il y en a une
        dprintf(STDERR_FILENO, "Error: redir_stdout_trunc shouldn't be called at all\n");
        return 0; 
    }

    // Sauvegarder la sortie standard actuelle
    fd_stdout = dup(STDOUT_FILENO);
    if (fd_stdout == -1) {
        dprintf(STDERR_FILENO, "Error saving stdout: %s\n", strerror(errno));
        goto error;
    }

    // Extraire le nom du fichier après ">|"
    char *output_file = strtok(redir_pos + 2, " ");
    if (output_file == NULL) {
        dprintf(STDERR_FILENO, "Error: no output file specified\n");
        goto error;
    }

    // Ouvrir le fichier avec O_TRUNC pour forcer l'écrasement
    fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
    if (fd == -1) {
        dprintf(STDERR_FILENO, "Error opening %s: %s\n", output_file, strerror(errno));
        goto error;
    }

    // Rediriger la sortie standard vers le fichier
    if (dup2(fd, STDOUT_FILENO) == -1) {
        dprintf(STDERR_FILENO, "Error redirecting stdout: %s\n", strerror(errno));
        goto error;
    }

    close(fd); // Le fichier est maintenant redirigé vers STDOUT_FILENO

    // Retirer la partie de redirection de la ligne de commande
    *redir_pos = '\0'; 

    // Remplir la structure command
    cmd = fillCommand(line);
    if (cmd == NULL) {
        // impossible normalement
        dprintf(STDERR_FILENO, "Error: fillCommand failed\n");
        goto error;
    }

    // Créer un processus enfant pour exécuter la commande
    pid_t pid = fork();
    if (pid == -1) { // ça devrait pas arriver mais juste au cas où aussi
        dprintf(STDERR_FILENO, "Error: fork failed: %s\n", strerror(errno));
        goto error;
    }

    if (pid == 0) {
        // execvp : j'utilisais un tab d'args ici au lieu d'avoir utilisé ce qui était déjà fait
        execvp(cmd->nom, cmd->args);
        // Si execvp échoue :
        dprintf(STDERR_FILENO, "Error: execvp failed: %s\n", strerror(errno));
        _exit(1); // Sortie immédiate en cas d'échec car c'est le gosse
    }

    // Processus parent : attendre la fin du processus enfant
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        dprintf(STDERR_FILENO, "Error: waitpid failed: %s\n", strerror(errno));
        goto error;
    }

    // Restaurer la sortie standard
    if (dup2(fd_stdout, STDOUT_FILENO) == -1) {
        dprintf(STDERR_FILENO, "Error restoring stdout: %s\n", strerror(errno));
        goto error;
    }

    close(fd_stdout); 
    clearCommands(cmd); // Libérer la mémoire associée à la commande
    return WEXITSTATUS(status); // Retourne le code de sortie de la commande

error:
    // encore une fois je vérifie avant de fermer, utile ou pas ?
    // en tout cas mieux que sans vérification et puis ça marche.
    if (fd != -1) {
        close(fd);
    }
    if (fd_stdout != -1) {
        dup2(fd_stdout, STDOUT_FILENO);
        close(fd_stdout);
    }
    if (cmd != NULL) {
        clearCommands(cmd); // Libérer la mémoire en cas d'erreur
    }
    return 1;
}
