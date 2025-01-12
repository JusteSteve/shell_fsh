/**
 * @file prompt.c
 * Ce fichier contient les fonctions nécessaires pour générer le prompt du shell.
 */

#include "../../headers/fsh.h"
#include "../../headers/signal.h"


char *display_prompt(int last_return_value)
{
  char *prompt = calloc(PROMPT_MAX_LENGTH, sizeof(char));
  if (prompt == NULL)
  {
    perror("[display_prompt]>calloc:");
    return NULL;
  }

  int idx = 0; // curseur qui se déplace dans le prompt au fur et à mesure de l'ajout des éléments

  // début du prompt [0] ou [1]
  if (last_return_value == 0)
  {
    idx += add_to_prompt(prompt + idx, COLOR_GREEN);
  }
  else if (last_return_value == 1)
  {
    idx += add_to_prompt(prompt + idx, COLOR_RED);
  }
  else
  {
    idx += add_to_prompt(prompt + idx, COLOR_YELLOW);
  }

  //différencier le cas interrompu par signal ou retour de fonction

  if (last_return_value == 255){
    idx += snprintf(prompt + idx, PROMPT_MAX_LENGTH - idx, "[SIG]");
  }
  else{
  idx += snprintf(prompt + idx, PROMPT_MAX_LENGTH - idx, "[%d]", last_return_value);
  }

  // récupérer le chemin du répertoire courant
  char *dir = getcwd(NULL, 0);
  if (dir == NULL)
  {
    perror("[display_prompt]>getenv:");
    goto error;
  }

  // tronquer le chemin si nécessaire
  int return_value_len;
  if (last_return_value == 255){
    return_value_len = snprintf(NULL, 0, "%d", last_return_value);
  }
  else{
  return_value_len = snprintf(NULL, 0, "SIG");
  }
  //int return_value_len = snprintf(NULL, 0, "%d", last_return_value);
  int visible_elements = 4 + return_value_len; // $, espace et []
  int path_max_len = 30 - visible_elements;
  char *new_path = truncate_path(dir, path_max_len);
  if (new_path == NULL)
  {
    free(dir);
    goto error;
  }

  idx += add_to_prompt(prompt + idx, COLOR_DEFAULT);
  idx += add_to_prompt(prompt + idx, new_path); // ajouter le chemin tronqué
  free(new_path);

  // fin du prompt $ et espace
  idx += add_to_prompt(prompt + idx, COLOR_BLUE);
  idx += add_to_prompt(prompt + idx, "$ ");

  // couleur par défaut pour la suite
  idx += add_to_prompt(prompt + idx, COLOR_DEFAULT);

  free(dir);
  return prompt;

error:
  free(prompt);
  return NULL;
}

// ***=== Fonctions auxiliaires ===***

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

  int path_len = strlen(path);
  if (path_len <= max_length)
  {
    return strdup(path); // pas besoin de tronquer
  }

  // allouer la mémoire pour le nouveau chemin
  char *new_path = calloc(max_length + 1, sizeof(char));
  if (new_path == NULL)
  {
    perror("[truncate_path]>calloc:");
    return NULL;
  }

  // tronquer le chemin si nécessaire
  if (path_len > max_length)
  {
    memcpy(new_path, "...", 3);
    char *src = path + path_len - (max_length - 3);
    size_t n = max_length - 3;
    memcpy(new_path + 3, src, n);
  }
  else
  {
    strncpy(new_path, path, path_len);
  }
  return new_path;
}
