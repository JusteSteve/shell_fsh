/**
 * @file cmd_utils.c
 * Contient les implémentations des fonctions utilitaires
 * pour exécuter des commandes.
 */

#include "../headers/internal_cmds.h"
#include "../headers/external_cmds.h"

int execute_commande(char *line)
{
  // obtenir un tableau de mots à partir de la ligne de commande
  char **args = split_cmd(line);
  char *nom_cmd = args[0];
  int return_value;
  if (is_internal_cmd(nom_cmd))
  {
    /*
    FIXME: peut-être qu'il faudrait libérer args ici
    car il n'est pas utilisé pour les commandes internes du moins pour l'instant
    comme ça si c'est exit qui est appelé, args sera pas libéré
    avant de terminer le programme
    */
    return_value = exec_internal_cmds(line);
  }
  else
  {
    return_value = exec_external_cmds(args);
  }
  prev_status = return_value;
  free_args(args);
  return return_value;
}

int is_internal_cmd(char *cmd)
{
  return (!strcmp(cmd, "cd") || !strcmp(cmd, "pwd") || !strcmp(cmd, "ftype") || !strcmp(cmd, "exit"));
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
    // return 0; // Pas besoin de return car exit termine le programme (NIC SUPPRIME MOI SI TU ME VOIS)
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
    if (strlen(line) > 6)
    {
      ref = line + 6; // on prend en compte "ftype " avant le fichier/répertoire
    }
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
