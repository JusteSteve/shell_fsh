/**
 * @file external_cmds.c
 * Implémente des fonctions pour gérer les commandes externes.
 */

#include "../../headers/cmd-utils.h"
#include "../../headers/signal.h"

extern volatile sig_atomic_t signal_recu;
extern volatile sig_atomic_t signal_sigint;

int exec_external_cmds(command *cmd)
{
  if (cmd == NULL)
  {
    fprintf(stderr, "[exec_external_cmds]> Invalid or empty command structure\n");
    return 1;
  }

  pid_t child_pid = fork();

  switch (child_pid)
  {
  case -1: // fork a échoué
    perror("[exec_external_cmds]>fork");
    exit(1);

  case 0: // processus enfant
  {
    restaurer_signal();
    cmd->args[cmd->taille] = NULL;
    execvp(cmd->nom, cmd->args);
    dprintf(2, "fsh: command not found: %s\n", cmd->nom);
    exit(1);
  }

  default: // processus parent
  {
    int status;
    waitpid(child_pid, &status, 0);
    if (WIFEXITED(status))
    {
      return WEXITSTATUS(status); // Retourner le statut de sortie du processus enfant
    }
    if (WIFSIGNALED(status))
    {
      signal_recu = 1;
      signal_sigint = (WTERMSIG(status) == SIGINT) ? 1 : 0;
      return 255;
    }
    return 1; // Retourner une valeur d'erreur par défaut
  }
  }
}

// ===*** Fonctions auxiliaires ***===

char **split_cmd(char *line, char *separateur, int flag)
{
  // allouer la mémoire pour stocker les arguments ou les commandes
  char **args = malloc(MAX_CMDS * sizeof(char *));
  if (args == NULL)
  {
    perror("[split_cmd]>malloc");
    exit(1);
  }

  // faire une copie de la ligne pour travailler avec
  char *line_copy = strdup(line);
  if (line_copy == NULL)
  {
    perror("[split_cmd]>strdup");
    free_args(args);
    exit(1);
  }

  int i = 0;
  // séparer la ligne en arguments ou en commandes
  char *tokens = strtok(line_copy, separateur);
  while (tokens != NULL && i < MAX_CMDS - 1)
  {
    // si c'est une commande for, on doit stocker la commande et les arguments dans une seule case
    if (flag && strncmp(tokens, " for", 4) == 0)
    {
      args[i++] = build_for_cmd(tokens, separateur);
    }
    else
    {
      //  stocker l'argument (ou la commande) i dans le tableau
      args[i] = strdup(tokens);
      if (args[i] == NULL)
      {
        perror("[split_cmd]>strdup");
        for (int j = 0; j < i; j++)
        {
          free(args[j]);
        }
        goto error;
      }
      i++;
    }
    // passer au prochain
    tokens = strtok(NULL, separateur);
  }
  args[i] = NULL; // on fini le tableau par NULL
  free(line_copy);
  return args;

error:
  free(args);
  free(line_copy);
  exit(1);
}

char *build_for_cmd(char *tokens, char *separateur)
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
    tokens = strtok(NULL, separateur);
    len += strlen(tokens) + 2; // +2 pour ; et le \0
    tmp = realloc(tmp, len);
    if (tmp == NULL)
    {
      perror("[build_for_cmd]>realloc");
      exit(1);
    }
    strcat(tmp, separateur);
    strcat(tmp, tokens);
  }
  return tmp;
}
