/**
 * @file redir.h
 * @brief Fichier contenant les différentes fonctions pour gérer les redirections.
*/

#ifndef REDIR_H
#define REDIR_H

// ***=== Inclusions ===***
#include "fsh.h"
#include "cmd-utils.h"


/** 
 * @brief Enumération des types de redirection
*/
typedef enum {
    REDIR_STDIN,            // <
    REDIR_STDOUT,           // >
    REDIR_STDOUT_TRUNC,     // >|
    REDIR_STDOUT_APPEND,    // >>
    REDIR_STDERR,           // 2>
    REDIR_STDERR_TRUNC,     // 2>|
    REDIR_STDERR_APPEND     // 2>>
} redir_type;

typedef struct 
{
    redir_type type;    // type de redirection
    char *fichier;      // nom du fichier
    int fd;             // le descripteur du flux (STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO)

} redirection;

int extraire_flags(redirection *r);

redirection *initialiser_redirection();
redirection *remplir_redirection(char *fichier, char *token);
void free_redirection(redirection *redir);

redirection **extraire_redirections(command *line);

int nb_redirections(command *cmd);

void free_redir_tab(redirection **redir_tab);

int exec_redirections(redirection **redir_tab);

int contient_redirection(char *line);

int execute_command_with_redirections(command *cmd);

int redir_out(command *cmd, redirection *redir);
int redir_in(command *cmd, redirection *redir);


/**
 * Détecte la présence d'une redirection dans la ligne de commande
 * @return le type de redirection détectée, -1 si aucune n'a été trouvée
*/
int is_redirection(char *line);

/**
 * Permet d'appeler la fonction de redirection adaptée
 * @return 0 si tout s'est bien passé, 1 sinon
*/
int redir_handler(command *cmd);

/**
 * Agit comme CMD < FIC
 * @return 0 si tout s'est bien passé, 1 sinon
*/
int redir_stdin(char *line);

/**
 * Agit comme CMD > FIC
 * @return 0 si tout s'est bien passé, 1 sinon
*/
int redir_stdout(char *line, redirection *redir);

/**
 * Agit comme CMD >| FIC
 * @return 0 si tout s'est bien passé, 1 sinon
*/
int redir_stdout_trunc(char *line);

/**
 * Agit comme CMD >> FIC 
 * @return 0 si tout s'est bien passé, 1 sinon
*/
int redir_stdout_append(char *line);

/**
 * Agit comme CMD 2> FIC
 * @return 0 si tout s'est bien passé, 1 sinon
*/
int redir_stderr(char *line);

/**
 * Agit comme CMD 2>| FIC
 * @return 0 si tout s'est bien passé, 1 sinon
*/
int redir_stderr_trunc(char *line);

/**
 * Agit comme CMD 2>> FIC
 * @return 0 si tout s'est bien passé, 1 sinon
*/
int redir_stderr_append(char *line);

/**
 * @brief alternative à isspace de la librairie ctype.h
 * @return 1 si c est un espace, saut de ligne, ..., 0 sinon
*/
int is_space(char c);

#endif
