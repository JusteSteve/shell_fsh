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
    printf("Commande structurée\n");
    // diviser la ligne en commandes simples
    command **cmds_tab = split_structured_cmd(line);
    if (cmds_tab == NULL)
    {
      return 1;
    }
    printf("Nombre de commandes: %d\n", get_nb_args((char **)cmds_tab));
    // exécuter chaque commande
    int cmd_index = 0;
    while (cmds_tab[cmd_index] != NULL)
    {
      char *ligne_de_commande = cmds_tab[cmd_index]->ligne;
      printf("Commande simple: %s\n", ligne_de_commande);
      return_value = execute_commande(ligne_de_commande);
      prev_status = return_value;
      if (return_value == 1) 
      {
        return 1;
      }
      
      clearCommands(cmds_tab[cmd_index]);
      cmd_index++;
    }
    free(cmds_tab);
    //free_cmds_tab(cmds_tab);
    return return_value;
  }

  // créer une structure de commande à partir de la ligne de commande
  command *cmd = fillCommand(line);

  if (strcmp("for", cmd->nom) == 0)
  {
    // créer une structure de commande for à partir de la cmd
    comFor *command = fillCommandFor(cmd);
    if (command == NULL)
    {
      return 1;
    }
    if (parcoursFor(command) != 0)
    {
      clearCommandFor(command);
      return 1;
    }
    clearCommandFor(command);
    return 0;
  }
  else if (is_internal_cmd(cmd->nom))
  {
    return_value = exec_internal_cmds(cmd->ligne);
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
  int len = strlen(line);
  if (len < 0)
  {
    fprintf(stderr, "Erreur de lecture\n");
    ;
    return 1;
  }

  // Retirer le caractère '\n' à la fin de la ligne
  if (line[len - 1] == '\n')
  {
    line[len - 1] = '\0'; // Supprime le '\n'
  }

  // Si l'utilisateur a tapé "exit", on arrête la boucle
  if (strncmp(line, "exit", 4) == 0)
  {
    char *val = NULL;
    if (strlen(line) > 5)
    {
      val = line + 5; // "exit " donc offset de 5 comme pour "cd " au final
    }
    cmd_exit(val);
    // Pas besoin de return car exit termine le programme (NIC SUPPRIME MOI SI TU ME VOIS)
  }

  // Si l'utilisateur a tapé "pwd", on cmd_pwd de pwd.c
  if (strncmp(line, "pwd", 3) == 0)
  {
    prev_status = cmd_pwd(); // status de la commande -> prev_status qui va être utilisé dans exit.c
    return prev_status;
  }

  // Si l'utilisateur a tapé "cd", on cmd_cd de cd.c
  if (strncmp(line, "cd", 2) == 0)
  {
    char *path = NULL;

    // Si un chemin est fourni après "cd", on le récupère
    if (strlen(line) > 3)
    {
      path = line + 3; // on prend en compte cd + espace avant le /repertoire/titi/machin
    }
    prev_status = cmd_cd(path); // status de la commande -> prev_status qui va être utilisé dans exit.c
    return prev_status;
  }

  // Si l'utilisateur a tapé "ftype", on cmd_ftype de ftype.c
  if (strncmp(line, "ftype", 5) == 0)
  {
    char *ref = NULL;
    command *cmd = fillCommand(line);
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
