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
  //int last_return_value = 0;
  prev_status = 0;

  printf("tapez 'exit' pour quitter\n");
  rl_outstream = stderr;
  while (1)
  {
    prompt = display_prompt(prev_status);
    line = readline(prompt);
    free(prompt);

    if (line == NULL || is_line_empty(line))
    {
      free(line);
      continue;
    }
    
    prev_status = execute_commande(line);
    add_history(line);
    free(line);
  }
  return 0;
}
