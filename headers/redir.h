/**
 * @file redir.h
 * @brief Fichier contenant les différentes fonctions pour gérer les redirections.
*/

#ifndef REDIR_H
#define REDIR_H

// ***=== Inclusions ===***
#include "internal_cmds.h"
#include "str-cmd.h"

/**
 * Détecte la présence d'une redirection dans la ligne de commande
 * @return le type de redirection détectée, -1 si aucune n'a été trouvée
*/
int is_redirection(char *line);

/**
 * Permet d'appeler la fonction de redirection adaptée
 * @return 0 si tout s'est bien passé, 1 sinon
*/
int redir_handler(char *line, int type);

/**
 * Agit comme CMD < FIC
 * @return 0 si tout s'est bien passé, 1 sinon
*/
int redir_stdin(char *line);

/**
 * Agit comme CMD > FIC
 * @return 0 si tout s'est bien passé, 1 sinon
*/
int redir_stdout(char *line);

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
 * @brief Enumération des types de redirection
*/
enum redir_type {
    REDIR_STDIN,
    REDIR_STDOUT,
    REDIR_STDOUT_TRUNC,
    REDIR_STDOUT_APPEND,
    REDIR_STDERR,
    REDIR_STDERR_TRUNC,
    REDIR_STDERR_APPEND
};

/**
 * @brief alternative à isspace de la librairie ctype.h
 * @return 1 si c est un espace, saut de ligne, ..., 0 sinon
*/
int is_space(char c);

#endif
