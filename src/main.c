#include "../headers/fsh.h"

/**
 * Fichier main du projet fsh
 */

int main()
{
  /*
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
  */

  char *prompt;
  char *cmd;
  int last_return_value = 0;
  printf("tapez 'ctr-D' pour quitter\n");
  while (1)
  {
    rl_outstream = stdout;
    prompt = display_prompt(last_return_value);

    cmd = readline(prompt);
    free(prompt);

    if (cmd == NULL)
    {
      printf("Sortie\n");
      break;
    }

    // TODO: Mettre à jour la valeur de retour
    last_return_value++;
    last_return_value %= 2;

    add_history(cmd);
    printf("Commande saisie: %s\n", cmd);
    free(cmd);
  }

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
