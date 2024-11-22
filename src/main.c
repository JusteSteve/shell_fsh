#include "../headers/fsh.h"
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
/*
int run_command(char *line)
{
  int len = strlen(line);
  int green_light = 1; // j'ai pas trouvé mieux
  if (len < 0)
  {
    fprintf(stderr, "Erreur de lecture\n");
    ;
    return 1;
  }

  // Retirer le caractère '\n' à la fin de la ligne
  if (line[len - 1] == '\n')
  {
    line[len - 1] = '\0'; // Supprime le '\n'
  }

  if (redir(line) == 0) {
    //green_light = 0;
  } 
  else { 
    fprintf(stderr, "Erreur lors de la redirection\n");
    return 1; // Une erreur de redirection est survenue
  }

  // Si l'utilisateur a tapé "exit", on arrête la boucle
    if (strncmp(line, "exit", 4) == 0)
    {
      char *val = NULL;
      if (strlen(line) > 5)
      {
        val = line + 5; // "exit " donc offset de 5 comme pour "cd " au final
      }
      cmd_exit(val);
      return 0;
    }

    // Si l'utilisateur a tapé "pwd", on cmd_pwd de pwd.c
    if (strncmp(line, "pwd", 3) == 0)
    {
      prev_status = cmd_pwd(); // status de la commande -> prev_status qui va être utilisé dans exit.c
      return prev_status;
    }

    // Si l'utilisateur a tapé "cd", on cmd_cd de cd.c
    if (strncmp(line, "cd", 2) == 0)
    {
      char *path = NULL;

      // Si un chemin est fourni après "cd", on le récupère
      if (strlen(line) > 3)
      {
        path = line + 3; // on prend en compte cd + espace avant le /repertoire/titi/machin
      }
      prev_status = cmd_cd(path); // status de la commande -> prev_status qui va être utilisé dans exit.c
      return prev_status;
    }

    // Si l'utilisateur a tapé "ftype", on cmd_ftype de ftype.c
    if (strncmp(line, "ftype", 5) == 0)
    {
      char *ref = NULL;
      if (strlen(line) > 6)
      {
        ref = line + 6; // on prend en compte "ftype " avant le fichier/répertoire
      }
      if (ref != NULL)
      {
        prev_status = cmd_ftype(ref);
        return prev_status;
      }
      else
      { // faudrait que je fasse la gestion d'erreur dans les fichiers .c respectifs
        fprintf(stderr, "ftype: missing reference argument\n");
        return 1;
        // si on gère l'erreur dans main, ça permet justement de décider si on veut continuer
        // malgré l'erreur en printant un msg, alors que dans .c, on va juste faire return 1
        // donc ça va retourner !prev_status, à voir pour l'instant donc.
      }
    }
    
    // Si l'utilisateur a tapé "ls", on cmd_ls de ls.c
    if (strncmp(line, "ls", 2) == 0)  
    {
      char *path = NULL;
      if (strlen(line) > 3)
      {
        path = line + 3; // idem
      }
      prev_status = cmd_ls(path);
      return prev_status;
    }

    // Si l'utilisateur a tapé "cat", on cmd_cat de cat.c
    if (strncmp(line, "cat", 3) == 0)
    {
      char *path = NULL;
      if (strlen(line) > 4)
      {
        path = line + 4;
      }
      prev_status = cmd_cat(path);
      return prev_status;
    }
    else {
      return prev_status;
    }
  
  return 1; // Par défaut, on continue la boucle
}
*/