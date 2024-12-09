/**
 * Fichier main du projet fsh
 */
#include "../headers/fsh.h"
#include "../headers/cmd-utils.h"

int main()
{
  char *prompt;
  char *line;
  int last_return_value = 0;

  printf("tapez 'exit' ou ctrl-d pour quitter\n");
  rl_outstream = stderr;
  while (1)
  {
    prompt = display_prompt(last_return_value);
    line = readline(prompt);
    free(prompt);

    if (line == NULL) // gerer ctrl+d (EOF)
    {
      break;
    }

    if (is_line_empty(line))
    {
      free(line);
      continue;
    }
    
    last_return_value = execute_commande(interm(line));  //interm va couper la ligne et la transformer en structure commandInfos, pour la renvoyer à execute_commande
    add_history(line);
    free(line);
  }
  return last_return_value;
}
