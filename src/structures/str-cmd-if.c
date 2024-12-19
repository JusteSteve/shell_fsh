/**
 * @file str-cmd-if.c
 * @brief Fichier contenant les fonctions pour les commandes structurées de type if.
 */
#include "../../headers/cmd-utils.h"

cmd_if *initialiser_cmd_if()
{
    cmd_if *cmd_if = malloc(sizeof(*cmd_if));
    if (cmd_if == NULL)
    {
        goto error;
    }
    cmd_if->ligne = NULL;
    cmd_if->test = NULL;
    cmd_if->cmd_if = NULL;
    cmd_if->cmd_else = NULL;
    return cmd_if;
error:
    free_cmd_if(cmd_if);
    return NULL;
}

cmd_if *remplir_cmd_if(command *cmd)
{
    cmd_if *cmd_if = initialiser_cmd_if();
    if (cmd_if == NULL)
    {
        goto error;
    }
    cmd_if->ligne = (cmd->ligne);
    if (cmd_if->ligne == NULL)
    {
        goto error;
    }

    int i = 1;
    int debut, fin;

    // ** extraire la commande test **
    if (strcmp(cmd->args[i], "test") == 0)
    {
        cmd_if->nom = strdup("test");
        if (cmd_if->nom == NULL)
        {
            goto error;
        }
        debut = ++i; // on passe le test
        while (strcmp(cmd->args[i], "{") != 0)
        {
            i++;
        }
        fin = i - 1; // on ne prend pas le {
    }
    else if (strcmp(cmd->args[i], "[") == 0)
    {
        debut = ++i; // on passe le [
        while (strcmp(cmd->args[i], "]") != 0)
        {
            if (cmd->args[i] == NULL)
            {
                fprintf(stderr, "Erreur de syntaxe: ] attendu\n");
                goto error;
            }
            i++;
        }
        fin = i - 1; // on ne prend pas le ]
        i++;
    }

    int test_taille = 0;
    for (int j = debut; j <= fin; j++)
    {
        test_taille += strlen(cmd->args[j]) + 1; // +1 pour l'espace
    }
    cmd_if->test = malloc(test_taille + 1); // +1 for the null terminator
    if (cmd_if->test == NULL)
    {
        goto error;
    }

    cmd_if->test[0] = '\0';
    for (int j = debut; j <= fin; j++)
    {
        strcat(cmd_if->test, cmd->args[j]);
        strcat(cmd_if->test, " ");
    }

    // ** extraire la commande if **
    debut = ++i; // on passe le {
    int nb_accolades = 1;
    while (nb_accolades != 0 && cmd->args[i] != NULL)
    {
        if (strcmp(cmd->args[i], "{") == 0)
        {
            nb_accolades++;
        }
        if (strcmp(cmd->args[i], "}") == 0)
        {
            nb_accolades--;
        }
        i++;
    }

    fin = i - 1; // on ne prend pas le }

    int cmd_if_taille = 0;
    for (int j = debut; j <= fin; j++)
    {
        cmd_if_taille += strlen(cmd->args[j]) + 1; // +1 pour l'espace
    }
    cmd_if->cmd_if = malloc(cmd_if_taille + 1); // +1 pour le \0
    if (cmd_if->cmd_if == NULL)
    {
        goto error;
    }

    cmd_if->cmd_if[0] = '\0';
    for (int j = debut; j < fin; j++)
    {
        strcat(cmd_if->cmd_if, cmd->args[j]);
        strcat(cmd_if->cmd_if, " ");
    }

    if (cmd->args[i] == NULL)
    {
        return cmd_if;
    }

    // ** extraire la commande else **
    if (strcmp(cmd->args[i], "else") != 0)
    {
        return cmd_if;
    }
    i++; // on passe le else
    if (strcmp(cmd->args[i], "{") != 0)
    {
        fprintf(stderr, "Erreur de syntaxe: { attendu après else\n");
        goto error;
    }
    debut = ++i; // on passe le {
    nb_accolades = 1;
    while (nb_accolades > 0 && cmd->args[i] != NULL)
    {
        if (strcmp(cmd->args[i], "{") == 0)
        {
            nb_accolades++;
        }
        if (strcmp(cmd->args[i], "}") == 0)
        {
            nb_accolades--;
        }
        i++;
    }
    fin = i - 1; // on ne prend pas le }

    int else_taille = 0;
    for (int j = debut; j <= fin; j++)
    {
        else_taille += strlen(cmd->args[j]) + 1; // +1 pour l'espace
    }
    cmd_if->cmd_else = malloc(else_taille + 1); // +1 pour le \0
    if (cmd_if->cmd_else == NULL)
    {
        goto error;
    }
    cmd_if->cmd_else[0] = '\0';
    for (int j = debut; j < fin; j++)
    {
        strcat(cmd_if->cmd_else, cmd->args[j]);
        strcat(cmd_if->cmd_else, " ");
    }
    return cmd_if;

error:
    free_cmd_if(cmd_if);
    return NULL;
}

int exec_cmd_if(cmd_if *cmd_if)
{
    int return_value;
    // exécuter la commande test
    return_value = exec_test(cmd_if->test);
    // exécuter la commande if
    if (return_value == 0)
    {
        return_value = execute_commande(cmd_if->cmd_if);
    }
    // exécuter la commande else
    else if (cmd_if->cmd_else != NULL && return_value == 1)
    {
        return_value = execute_commande(cmd_if->cmd_else);
    }
    free_cmd_if(cmd_if);
    return (return_value == 1 ? 0 : return_value);
}

int exec_test(char *test)
{
    char *args[256];
    int i = 0;

    args[i++] = "test";
    char *tokens = strtok(test, " ");
    while (tokens != NULL)
    {
        args[i++] = tokens;
        tokens = strtok(NULL, " ");
    }
    args[i] = NULL;

    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("[exec_test]>fork");
        return 1;
    case 0:
        execvp("test", args);
        perror("[exec_test]>execvp");
        exit(1);
    default:
    {
        int status;
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("[exec_test]>waitpid");
            return 1;
        }
        return WEXITSTATUS(status);
    }
    }
}

void free_cmd_if(cmd_if *cmd_if)
{
    if (cmd_if->test != NULL)
    {
        free(cmd_if->test);
    }
    if (cmd_if->cmd_if != NULL)
    {
        free(cmd_if->cmd_if);
    }
    if (cmd_if->cmd_else != NULL)
    {
        free(cmd_if->cmd_else);
    }
    free(cmd_if);
}