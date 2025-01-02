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
 * @brief Structure de données pour les redirections
*/
typedef struct 
{
    redir_type type;    // type de redirection
    char *fichier;      // nom du fichier
    int fd;             // le descripteur du flux (STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO)

} redirection;

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

// ***=== Prototypes des fonctions associées ===***

int exec_cmd_redirection(command *cmd);

int redir_handler(command *cmd);

int redir_out(command *cmd, redirection *redir);

int redir_in( redirection *redir);

/**
 * @brief Initialise la structure redirection
 * @return pointeur sur la structure redirection
 */
redirection *initialiser_redirection();

/**
 * @brief Remplit les champs de la structure redirection selon les informations
 * @param fichier 
 * @param token : type de redirection
 * @return pointeur sur la structure redirection
 */
redirection *remplir_redirection(char *fichier, char *token);

int extraire_flags(redirection *r);

int contient_redirection(char *line);

/**
 * Détecte la présence d'une redirection dans la ligne de commande
 * @return le type de redirection détectée, -1 si aucune n'a été trouvée
*/
int is_redirection(char *line);

/**
 * @brief Libère la mémoire allouée pour les champs de la structure redirection
 * @param redir : pointeur sur la structure redirection
 */
void free_redirection(redirection *redir);

#endif
