/**
 * Fichier main du projet fsh
 */
#include "../headers/fsh.h"
#include "../headers/cmd-utils.h"
#include "../headers/signal.h"

extern volatile sig_atomic_t signal_recu;
extern volatile sig_atomic_t signal_sigint;

extern int prev_status;
int main()
{
  char *prompt;
  char *line;
  int last_return_value = 0;
  init_signal();

  printf("tapez 'exit' ou ctrl-d pour quitter\n");
  rl_outstream = stderr;
  while (1)
  {
    if(signal_recu)
    {
      last_return_value = 255;
    }
    prompt = display_prompt(last_return_value);
    signal_recu = 0;

    line = readline(prompt);
    free(prompt);
    signal_recu = 0;

    if (line == NULL) // gerer ctrl+d (EOF)
    {
      break;
    }

    if (is_line_empty(line))
    {
      free(line);
      continue;
    }

    last_return_value = execute_commande(line);
    add_history(line);
    free(line);

  }
  return last_return_value;
}
