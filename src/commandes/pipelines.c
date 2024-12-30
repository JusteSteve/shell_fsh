#include "../../headers/cmd-utils.h"

int exec_pipeline_cmds(char *line)
{
    // séparer les commandes de la ligne
    char **cmds = split_cmd(line, "|", 1);
    if (cmds == NULL)
    {
        perror("[exec_pipeline_cmds]>split_cmd");
        return 1;
    }

    // compter le nombre de commandes
    int nb_cmds = get_nb_args(cmds);

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

    //  exécuter les commandes
    for (int i = 0; i < nb_cmds; i++)
    {
        // créer un processus fils pour chaque commande
        pid_t pid = fork();
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
            if (execute_commande(cmds[i]) == 1)
            {
                exit(1);
            }
            exit(0);

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
        wait(NULL);
    }

    free(cmds);
    return 0;

error:
    free(cmds);
    return 1;
}