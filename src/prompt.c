#include "../headers/fsh.h"

#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RED "\033[91m"
#define COLOR_DEFAULT "\033[00m"

int switch_color(char *s, char color)
{
  if (s == NULL)
  {
    return -1; // erreur: pointeur null
  }
  char *c;
  switch (color)
  {
  case 'v': // vert
    c = "\033[32m";
    break;
  case 'j': // jaune
    c = "\033[33m";
    break;
  case 'b': // bleu
    c = "\033[34m";
    break;
  case 'r': // rouge
    c = "\033[91m";
    break;

  default: // normal
    c = "\033[00m";
    break;
  }
  int size = strlen(c);
  memcpy(s, c, size);
  return size;
}

int add_prompt(char *dst, char *src)
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
    perror("[display_prompt]> Erreur : calloc a échoué");
    return NULL;
  }

  int idx = 0;
  // FIXME: régler le problème de la couleur qui ne s'applique pas sur le prompt
  // chemin du répertoire
  prompt[idx++] = '\001';
  idx += switch_color(prompt + idx, 'j');
  prompt[idx++] = '\002';

  // FIXME: valeur de retour
  if (last_return_value == 0)
  {
    idx += add_prompt(prompt + idx, "[0]");
  }
  else
  {
    idx += add_prompt(prompt + idx, "[1]");
  }

  // récupérer le chemin courant
  char *dir = getenv("PWD"); // répertoire courant
  if (dir == NULL)
  {
    perror("[display_prompt]> Erreur : getenv a échoué");
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
  idx += add_prompt(prompt + idx, new_path); // ajouter le chemin tronqué
  free(new_path);

  // deuxième couleur et $
  prompt[idx++] = '\001';
  idx += switch_color(prompt + idx, 'b');
  prompt[idx++] = '\002';
  idx += add_prompt(prompt + idx, "$ ");

  // couleur du retour
  prompt[idx++] = '\001';
  idx += switch_color(prompt + idx, 'x');
  prompt[idx++] = '\002';

  return prompt;

error:
  free(prompt);
  return NULL;
}
