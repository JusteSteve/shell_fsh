/**
 * @file cmd_utils.c
 * Contient les implémentations des fonctions utilitaires
 * pour exécuter des commandes.
 */

#include "../../headers/internal_cmds.h"
#include "../../headers/cmd-utils.h"

int prev_status; // pour stocker le status précédent

int execute_commande(char *line)
{
  int return_value;
  // vérifier si la contient un ;
  if (strchr(line, ';') != NULL)
  {
    // exécuter la commande structurée
    return_value = exec_structured_cmds(line);
    prev_status = return_value;
    return return_value;
  }
  // créer une structure de commande à partir de la ligne de commande
  command *cmd = fillCommand(line);
  if (cmd == NULL)
  {
    return 1;
  }
  // vérifier si la commande est interne
  if (is_internal_cmd(cmd->nom))
  {
    return_value = exec_internal_cmds(line);
  }
  // vérifier si la commande est un for
  else if (strcmp("for", cmd->nom) == 0)
  {
    return_value = exec_for_cmds(cmd);
  }
  else
  {
    return_value = exec_external_cmds(cmd);
  }
  prev_status = return_value;
  clearCommands(cmd);
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
    char *val = NULL;
    if (cmd->args[1] != NULL)
    {
      val = cmd->args[1];
    }
    cmd_exit(val);
    // Pas besoin de return car exit termine le programme (NIC SUPPRIME MOI SI TU ME VOIS)
  }
  // Si l'utilisateur a tapé "pwd", on cmd_pwd de pwd.c
  if (strncmp(cmd->nom, "pwd", 3) == 0)
  {
    prev_status = cmd_pwd(); // status de la commande -> prev_status qui va être utilisé dans exit.c
    return prev_status;
  }
  // Si l'utilisateur a tapé "cd", on cmd_cd de cd.c
  if (strncmp(cmd->nom, "cd", 2) == 0)
  {
    char *path = NULL;
    if (cmd->args[1] != NULL)
    {
      path = cmd->args[1];
    }
    prev_status = cmd_cd(path); // status de la commande -> prev_status qui va être utilisé dans exit.c
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
      return prev_status;
    }
    else
    { // faudrait que je fasse la gestion d'erreur dans les fichiers .c respectifs
      fprintf(stderr, "ftype: missing reference argument\n");
      return 1;
      // si on gère l'erreur dans main, ça permet justement de décider si on veut continuer
      // malgré l'erreur en printant un msg, alors que dans .c, on va juste faire return 1
      // donc ça va retourner !prev_status, à voir pour l'instant donc.
    }
  }
  return 1; // Par défaut, on continue la boucle
}

int exec_structured_cmds(char *line)
{
  int return_value;
  // diviser la ligne en tableau de commandes simples
  char **cmds_tab = split_cmd(line, 1);
  if (cmds_tab == NULL)
  {
    return 1;
  }
  int cmd_i = 0;
  // exécuter toutes les commandes
  while (cmds_tab[cmd_i] != NULL)
  {
    return_value = execute_commande(cmds_tab[cmd_i]);

    prev_status = return_value;
    if (return_value == 1)
    {
      return 1;
    }
    sleep(1);
    cmd_i++;
  }
  free_args(cmds_tab);
  return return_value;
}

int exec_for_cmds(command *cmd)
{
  // créer une structure de commande for à partir de la cmd
  comFor *command = fillCommandFor(cmd);
  if (command == NULL)
  {
    return 1;
  }
  // exécuter la commande for
  if (parcoursFor(command) != 0)
  {
    clearCommandFor(command);
    return 1;
  }
  clearCommandFor(command);
  return 0;
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
