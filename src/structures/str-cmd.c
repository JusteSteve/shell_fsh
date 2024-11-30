/**
 * @file str-cmd.c
 * @brief Fichier contenant l'implémentation des fonctions pour les commandes simples.
 */
#include "../../headers/cmd-utils.h"

command *initialiseCommand()
{
    // allocation de la mémoire pour la structure
    command *com = malloc(sizeof(command));
    if (com == NULL)
    {
        return NULL;
    }
    // initialisation des champs
    com->nom = NULL;
    com->args = NULL;
    com->ligne = NULL;
    com->taille = 0;

    return com;
}

command *fillCommand(char *line)
{
    command *commande = initialiseCommand();
    if (commande == NULL)
    {
        goto error;
    }
    
    // mettre la ligne de commande dans un tableau
    char **tableau_args = split_cmd(line);
    if (tableau_args == NULL)
    {
        perror("[fillCommand]>Erreur de split_cmd");
        goto error;
    }

    // remplir les champs de la commande
    commande->nom = strdup(tableau_args[0]);
    commande->taille = get_nb_args(tableau_args);
    commande->args = realloc(tableau_args, (commande->taille + 1) * sizeof(char *));
    commande->ligne = strdup(line);

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
