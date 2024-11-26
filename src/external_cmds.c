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

// FIXME: SUPPRIMER CAR ON UTILISE exec_external_cmds A LA PLACE
int exec_external_cmdsFor(comFor *cmd)
{
  if (cmd == NULL || cmd->ligne == NULL || is_line_empty(cmd->ligne))
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
    execlp(cmd->command, cmd->path, NULL);
    dprintf(2, "fsh: command not found: %s\n", cmd->command);
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

void free_args(char **args)
{
  if (args == NULL)
    return;
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
