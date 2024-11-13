/**
 * @file prompt.c
 * Ce fichier contient les fonctions nécessaires pour générer le prompt du shell.
 *
 * Fonctions principales:
 * - add_to_prompt: ajoute une chaîne de caractère au prompt.
 * - truncate_path: tronque le chemin passé en paramètre si sa longueur dépasse une limite spécifiée.
 * - display_prompt: génère le prompt du shell en incluant la couleur, le statut de la dernière commande et le chemin courant
 */

#include "../headers/fsh.h"

/*
  FIXME: j'ai enlevé les caractères de contrôle '\001' et '\002' car ils ne sont pas nécessaires pour la coloration
  peut-être rajouté plus tard si c'est obligatoire
*/

#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RED "\033[91m"
#define COLOR_DEFAULT "\033[00m"

int add_to_prompt(char *dst, char *src)
{
  if (dst == NULL || src == NULL)
  {
    return -1; // erreur: pointeur null
  }
  int size = strlen(src);
  memcpy(dst, src, size);
  return size;
}

char *truncate_path(char *path, int max_length)
{
  if (path == NULL)
  {
    return NULL; // erreur: pointeur null
  }
  char *new_path = calloc(max_length + 1, 1);
  if (new_path == NULL)
  {
    perror("[truncate_path]> Erreur : calloc a échoué");
    return NULL;
  }

  // tronquer le chemin si nécessaire
  int path_len = strlen(path);
  if (path_len > max_length)
  {
    memcpy(new_path, "...", 3);
    char *src = path + path_len - max_length + 6; // FIXME: à régler, normalement c'est +3 pour le "..." mais on a ajouté 3 caractères "[0]" ou "[1]"
    size_t n = max_length - 3;
    memcpy(new_path + 3, src, n);
  }
  else
  {
    strncpy(new_path, path, path_len);
  }
  return new_path;
}

char *display_prompt(int last_return_value)
{
  char *prompt = calloc(PROMPT_MAX_LENGTH, sizeof(char));
  if (prompt == NULL)
  {
    perror("[display_prompt]>calloc:");
    return NULL;
  }

  int idx = 0;

  // FIXME: valeur de retour
  // début du prompt [0] ou [1]
  switch (last_return_value)
  {
  case 0:
    idx += add_to_prompt(prompt + idx, COLOR_GREEN);
    idx += add_to_prompt(prompt + idx, "[0]");
    break;
  case 1:
    idx += add_to_prompt(prompt + idx, COLOR_RED);
    idx += add_to_prompt(prompt + idx, "[1]");
    break;
  default:
    idx += add_to_prompt(prompt + idx, COLOR_YELLOW);
    idx += add_to_prompt(prompt + idx, "[SIG]");
    break;
  }

  // récupérer le chemin du répertoire courant
  char *dir = getenv("PWD");
  if (dir == NULL)
  {
    perror("[display_prompt]>getenv:");
    goto error;
  }

  // tronquer le chemin si nécessaire
  int visible_elements = 2; // $ et espace
  int path_max_len = 30 - visible_elements;
  char *new_path = truncate_path(dir, path_max_len);
  if (new_path == NULL)
  {
    goto error;
  }
  idx += add_to_prompt(prompt + idx, COLOR_DEFAULT);
  idx += add_to_prompt(prompt + idx, new_path); // ajouter le chemin tronqué
  free(new_path);

  // fin du prompt $ et espace
  idx += add_to_prompt(prompt + idx, COLOR_BLUE);
  idx += add_to_prompt(prompt + idx, "$ ");

  // couleur du retour
  idx += add_to_prompt(prompt + idx, COLOR_DEFAULT);

  return prompt;

error:
  free(prompt);
  return NULL;
}
