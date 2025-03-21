/**
 * @file cmd_utils.c
 * Contient les implémentations des fonctions utilitaires
 * pour exécuter des commandes.
 */

#include "../../headers/internal_cmds.h"
#include "../../headers/redir.h"
#include "../../headers/signal.h"

int prev_status; // pour stocker le status précédent
extern volatile sig_atomic_t signal_sigint;
extern volatile sig_atomic_t signal_recu;

int execute_commande(char *line)
{
  int return_value;
  // créer une structure de commande à partir de la ligne de commande
  command *cmd = fillCommand(line);
  if (cmd == NULL)
  {
    return 1;
  }
  if (strcmp(cmd->nom, "for") == 0)
  {
    return_value = exec_for_cmds(cmd);
  }
  else if (strcmp(cmd->nom, "if") == 0)
  {
    cmd_if *cmd_if = remplir_cmd_if(cmd);
    if (cmd_if == NULL)
    {
      clearCommands(cmd);
      return 1;
    }
    return_value = exec_cmd_if(cmd_if);
  }
  else if (strchr(line, ';') != NULL)
  {
    return_value = exec_structured_cmds(line);
  }
  else if (strstr(line, " | ") != NULL)
  {
    return_value = exec_pipeline_cmds(line);
  }
  else if (contient_redirection(line))
  {
    return_value = exec_cmd_redirection(cmd);
  }

  else if (is_internal_cmd(cmd->nom))
  {
    return_value = exec_internal_cmds(line);
  }
  else
  {
    return_value = exec_external_cmds(cmd);
  }
  clearCommands(cmd);

  prev_status = return_value;
  return return_value;
}

int exec_internal_cmds(char *line)
{
  command *cmd = fillCommand(line);
  if (cmd == NULL)
  {
    return 1;
  }
  // Si l'utilisateur a tapé "exit", on arrête la boucle
  if (strncmp(cmd->nom, "exit", 4) == 0)
  {
    // si il ya trop d'arguments, on affiche un message d'erreur
    if (cmd->taille > 2)
    {
      fprintf(stderr, "exit: too many arguments\n");
      goto error;
    }
    char *val = NULL;
    if (cmd->args[1] != NULL)
    {
      val = cmd->args[1];
    }
    cmd_exit(val);
  }
  // Si l'utilisateur a tapé "pwd", on cmd_pwd de pwd.c
  if (strncmp(cmd->nom, "pwd", 3) == 0)
  {
    // si il ya trop d'arguments, on affiche un message d'erreur
    if (cmd->taille > 1)
    {
      fprintf(stderr, "pwd: %s : invalid argument\n", cmd->args[1]);
      goto error;
    }
    prev_status = cmd_pwd(); // status de la commande -> prev_status qui va être utilisé dans exit.c
    clearCommands(cmd);
    return prev_status;
  }
  // Si l'utilisateur a tapé "cd", on cmd_cd de cd.c
  if (strncmp(cmd->nom, "cd", 2) == 0)
  {
    // si il ya trop d'arguments, on affiche un message d'erreur
    if (cmd->taille > 2)
    {
      fprintf(stderr, "cd: too many arguments\n");
      goto error;
    }
    char *path = NULL;
    if (cmd->args[1] != NULL)
    {
      path = cmd->args[1];
    }
    prev_status = cmd_cd(path); // status de la commande -> prev_status qui va être utilisé dans exit.c
    clearCommands(cmd);
    return prev_status;
  }
  // Si l'utilisateur a tapé "ftype", on cmd_ftype de ftype.c
  if (strncmp(cmd->nom, "ftype", 5) == 0)
  {
    char *ref = NULL;
    ref = cmd->args[1];
    if (ref != NULL)
    {
      prev_status = cmd_ftype(ref);
      clearCommands(cmd);
      return prev_status;
    }
    else
    {
      fprintf(stderr, "ftype: missing reference argument\n");
      goto error;
    }
  }
  clearCommands(cmd);
  return 1; // si la commande n'est pas reconnue
error:
  clearCommands(cmd);
  return 1;
}

int exec_structured_cmds(char *line)
{
  int return_value;
  char **cmds_tab = split_cmd(line, ";", 1);
  if (cmds_tab == NULL)
  {
    return 1;
  }
  int cmd_i = 0;
  // exécuter toutes les commandes
  while (cmds_tab[cmd_i] != NULL)
  {
    signal_recu = 0;
    if (signal_sigint)
    {
      break;
    }
    return_value = execute_commande(cmds_tab[cmd_i]);
    prev_status = return_value;
    cmd_i++;
  }
  free_args(cmds_tab);
  return return_value;
}

int exec_for_cmds(command *cmd)
{
  int return_value;
  //  créer une structure de commande for à partir de la cmd
  comFor *command = fillCommandFor(cmd);
  if (command == NULL)
  {
    return 1;
  }
  // exécuter la commande for
  return_value = parcoursFor(command);
  if (return_value != 0)
  {
    clearCommandFor(command);
    return return_value;
  }

  // vérifier si on est dans le cas for ... { ... } ; cmd
  char *tmp = strchr(cmd->ligne, '}');
  char *point_virgule = strchr(tmp, ';');
  if (point_virgule != NULL)
  {
    // se débarasser de tout ce qui est avant le ;
    char *new_line = strdup(point_virgule + 1);
    if (new_line == NULL)
    {
      perror("[exec_for_cmds]>strdup");
      clearCommandFor(command);
      return 1;
    }
    return_value = execute_commande(new_line);
    free(new_line);
  }
  clearCommandFor(command);
  return return_value;
}

// ***=== Fonctions auxiliaires ===***

int is_internal_cmd(char *cmd)
{
  return (!strcmp(cmd, "cd") || !strcmp(cmd, "pwd") || !strcmp(cmd, "ftype") || !strcmp(cmd, "exit"));
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
