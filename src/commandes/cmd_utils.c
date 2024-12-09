/**
 * @file cmd_utils.c
 * Contient les implémentations des fonctions utilitaires
 * pour exécuter des commandes.
 */

#include "../../headers/internal_cmds.h"
#include "../../headers/cmd-utils.h"

int prev_status; // pour stocker le status précédent

/* int execute_commande(char *line)
{
  int return_value;
  // vérifier si la contient un ;
  if (strchr(line, ';') != NULL)
  {
    // exécuter la commande structurée
    return_value = exec_structured_cmds(line);
    prev_status = return_value;
    return return_value;
  }
  // créer une structure de commande à partir de la ligne de commande
  command *cmd = fillCommand(line);
  if (cmd == NULL)
  {
    return 1;
  }
  // vérifier si la commande est interne
  if (is_internal_cmd(cmd->nom))
  {
    return_value = exec_internal_cmds(line);
  }
  // vérifier si la commande est un for
  else if (strcmp("for", cmd->nom) == 0)
  {
    return_value = exec_for_cmds(cmd);
  }
  else
  {
    return_value = exec_external_cmds(cmd);
  }
  prev_status = return_value;
  clearCommands(cmd);
  return return_value;
} */

int exec_internal_cmds(char *line)
{
  command *cmd = fillCommand(line);
  if (cmd == NULL)
  {
    return 1;
  }
  // Si l'utilisateur a tapé "exit", on arrête la boucle
  if (strncmp(cmd->nom, "exit", 4) == 0)
  {
    char *val = NULL;
    if (cmd->args[1] != NULL)
    {
      val = cmd->args[1];
    }
    cmd_exit(val);
    // Pas besoin de return car exit termine le programme (NIC SUPPRIME MOI SI TU ME VOIS)
  }
  // Si l'utilisateur a tapé "pwd", on cmd_pwd de pwd.c
  if (strncmp(cmd->nom, "pwd", 3) == 0)
  {
    prev_status = cmd_pwd(); // status de la commande -> prev_status qui va être utilisé dans exit.c
    return prev_status;
  }
  // Si l'utilisateur a tapé "cd", on cmd_cd de cd.c
  if (strncmp(cmd->nom, "cd", 2) == 0)
  {
    char *path = NULL;
    if (cmd->args[1] != NULL)
    {
      path = cmd->args[1];
    }
    prev_status = cmd_cd(path); // status de la commande -> prev_status qui va être utilisé dans exit.c
    return prev_status;
  }
  // Si l'utilisateur a tapé "ftype", on cmd_ftype de ftype.c
  if (strncmp(cmd->nom, "ftype", 5) == 0)
  {
    char *ref = NULL;
    ref = cmd->args[1];
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
  return 1; // Par défaut, on continue la boucle
}

/* int exec_structured_cmds(char *line)
{
  int return_value;
  // diviser la ligne en tableau de commandes simples
   char **cmds_tab = split_cmd(line, 1);
  if (cmds_tab == NULL)
  {
    return 1;
  }
  int cmd_i = 0;
  // exécuter toutes les commandes
  while (cmds_tab[cmd_i] != NULL)
  {
    return_value = execute_commande(cmds_tab[cmd_i]); // à reprendre par ICI BIENTOT FINI

    prev_status = return_value;
    if (return_value == 1)
    {
      return 1;
    }
    sleep(1);
    cmd_i++;
  }
  free_args(cmds_tab); 
  return return_value;
} */

int exec_for_cmds(command *cmd)
{
  // créer une structure de commande for à partir de la cmd
  comFor *command = fillCommandFor(cmd);
  if (command == NULL)
  {
    return 1;
  }
  // exécuter la commande for
  if (parcoursFor(command) != 0)
  {
    clearCommandFor(command);
    return 1;
  }
  clearCommandFor(command);
  return 0;
}

// ***=== Fonctions auxiliaires ===***

int is_internal_cmd(char *cmd)
{
  return (!strcmp(cmd, "cd") || !strcmp(cmd, "pwd") || !strcmp(cmd, "ftype") || !strcmp(cmd, "exit"));
}

int is_line_empty(char *line)
{
  for (int i = 0; line[i] != '\0'; i++)
  {
    if (line[i] != ' ' && line[i] != '\t')
    {
      return 0;
    }
  }
  return 1;
}


int execute_commande(commandsInfo * com)  // voilà le gros du gros. Pas pauffiné au max, je le ferai plus tard.
{// en prenant en argument la structure commandsInfo, il va recouper toutes les commandes par ; puis les executer une par une selon leur spécificité (commande interne, externe ou for)
  int return_value = 0, cptComFor = 0;
  char ** commandes = com->commands;
  char ** commandesFor = com->commandsFor;
  
  for (size_t t = 0; t < *com->com; t++){
	  
	  char *command_copy = strdup(commandes[t]); 
        if (command_copy == NULL) {
            perror("Erreur de duplication de commande");
            return 1;
        }
  
	char *strTrunc = strtok(command_copy, ";");
  
	while (strTrunc != NULL){  //boucle pour attraper tous les arugments séparés par ;
		char *cleaned = trim(strTrunc);  //nettoie des espaces la commande
		if (cleaned == NULL){return 1;}
		printf("Section de code: [%s]\n", cleaned);  // DEBUG
	  
  //if (strchr(line, ';') != NULL)
  //{
    //// exécuter la commande structurée
    //return_value = exec_structured_cmds(line);
    //prev_status = return_value;
    //return return_value;
  //}
		if (strlen(cleaned) == 0) { // Ignorer les portions vides
			strTrunc = strtok(NULL, ";");
			continue;
		}
		else if (strncmp(cleaned, "for", 3) == 0 /*&& (isspace(cleaned[3]) || cleaned[3] == '\0')*/){
			//printf("Good job!\n"); // DEBUG
			command *cmdfor = fillCommand(creating_for_command(strTrunc,commandesFor[cptComFor])); //recréer la commande for pour la transmettre après à exec_for_cmds
			cptComFor++;
			return_value = exec_for_cmds(cmdfor);
      
      if (return_value == 1){  // s'il y a au moins une valeur de retou = 1, alors retourne 1
        prev_status = return_value;
      }
		}
		
    //créer une structure de commande à partir dearguments tronqués, un par un
    command *cmd = fillCommand(strTrunc);  
  
  // vérifier si la commande est interne
    if (is_internal_cmd(cmd->nom)){
      return_value = exec_internal_cmds(strTrunc);
      if (return_value == 1){
        prev_status = return_value;
      }
    }
    else{
      return_value = exec_external_cmds(cmd);
      if (return_value == 1){
        prev_status = return_value;
      }
    }
    //prev_status = return_value;
    clearCommands(cmd);
	  strTrunc = strtok(NULL, ";"); 
	  }
  }
	
  return return_value;  
}



commandsInfo *interm(char* line){ // voilà le gros du gros du big boss. Pas pauffiné au max, je le ferai plus tard.
	char * strToken = strtok(line, "{}");  // séparer les commandes selon si c'est des commandes du for ou commandes classiques
	printf("Début du tour : %s\n", strToken); // DEBUG
	
	commandsInfo *st = malloc(sizeof(commandsInfo));  //allocation de la structure et tous les champs
  if(st == NULL) {return NULL;}
  st->commands = malloc(PATH_MAX * sizeof(char*));
  st->commandsFor = malloc(PATH_MAX * sizeof(char*));
  st->com = malloc(sizeof(size_t));
  st->comfor = malloc(sizeof(size_t));
  if (st->commands == NULL || st->commandsFor == NULL || st->com == NULL || st->comfor == NULL) {return NULL;}
    
	size_t i = 0, j = 0, l = 0;
	
  while (strToken != NULL) {
    st->commands[j] = malloc(strlen(strToken) + 1);
    if (st->commands[j] == NULL) {
      perror("malloc failed");
            
      for (size_t k = 0; k < i; k++) {
          free(st->commands[k]);
      }
      free(st->commands);
      return NULL;
    }
    else if (i % 2 == 0){  // si on tombe sur des commandes classiques
			strcpy(st->commands[j], strToken); 
			printf("Segment %zu: %s\n", j, st->commands[j]);
			j++;
		}
		else {  // si on tombe sur les commandes de for
			st->commandsFor[l] = malloc(strlen(strToken) + 1);
			if (st->commandsFor[l] == NULL) {
				perror("malloc failed");
            
				for (size_t k = 0; k < i; k++) {
					free(st->commandsFor[k]);
				}
				free(st->commandsFor);
				return NULL;
			}
			strcpy(st->commandsFor[l], strToken);  // les commandes de chaque for rencontré sont stockées individuellement dans le pointeur
		  printf("voilà les commandes %s\n", st->commandsFor[l]);  // DEBUG
			l++;
		}
        strToken = strtok(NULL, "{}"); 
        i++;
    }
    //printf("Avant la mise à jour, st->com = %zu, st->comfor = %zu\n", j, l);  // DEBUG

    
    if(j > 0){  //réallocation du pointeur des commandes classiques si besoin
      char **tmpCommands = realloc(st->commands, j * sizeof(char *));
      if (tmpCommands == NULL) {
        for (size_t k = 0; k < j; k++) { free(st->commands[k]); }
          free(st->commands);
          free(st->commandsFor);
          free(st->com);
          free(st->comfor);
          return NULL;
      }
      st->commands = tmpCommands;
    }

	if (l > 0) { //réallocation du pointeur des commandes des for si besoin
    char **tmpCommandsFor = realloc(st->commandsFor, l * sizeof(char *));
    if (tmpCommandsFor == NULL) {
      for (size_t k = 0; k < l; k++) { free(st->commandsFor[k]); }
        free(st->commandsFor);
        free(st->commands);
        free(st->com);
        free(st->comfor);
        return NULL;
    }
    st->commandsFor = tmpCommandsFor;
  }
    *st->com = j; *st->comfor = l;  //le nombre de commandes classiques et le nombre de commandes de for
    
    //printf("Après la mise à jour, st->com = %zu, st->comfor = %zu\n", *st->com, *st->comfor); // DEBUG
    printf("il y a %zu parties de commandes en tout, et %zu commandes pour les for\n", *st->com, *st->comfor); // DEBUG
    
    return st;

    //error:


}



char *trim(char *str) {  // pour nettoyer les commandes des espaces
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str; 

    //char *end = str + strlen(str) - 1;
    //while (end > str && isspace((unsigned char)*end)) end--;
    //*(end + 1) = '\0';
    
    return str;
}

char *creating_for_command(char *corps, char *core) { //corps représente la partie 'for F in REP' de la boucle for, le core représente ce qu'il y a entre les brakets
    size_t taille = strlen(corps) + strlen(core) + 6; // +5 pour les {} et les espaces + 1 pour le '\0'

    char* for_core_tmp = malloc(taille);
    if (for_core_tmp == NULL) {return NULL;}

    snprintf(for_core_tmp, taille, "%s{ %s }", corps, core);
    printf("Jolie boucle for: %s\n", for_core_tmp);

    return for_core_tmp;
}
