/**
 * @file redir.handler
 * Fichier qui permet d'appeler les fonctions de redirections
 */

#include "../../headers/redir.h"

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
            if (redir->type == REDIR_STDIN)
            {
                return_value = redir_in(cmd, redir);
            }
            else
            {
                return_value = redir_out(cmd, redir);
            }

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

redirection *initialiser_redirection()
{
    redirection *redir = malloc(sizeof(redirection));
    if (redir == NULL)
    {
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

redirection **extraire_redirections(command *cmd)
{
    redirection **redir_tab = malloc(sizeof(redirection *) * (nb_redirections(cmd) + 1));
    if (redir_tab == NULL)
    {
        perror("[extraire_redirections]>malloc");
        return NULL;
    }
    int i = 0;
    while (cmd->args[i] != NULL)
    {
        char *token = cmd->args[i];
        if (is_redirection(token) != -1)
        {
            printf("token: %s\n", token);
            redirection *redir = initialiser_redirection();
            if (redir == NULL)
            {
                perror("[extraire_redirections]>initialiser_redirection");
                goto error;
            }
            redir->fichier = strdup(cmd->args[i + 1]);
            printf("fichier: %s\n", redir->fichier);
            if (redir->fichier == NULL)
            {
                perror("[extraire_redirections]>strdup");
                return NULL;
            }
            redir->type = is_redirection(token);
            printf("type: %d\n", redir->type);
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
            printf("fd: %d\n", redir->fd);
            redir_tab[i] = redir;
        }
        i++;
    }
    redir_tab[i] = NULL;
    return redir_tab;
error:
    free_redir_tab(redir_tab);
    return NULL;
}

int exec_redirections(redirection **redirections)
{
    int fd_file = -1;
    int flags = -1;

    for (int i = 0; redirections[i] != NULL; i++)
    {
        flags = extraire_flags(redirections[i]);
        fd_file = open(redirections[i]->fichier, flags, 0644);
        if (fd_file == -1)
        {
            perror("[exec_redirections]>open");
            return 1;
        }
        if (dup2(fd_file, redirections[i]->fd) == -1)
        {
            perror("[exec_redirections]>dup2");
            close(fd_file);
            return 1;
        }
        close(fd_file);
    }
    return 0;
}

int nb_redirections(command *cmd)
{
    int nb = 0;
    for (int i = 0; i < cmd->taille; i++)
    {
        if (is_redirection(cmd->args[i]) != -1)
        {
            nb++;
        }
    }
    return nb;
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

/*
int redir_handler(command *cmd)
{
    redirection **redir_tab = extraire_redirections(cmd);
    switch (type)
    {
    case REDIR_STDIN:
        // printf ("redir stdin\n");
        return redir_stdin(line);
    case REDIR_STDOUT:
        // printf ("redir stdout\n");
        return redir_stdout(line);
    case REDIR_STDOUT_TRUNC:
        // printf ("redir stdout trunc\n");
        return redir_stdout_trunc(line);
    case REDIR_STDOUT_APPEND:
        // printf ("redir stdout append\n");
        return redir_stdout_append(line);
    case REDIR_STDERR:
        // printf ("redir stderr\n");
        return redir_stderr(line);
    case REDIR_STDERR_TRUNC:
        // printf ("redir stderr trunc\n");
        return redir_stderr_trunc(line);
    case REDIR_STDERR_APPEND:
        // printf ("redir stderr append\n");
        return redir_stderr_append(line);
    default:
        // ne devrait pas être appelé mais ça devrait pas arriver normalement
        dprintf(STDERR_FILENO, "Error: invalid redirection type\n");
        return 1;
    }
}
*/

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

void free_redir_tab(redirection **redir_tab)
{
    for (int i = 0; redir_tab[i] != NULL; i++)
    {
        free(redir_tab[i]->fichier);
        free(redir_tab[i]);
    }
    free(redir_tab);
}

void free_redirection(redirection *redir)
{
    free(redir->fichier);
    free(redir);
}
