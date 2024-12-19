/**
 * @file str-cmd.c
 * @brief Fichier contenant l'implémentation des fonctions pour les commandes simples.
 */
#include "../../headers/cmd-utils.h"

command *initialiseCommand()
{
  //printf("initialiseCommand\n");
  // allocation de la mémoire pour la structure
  command *com = malloc(sizeof(command));
  //printf("après malloc\n");
  if (com == NULL)
  {
    return NULL;
  }
  // initialisation des champs
  com->nom = NULL;
  com->args = NULL;
  com->ligne = NULL;
  com->taille = 0;
  //printf("initialiseCommand fin\n");

  return com;
}

command *fillCommand(char *line)
{
  //printf("fillCommand\n");
  command *commande = initialiseCommand();
  if (commande == NULL)
  {
    goto error;
  }
  // printf("fillcommand apres initialisation\n");

  // mettre la ligne de commande dans un tableau
  char **tableau_args = split_cmd(line, 0);
  if (tableau_args == NULL)
  {
    perror("[fillCommand]>Erreur de split_cmd");
    goto error;
  }

  // remplir les champs de la commande
  commande->nom = strdup(tableau_args[0]);
  if (commande->nom == NULL)
  {
    perror("[fillCommand]>strdup");
    goto error;
  }
  commande->taille = get_nb_args(tableau_args);
  commande->args = realloc(tableau_args, (commande->taille + 1) * sizeof(char *));
  if (commande->args == NULL)
  {
    perror("[fillCommand]>realloc");
    goto error;
  }
  commande->ligne = strdup(line);
  if (commande->ligne == NULL)
  {
    perror("[fillCommand]>strdup");
    goto error;
  }
  // printf("fillcommand ligne: %s\n", commande->ligne);
  //printf("fillCommand fin\n");

  return commande;
error:
  clearCommands(commande);
  return NULL;
}

void clearCommands(command *com)
{
  if (com->nom != NULL)
  {
    free(com->nom);
  }
  if (com->ligne != NULL)
  {
    free(com->ligne);
  }
  if (com->args != NULL)
  {
    free_args(com->args);
  }
  com->taille = 0;

  if (com != NULL)
  {
    free(com);
  }
}

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

void free_cmds_tab(command **cmds_tab)
{
  if (cmds_tab == NULL)
    return;
  for (int i = 0; cmds_tab[i] != NULL; i++)
  {
    clearCommands(cmds_tab[i]);
  }
  free(cmds_tab);
}