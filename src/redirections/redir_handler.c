/**
 * @file redir.handler
 * Fichier qui permet d'appeler les fonctions de redirections
 */

#include "../../headers/redir.h"
#include "../../headers/internal_cmds.h"

int exec_cmd_redirection(command *cmd)
{
    //! FIXME : à régler car ne sert que pour le test exit < fichier 3
    if (strncmp(cmd->nom, "exit", 4) == 0)
    {
        fclose(stdin); // Fermer stdin si redirigé
        exit(0);
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("[exec_cmd_redirection]>fork");
        exit(1);
    }
    if (pid == 0)
    {
        redir_handler(cmd);

        execvp(cmd->nom, cmd->args);
        dprintf(STDERR_FILENO, "Error: execvp failed: %s\n", strerror(errno));
        clearCommands(cmd);
        exit(1);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0); // Attendre la fin du processus enfant et récupérer son statut
        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status); // Retourner le statut de sortie du processus enfant
        }
        return 1; // Retourner une valeur d'erreur par défaut
    }
}

int redir_handler(command *cmd)
{
    int return_value = 0;
    int i = 0;
    while (cmd->args[i] != NULL)
    {
        char *token = cmd->args[i];
        if (is_redirection(token) != -1)
        {
            if (cmd->args[i + 1] == NULL)
            {
                dprintf(STDERR_FILENO, "Error: no file specified\n");
                clearCommands(cmd);
                return 1;
            }
            // remplir les champs de redirection
            redirection *redir = remplir_redirection(cmd->args[i + 1], token);
            if (redir == NULL)
            {
                clearCommands(cmd);
                return 1;
            }
            // exécuter la redirection
            return_value = (redir->type == REDIR_STDIN) ? redir_in(redir) : redir_out(cmd, redir);

            // suppression des tokens de redirection du tableau de commandes
            int j = i;
            while (cmd->args[j + 2] != NULL)
            {
                cmd->args[j] = cmd->args[j + 2];
                j++;
            }
            cmd->args[j] = NULL;
        }
        else
        {
            i++;
        }
    }
    return return_value;
}

// ****==== Fonctions auxiliaires ====****

redirection *initialiser_redirection()
{
    redirection *redir = malloc(sizeof(redirection));
    if (redir == NULL)
    {
        perror("[initialiser_redirection]>malloc");
        return NULL;
    }
    redir->type = -1;
    redir->fichier = NULL;
    return redir;
}

redirection *remplir_redirection(char *fichier, char *token)
{
    redirection *redir = initialiser_redirection();
    if (redir == NULL)
    {
        return NULL;
    }
    redir->fichier = strdup(fichier);
    if (redir->fichier == NULL)
    {
        perror("[remplir_redirection]>strdup");
        free_redirection(redir);
        return NULL;
    }
    redir->type = is_redirection(token);
    if (redir->type == REDIR_STDIN)
    {
        redir->fd = STDIN_FILENO;
    }
    else if (redir->type == REDIR_STDOUT || redir->type == REDIR_STDOUT_TRUNC || redir->type == REDIR_STDOUT_APPEND)
    {
        redir->fd = STDOUT_FILENO;
    }
    else
    {
        redir->fd = STDERR_FILENO;
    }
    return redir;
}

int is_redirection(char *line)
{
    if (strcmp(line, "<") == 0)
    {
        return REDIR_STDIN;
    }
    else if (strcmp(line, ">") == 0)
    {
        return REDIR_STDOUT;
    }
    else if (strcmp(line, ">|") == 0)
    {
        return REDIR_STDOUT_TRUNC;
    }
    else if (strcmp(line, ">>") == 0)
    {
        return REDIR_STDOUT_APPEND;
    }
    else if (strcmp(line, "2>") == 0)
    {
        return REDIR_STDERR;
    }
    else if (strcmp(line, "2>|") == 0)
    {
        return REDIR_STDERR_TRUNC;
    }
    else if (strcmp(line, "2>>") == 0)
    {
        return REDIR_STDERR_APPEND;
    }
    return -1;
}

int extraire_flags(redirection *redir)
{
    switch (redir->type)
    {
    case REDIR_STDIN:
        return O_RDONLY;
    case REDIR_STDOUT:
        return O_WRONLY | O_CREAT | O_EXCL;
    case REDIR_STDOUT_TRUNC:
        return O_WRONLY | O_CREAT | O_TRUNC;
    case REDIR_STDOUT_APPEND:
        return O_WRONLY | O_CREAT | O_APPEND;
    case REDIR_STDERR:
        return O_WRONLY | O_CREAT | O_EXCL;
    case REDIR_STDERR_TRUNC:
        return O_WRONLY | O_CREAT | O_TRUNC;
    case REDIR_STDERR_APPEND:
        return O_WRONLY | O_CREAT | O_APPEND;
    default:
        return -1;
    }
}

int contient_redirection(char *line)
{
    if (strstr(line, " > ") != NULL || strstr(line, " >| ") != NULL ||
        strstr(line, " < ") != NULL || strstr(line, " >> ") != NULL ||
        strstr(line, " 2> ") != NULL || strstr(line, " 2>| ") != NULL ||
        strstr(line, " 2>> ") != NULL)
    {
        return 1;
    }
    return 0;
}

void free_redirection(redirection *redir)
{
    free(redir->fichier);
    free(redir);
}
