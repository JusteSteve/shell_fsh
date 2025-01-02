/**
 * @file redir_stdin.c
 * Implémentation de CMD < FIC avec gestion spéciale pour "exit"
 */

#include "../../../headers/redir.h"

int is_space(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
} /*jsp si on peut utiliser isspace*/  

int redir_stdin(char *line) {
    char *input_file = NULL;
    char *redir_pos;
    int fd_pipe = -1; /*error à la compilation sinon Wsometimes-uninitialized*/
    int fd_stdin = -1;
    /*j'ai oublié de remplacé system(line) par execvp */
    pid_t pid;
    int status;

    // Recherche de la redirection " < "
    redir_pos = strstr(line, " < ");
    if (redir_pos == NULL) {
        dprintf(STDERR_FILENO, "Error : redir_stdin shouldn't be called at all\n");
        goto error;
    }

    // Sauvegarder l'entrée standard actuelle
    fd_stdin = dup(STDIN_FILENO);
    if (fd_stdin == -1) {
        dprintf(STDERR_FILENO, "Error : fd_stdin");
        goto error;
    }

    // Extraire le nom du fichier après " < "
    input_file = strtok(redir_pos + 3, " ");
    if (input_file == NULL) {
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

    // Vérifier si la commande est "exit"
    if (strncmp(line, "exit", 4) == 0 && (line[4] == '\0' || is_space(line[4]))) {
        fclose(stdin); // Fermer stdin si redirigé
        exit(0);       // Terminer proprement le shell
    }

    // Fork pour exécuter la commande redirigée
    pid = fork();
    if (pid == -1) {
        dprintf(STDERR_FILENO, "Error forking process: %s\n", strerror(errno));
        goto error;
    }

    if (pid == 0) {
        char *args[128]; // Tableau pour stocker les arguments
        char *token = strtok(line, " ");
        int i = 0;

        while (token != NULL && i < 127) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Exécuter la commande avec execvp
        if (execvp(args[0], args) == -1) {
            dprintf(STDERR_FILENO, "Error executing command %s: %s\n", args[0], strerror(errno));
            exit(1); // Quitter si execvp échoue
        }
    } else {
        // Parent : attendre la fin de l'enfant
        if (waitpid(pid, &status, 0) == -1) {
            dprintf(STDERR_FILENO, "Error waiting for child process: %s\n", strerror(errno));
            goto error;
        }

        // Restaurer l'entrée standard pour le shell
        if (dup2(fd_stdin, STDIN_FILENO) == -1) {
            dprintf(STDERR_FILENO, "Error restoring stdin: %s\n", strerror(errno));
            goto error;
        }
        close(fd_stdin); // Fermer le descripteur sauvegardé

        // Retourner le statut de la commande exécutée
        return WEXITSTATUS(status);
    }

error:
    if (fd_pipe != -1) {
        close(fd_pipe);
    }
    if (fd_stdin != -1) {
        dup2(fd_stdin, STDIN_FILENO);
        close(fd_stdin);
    }

    return 1;
}

int redir_in(command *cmd,redirection *redir)
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
        clearCommands(cmd);
        return 1;
    } 

    if (dup2(fd, redir->fd) == -1) {
        dprintf(STDERR_FILENO, "Error redirecting stdin: %s\n", strerror(errno));
        close(fd);
        clearCommands(cmd);
        return 1;
    }
    close(fd);

    /*
    // vérifier si la commande est "exit"
    if(strcmp(cmd->nom, "exit") == 0 && (cmd->ligne[4] == '\0' || is_space(cmd->ligne[4])))
    {
        fclose(stdin);
        exit(0);
    }

    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            dprintf(STDERR_FILENO, "Error: fork failed: %s\n", strerror(errno));
            return 1;
        case 0:
        {
            if(execvp(cmd->nom, cmd->args) == -1)
            {
                dprintf(STDERR_FILENO, "Error: execvp failed: %s\n", strerror(errno));
                exit(1);
            }
        }
        default:
        {
            int status;
            if(waitpid(pid, &status, 0) == -1)
            {
                dprintf(STDERR_FILENO, "Error: waitpid failed: %s\n", strerror(errno));
                return 1;
            }
            if(dup2(STDIN_FILENO, redir->fd) == -1)
            {
                dprintf(STDERR_FILENO, "Error restoring stdin: %s\n", strerror(errno));
                return 1;
            }
            return WEXITSTATUS(status);
        }
    }
    */
    return 0;
}
