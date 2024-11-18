#include "../headers/external_cmds.h"

char **split_cmd(char *cmd)
{
    char **args = malloc(MAX_CMDS * sizeof(char *));
    if (args == NULL)
    {
        perror("[split_cmd]>malloc");
        exit(1);
    }

    char *cmd_copy = strdup(cmd);
    if (cmd_copy == NULL)
    {
        perror("[split_cmd]>strdup");
        free(args);
        exit(1);
    }

    int i = 0;
    char *words = strtok(cmd_copy, " ");
    while (words != NULL && i < MAX_CMDS - 1)
    {
        args[i] = strdup(words);
        if (args[i] == NULL)
        {
            perror("[split_cmd]>strdup");
            free(cmd_copy);
            for (int j = 0; j < i; i++)
            {
                free(args[i]);
            }
            free(args);
            exit(1);
        }
        i++;
        words = strtok(NULL, " ");
    }
    args[i] = NULL; // on fini le tableau par NULL
    free(cmd_copy);
    return args;
}

void free_words(char **args)
{
    for (int i = 0; args[i] != NULL; i++)
    {
        free(args[i]);
    }
    free(args);
}

int get_nb_args(char **args)
{
    int i = 0;
    while (args[i] != NULL)
    {
        i++;
    }
    return i;
}

int exec_external_cmds(char **args)
{

    pid_t child_pid = fork();

    switch (child_pid)
    {
    case 0: // processus enfant
    {
        int nb_args = get_nb_args(args);
        char **a = malloc(sizeof(char *) * (nb_args + 1));
        for (int i = 0; i < nb_args; i++)
        {
            a[i] = args[i];
        }
        a[nb_args] = NULL;
        execvp(args[0], a);
        dprintf(2, "fsh: command not found: %s\n", args[0]);
        return 1;
    }

    default: // processus parent
        wait(NULL);
        break;
    }

    return 0;
}