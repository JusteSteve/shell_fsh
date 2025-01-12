/**
 * Fichier main du projet fsh
 */
#include "../headers/fsh.h"
#include "../headers/cmd-utils.h"
#include "../headers/signal.h"
volatile sig_atomic_t last_signal = 0;
volatile sig_atomic_t signalint = 0;



void handler_othersignals (int sig) {
  last_signal = sig;
}

void handler_gather (int sig) {
  if (sig >= 0){
  signalint = 1;
  }
}

int main()
{
  struct sigaction action_return = {0};
  struct sigaction action_gather = {0};
  struct sigaction action_term = {0};
	
	action_return.sa_handler = handler_othersignals; 
  action_gather.sa_handler = handler_gather;
	action_term.sa_handler = SIG_IGN;
	
	for (int i = 1; i < NSIG; i++){
		if (i == SIGTERM || i == SIGINT) {continue;}
		if (sigaction(i, &action_return, NULL) != 0){
			//fprintf(stderr, "%d (%s): %s\n", i, strsignal(i), strerror(errno));
		}
	}

  if (sigaction(SIGINT, &action_gather, NULL) != 0){
			//fprintf(stderr, "%d (%s): %s\n", SIGINT, strsignal(SIGINT), strerror(errno));
  }
	
	if (sigaction(SIGTERM, &action_term, NULL) != 0){
			//fprintf(stderr, "%d (%s): %s\n", SIGTERM, strsignal(SIGTERM), strerror(errno));
  }

  char *prompt;
  char *line;
  int last_return_value = 0;

  printf("tapez 'exit' ou ctrl-d pour quitter\n");
  rl_outstream = stderr;
  while (1)
  {
    if (last_signal != 0){
      last_signal = 0;
      last_return_value = 255;
    }

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

    last_return_value = execute_commande(line);
    add_history(line);
    free(line);
  }
  return last_return_value;
}