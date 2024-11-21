#include "../headers/fsh.h"
#include "../headers/internal_cmds.h"
#include "../headers/external_cmds.h"

/**
 * Fichier main du projet fsh
 */

int main()
{
  char *prompt;
  char *line;
  int last_return_value = 0;

  printf("tapez 'exit' pour quitter\n");
  rl_outstream = stderr;
  while (1)
  {
    prompt = display_prompt(last_return_value);
    line = readline(prompt);
    free(prompt);

    if (line == NULL || is_line_empty(line))
    {
      free(line);
      continue;
    }
    
    last_return_value = execute_commande(line);
    add_history(line);
    free(line);
  }
  return 0;
}
