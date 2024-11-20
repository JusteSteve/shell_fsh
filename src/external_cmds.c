/**
 * @file external_cmds.c
 * Implémente des fonctions pour gérer les commandes externes.
 */

#include "../headers/external_cmds.h"

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

int exec_external_cmds(char **args)
{
  pid_t child_pid = fork();

  switch (child_pid)
  {
  case -1: // fork a échoué
    perror("[exec_external_cmds]>fork");
    exit(1);

  case 0: // processus enfant
  {
    int nb_args = get_nb_args(args);
    args = realloc(args, (nb_args + 1) * sizeof(char *));
    args[nb_args] = NULL;
    execvp(args[0], args);
    dprintf(2, "fsh: command not found: %s\n", args[0]);
    return 1;
  }

  default: // processus parent
    wait(NULL);
    return 0;
  }
}

// ===*** Fonctions auxiliaires ***===

void free_args(char **args)
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

int is_line_empty(char *line)
{
  for (int i = 0; line[i] != '\0'; i++)
  {
    if (line[i] != ' ' && line[i] != '\t')
    {
      return 0;
    }
  }
  return 1;
}
