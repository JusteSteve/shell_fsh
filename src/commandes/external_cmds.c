/**
 * @file external_cmds.c
 * Implémente des fonctions pour gérer les commandes externes.
 */

#include "../../headers/cmd-utils.h"

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
    cmd->args[cmd->taille] = NULL; // on fini le tableau par NULL
    execvp(cmd->nom, cmd->args);
    dprintf(2, "fsh: command not found: %s\n", cmd->nom);
    exit(1); // si execvp échoue, on sort avec un 1
  }

  default: // processus parent
  {
    int status;
    waitpid(child_pid, &status, 0); // Attendre la fin du processus enfant et récupérer son statut
    if (WIFEXITED(status))
    {
      return WEXITSTATUS(status); // Retourner le statut de sortie du processus enfant
    }
    return 1; // Retourner une valeur d'erreur par défaut
  }
  }
}

// ===*** Fonctions auxiliaires ***===

char **split_cmd(char *line)
{
  // allouer la mémoire pour stocker les arguments
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
    goto error;
  }

  // séparer la ligne en arguments
  int i = 0;
  char *words = strtok(line_copy, " ");
  while (words != NULL && i < MAX_CMDS - 1)
  {
    // stocker l'argument i dans le tableau
    args[i] = strdup(words);
    if (args[i] == NULL)
    {
      perror("[split_cmd]>strdup");
      free(line_copy);
      for (int j = 0; j < i; i++)
      {
        free(args[i]);
      }
      goto error;
    }
    i++;
    // passer au prochain argument
    words = strtok(NULL, " ");
  }
  args[i] = NULL; // on fini le tableau par NULL
  free(line_copy);
  return args;

error:
  free(args);
  exit(1);
}

command **split_structured_cmd(char *line)
{

  // allouer mémoire pour stocker les commandes
  command **commandes = malloc(MAX_CMDS * sizeof(command *));
  if (commandes == NULL)
  {
    perror("[split_structured_cmd]>malloc");
    exit(1);
  }

  char *line_copy = strdup(line);
  if (line_copy == NULL)
  {
    perror("[split_structured_cmd]>strdup");
    goto error;
  }

  int i = 0;
  char *cmd = strtok(line_copy, ";");
  while (cmd != NULL && i < MAX_CMDS - 1)
  {
    // stocker la commande i dans le tableau de commandes
    commandes[i] = fillCommand(cmd);
    if (commandes[i] == NULL)
    {
      fprintf(stderr, "[split_structured_cmd]> Invalid command structure\n");
      free(line_copy);
      for (int j = 0; j < i; j++)
      {
        clearCommands(commandes[j]);
      }
      goto error;
    }
    i++;
    // passer à la prochaine commande
    cmd = strtok(NULL, ";");
  }
  commandes[i] = NULL; // on fini le tableau par NULL
  free(line_copy);
  return commandes;

error:
  free(commandes);
  exit(1);
}
