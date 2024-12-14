/**
 * @file redir.h
 * @brief Fichier contenant les différentes fonctions pour gérer les redirections.
*/

#ifndef REDIR_H
#define REDIR_H

// ***=== Inclusions ===***
#include "internal_cmds.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>

/**
 * Détecte la présence d'une redirection dans la ligne de commande
 * @return 1 si une redirection est détectée, 0 sinon et le code continue
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

#endif
