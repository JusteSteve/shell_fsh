#include "../../headers/cmd-utils.h"

int exec_pipeline_cmds(char *line)
{
    // séparer les commandes de la ligne
    char **cmds = split_pipeline_cmd(line);
    if (cmds == NULL)
    {
        return 1;
    }

    // compter le nombre de commandes
    int nb_cmds = 0;
    while (cmds[nb_cmds] != NULL)
    {
        nb_cmds++;
    }

    // créer un tableau de descripteurs de fichiers pour les pipes il est de taille 2 * (nb_cmds - 1)
    int pipe_fds[2 * (nb_cmds - 1)];
    for (int i = 0; i < nb_cmds - 1; i++)
    {
        // créer un pipe pour chaque paire de commandes
        if (pipe(pipe_fds + 2 * i) == -1)
        {
            perror("[exec_pipeline_cmds]>pipe");
            goto error;
        }
    }

    
    int pid;
    int status;
    // exécuter les commandes
    for (int i = 0; i < nb_cmds; i++)
    {
        // créer un processus fils pour chaque commande
        pid = fork();
        switch (pid)
        {

        case 0: // enfant
            if (i > 0)
            {
                // rediriger l'entrée standard vers le descripteur de fichier en lecture du pipe
                if (dup2(pipe_fds[(i - 1) * 2], STDIN_FILENO) == -1)
                {
                    perror("[exec_pipeline_cmds]1>dup2");
                    goto error;
                }
            }

            // rediriger la sortie standard vers le descripteur de fichier en écriture du pipe
            if (i < nb_cmds - 1)
            {
                if (dup2(pipe_fds[i * 2 + 1], STDOUT_FILENO) == -1)
                {
                    perror("[exec_pipeline_cmds]2>dup2");
                    goto error;
                }
            }
            // fermer les descripteurs de fichiers inutiles
            for (int j = 0; j < 2 * (nb_cmds - 1); j++)
            {
                close(pipe_fds[j]);
            }
            // exécuter la commande
            int ret_val = execute_commande(cmds[i]);
            if (ret_val == 1)
            {
                return 1;
            }
            // sortir de la boucle for 
            exit(ret_val);

        case -1:
            perror("[exec_pipeline_cmds]>fork");
            goto error;

        default: // parent
            break;
        }
    }

    // fermer les descripteurs de fichiers inutiles
    for (int i = 0; i < 2 * (nb_cmds - 1); i++)
    {
        close(pipe_fds[i]);
    }

    // attendre la fin de tous les processus fils
    for (int i = 0; i < 2 * (nb_cmds - 1); i++)
    {
        wait(&status);
    }

    free(cmds);
    return WEXITSTATUS(status);
error:
    free(cmds);
    return 1;
}

// ***=== Fonctions auxiliaires ===***

char **split_pipeline_cmd(char *line)
{
    // allouer la mémoire pour stocker les commandes
    char **cmds = malloc(MAX_CMDS * sizeof(char *));
    if (cmds == NULL)
    {
        perror("[split_pipeline_cmd]>malloc");
        exit(1);
    }

    // faire une copie de la ligne pour travailler avec
    char *line_copy = strdup(line);
    if (line_copy == NULL)
    {
        perror("[split_pipeline_cmd]>strdup");
        free(cmds);
        exit(1);
    }

    int i = 0;
    char *token = strtok(line_copy, "|");
    while (token != NULL && i < MAX_CMDS - 1)
    {
        // si c'est une commande for, on doit stocker la commande et les arguments dans une seule case
        if (strncmp(token, " for", 4) == 0)
        {
            cmds[i++] = build_for_pipe_cmd(token);
        }
        else
        {
            // stocker la commande i dans le tableau
            cmds[i] = strdup(token);
            if (cmds[i] == NULL)
            {
                perror("[split_pipeline_cmd]>strdup");
                free(line_copy);
                for (int j = 0; j < i; j++)
                {
                    free(cmds[j]);
                }
                goto error;
            }
            i++;
        }
        // passer à la commande suivante
        token = strtok(NULL, "|");
    }
    cmds[i] = NULL; // on finit le tableau par NULL
    free(line_copy);
    return cmds;
error:
    free(cmds);
    exit(1);
}

char *build_for_pipe_cmd(char *tokens)
{
    size_t len = strlen(tokens) + 1;
    char *tmp = malloc(len);
    if (tmp == NULL)
    {
        perror("[build_for_cmd]>malloc");
        exit(1);
    }
    strcpy(tmp, tokens);
    // reconstruire la commande for ... { ... }
    while (strstr(tokens, "}") == NULL)
    {
        tokens = strtok(NULL, "|");
        len += strlen(tokens) + 2; // +2 pour ; et le \0
        tmp = realloc(tmp, len);
        if (tmp == NULL)
        {
            perror("[build_for_cmd]>realloc");
            exit(1);
        }
        strcat(tmp, "|");
        strcat(tmp, tokens);
    }
    return tmp;
}