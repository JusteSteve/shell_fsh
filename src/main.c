#include "../headers/fsh.h"
/**
 * Fichier main du projet fsh
 */

int main()
{
  char *line = NULL;
  size_t line_size = 0;
  printf("--- Test Boucle ---\n");
  printf("tapez 'exit' pour quitter\n");

  int num = 1; // Numéro de la ligne
  int running = 1;
  while (running)
  {
    running = display_prompt_test(line, num, line_size);
    num++;
  }

  free(line);
  printf("\nFin du programme\n");

  return 0;
}

int display_prompt_test(char *line, int line_num, size_t line_size)
{
  printf("[%d]>> ", line_num);
  int len = getline(&line, &line_size, stdin);
  if (len < 0)
  {
    printf("Erreur de lecture\n");
    return 0;
  }
  // alterner entre rouge et vert pour chaque ligne
  switch (line_num % 2)
  {
  case 0:
    printf("\033[0;31m"); // Rouge
    break;
  case 1:
    printf("\033[0;32m"); // Vert
    break;

  default:
    break;
  }

  return strcmp(line, "exit\n");
}
