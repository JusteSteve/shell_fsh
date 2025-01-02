/**
 * @file redir_stdout.c
 * Contient l'implémentation de CMD > FIC
 */
#include "../../../headers/redir.h"
#include "../../../headers/str-cmd.h"


int redir_stdout(char *line, redirection *redir) {
    command *cmd = NULL;
    char *redir_pos;
    int fd = -1;
    int fd_stdout = -1;
    int flags = -1;

    // Recherche de la redirection de sortie " > "
    /*
    */
    redir_pos = strstr(line, " > ");
    if (redir_pos == NULL) {
        dprintf(STDERR_FILENO, "Error: redir shouldn't be called at all\n");
        return 0; 
    }

    // Sauvegarder la sortie standard actuelle
    fd_stdout = redir->fd;
    if (fd_stdout == -1) {
        dprintf(STDERR_FILENO, "Error saving stdout: %s\n", strerror(errno));
        goto error;
    }

    // Extraire le nom du fichier après " > "
    char *output_file = redir->fichier;
    if (output_file == NULL) {
        dprintf(STDERR_FILENO, "Error: no output file specified\n");
        goto error;
    }

    // Vérifier si le fichier existe déjà
    struct stat stat_exist;
    if (stat(output_file, &stat_exist) == 0) {
        dprintf(STDERR_FILENO, "Error: %s already exists\n", output_file);
        goto error;
    }

    // Ouvrir le fichier avec O_EXCL pour éviter l'écrasement
    flags = extraire_flags(redir);
    fd = open(output_file, flags, 0644); 
    if (fd == -1) {
        dprintf(STDERR_FILENO, "Error opening %s: %s\n", output_file, strerror(errno));
        goto error;
    }

    // Rediriger la sortie standard vers le fichier
    if (dup2(fd, redir->fd) == -1) {
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
        dprintf(STDERR_FILENO, "Error: failed to parse command\n");
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
        dprintf(STDERR_FILENO, "Error : execvp failed %s\n", strerror(errno));
        _exit(1); // Sortie immédiate en cas d'échec car c'est le gosse
    }

    // Processus parent : attendre la fin du processus enfant
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        dprintf(STDERR_FILENO, "Error : child's a failure (asian mood) %s\n", strerror(errno));
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

    // sauvegarder la sortie standard
    int fd_std = dup(STDOUT_FILENO);
    if (fd_std == -1) {
        dprintf(STDERR_FILENO, "Error saving stdout: %s\n", strerror(errno));
        close(fd);
        clearCommands(cmd);
        return 1;
    }
    // rediriger la sortie  vers le fichier
    if (dup2(fd, redir->fd) == -1) 
    {
        dprintf(STDERR_FILENO, "Error redirecting stdout: %s\n", strerror(errno));
        close(fd);
        close(fd_std);
        clearCommands(cmd);
        return 1;
    }
    close(fd);
    /*
    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            dprintf(STDERR_FILENO, "Error: fork failed: %s\n", strerror(errno));
            close(fd_std);
            clearCommands(cmd);
            return 1;
        case 0:
            execvp(cmd->nom, cmd->args);
            dprintf(STDERR_FILENO, "Error: execvp failed: %s\n", strerror(errno));
            exit(1);
        default:
        {
            int status = 0;
            if (waitpid(pid, &status, 0) == -1)
            {
                dprintf(STDERR_FILENO, "Error: waitpid failed: %s\n", strerror(errno));
                close(fd_std);
                clearCommands(cmd);
                return 1;
            }

            // restauration de la sortie standard
            if (dup2(fd_std, STDOUT_FILENO) == -1)
            {
                dprintf(STDERR_FILENO, "Error restoring stdout: %s\n", strerror(errno));
                close(fd_std);
                clearCommands(cmd);
                return 1;
            }
            close(fd_std);
            clearCommands(cmd);

            return WEXITSTATUS(status);
        }
    }
    */
    return 0;


}
