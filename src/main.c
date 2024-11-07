#include "../headers/fsh.h"
#include "../headers/pwd.h"
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
    /*
    if (strcmp(line, "pwd\n") == 0) {
      print_path();  // walahi i'm doomed ça crash
    }
    */
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

  // Retirer le caractère '\n' à la fin de la ligne
  if (line[len - 1] == '\n') {
    line[len - 1] = '\0';  // Supprime le '\n'
  }

  // Alterner entre rouge et vert pour chaque ligne
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

  // Si l'utilisateur a tapé "exit", on arrête la boucle
  if (strcmp(line, "exit") == 0) {
    return 0; 
  }
  
  // Si l'utilisateur a tapé "pwd", on path_print de pwd.c
  if (strcmp(line, "pwd") == 0) {
    print_path(); 
    return 1;  // je sais pas 0 ou 1 mais on laisse sur 1 pour le moment
  }  
  return 1;  // Par défaut, on continue la boucle
}

