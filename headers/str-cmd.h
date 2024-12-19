/**
 * @file str-cmd.h
 * @brief Fichier contenant les différentes structures pour les différents types de commandes
 * et les fonctions associées.
 */

#ifndef STR_CMD_H
#define STR_CMD_H

// ***=== Inclusions ===***
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

// ***=== Structure pour les commandes simples ===***

#define PATH_MAX 4096 // taille maximale d'un chemin
#define MAX_CMDS 1024 // nombre maximal d'arguments dans une commande

/**
 * @brief Structure de données pour les commandes simples
 */
typedef struct command
{
    char *ligne; // toute la ligne de commande
    char *nom;   // nom de la commande
    char **args; // arguments de la ligne de commande
    int taille;  // nombre d'éléments dans le tableau args
} command;

// ***=== Prototypes des fonctions associées ===***

/**
 * @brief Initialise la structure command
 * @return pointeur sur la structure command
 */
command *initialiseCommand();

/**
 * @brief Remplit les champs de la structure command selon les informations
 * @param line : ligne de commande
 * @return pointeur sur la structure command
 */
command *fillCommand(char *line);

/**
 * divise une ligne de commande en arguments
 * avec les espaces comme séparateurs.
 * @param cmd : ligne de commande à exécuter
 * @param flag : 1 si c'est une ligne de commande structurée, 0 sinon
 * @return tableau de pointeur de char contenant les arguments de la commande
 */
char **split_cmd(char *line, int flag);

/**
 * @param args : tableau de chaines de caracteres
 * @return le nombre cases occupées de args
 */
int get_nb_args(char **args);

/**
 * @param args : tableau de chaines de caracteres
 * libère la mémoire allouée pour args
 */
void free_args(char **args);

/**
 * @brief Libère la mémoire allouée pour les champs de la structure command
 * @param com : pointeur sur la structure command
 */
void clearCommands(command *cmd);

/**
 * @brief Libère la mémoire allouée pour les champs de la structure command
 * @param cmds_tab : pointeur sur la structure command
 */
void free_cmds_tab(command **cmds_tab);

// ***=== Structure pour les commandes structurées de type for ===***

/**
 * @brief Structure de données pour les commandes structurées type for ... in .. {commandes}
 */
typedef struct comFor
{
    char *ligne;   // toute la ligne de commande
    char *command; // renseigne le nom de la commande
    char *var;     // renseigne la variable de boucle
    char *dir;     // renseigne le répertoire courant
    char *path;    // renseigne le chemin du fichier dont l'on désire l'exécution
    char *options; // donne les options de boucle

    // C POUR LES OPTIONS PLUSTARDS
    int fic_caches;   // fichiers cachés -A
    int recursive;    // récursivité -r
    char *extention;  // extension -e
    char type;        // type de fichier à traiter -t (f : fichiers ordinaires, d : répertoire, l : lien symbolique, p : tubes)
    int max_parallel; // nombre maximal de tours de boucle en parallèle -p
} comFor;

// Prototypes des fonctions associées

/**
 * @brief Initialise la structure command
 * @return pointeur sur la structure comFor
 */
comFor *initialiseCommandFor();

/**
 * @brief Remplit les champs de la structure comFor selon les informations
 * @param cmd : pointeur sur la structure command
 * @return pointeur sur la structure comFor
 */
comFor *fillCommandFor(command *cmd);

/**
 * @brief Libère la mémoire allouée pour les champs de la structure comFor
 * @param com : pointeur sur la structure comFor
 */
void clearCommandFor(comFor *cmd_for);

/**
 * @brief Fait le parcours du répertoire courant et y applique la commande à chaque fichier trouvé
 * @param cm : pointeur sur la structure comFor
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int parcoursFor(comFor *cmd_for);

/**
 * @brief Remplace une variable par une valeur dans une chaîne de caractères
 * @param str : chaîne de caractères
 * @param var : variable à remplacer
 * @param valeur : valeur de remplacement
 * @return chaîne de caractères avec la variable remplacée
 */
char *remplacer_variable(char *str, char *var, char *valeur);

/**
 * @brief Vérifie si un fichier a une certaine extension
 * @param file : nom du fichier
 * @param extention : extension à vérifier
 * @return true si le fichier a l'extension, false sinon
 */
bool has_extention(char *file, char *extention);

/**
 * @brief Supprime l'extension d'un fichier
 * @param file : nom du fichier
 * @return nom du fichier sans extension
 */
char *supprimer_extention(char *file);

/**
 * @brief Vérifie si un fichier est de type type
 * @param file : nom du fichier
 * @param sb : structure stat du fichier
 * @return 1 si le fichier est du type, 0 sinon
 */
int is_type(char file, struct stat *sb);

// ***=== Structure pour les commandes structurées de type if ===***

/**
 * @brief Structure de données pour les commandes structurées type if TEST { CMD1 } else { CMD2 }
 */
typedef struct cmd_if
{
    char *nom;
    char *ligne;    // toute la ligne de commande
    char *test;     // renseigne le test à effectuer
    char *cmd_if;   // renseigne la commande à exécuter si le test est vrai
    char *cmd_else; // renseigne la commande à exécuter si le test est faux
} cmd_if;

// ***=== Prototypes des fonctions associées ===***

/**
 * @brief Initialise la structure cmd_if
 * @return pointeur sur la structure cmd_if
 */
cmd_if *initialiser_cmd_if();

/**
 * @brief Remplit les champs de la structure cmd_if selon les informations
 * @param cmd : pointeur sur la structure command
 * @return pointeur sur la structure cmd_if
 */
cmd_if *remplir_cmd_if(command *cmd);

/**
 * @brief Libère la mémoire allouée pour les champs de la structure cmd_if
 * @param com : pointeur sur la structure cmd_if
 */
void free_cmd_if(cmd_if *cmd_if);

/**
 * @brief Exécute la commande structurée de type if
 * @param cmd_if : pointeur sur la structure cmd_if
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int exec_cmd_if(cmd_if *cmd_if);

/**
 * @brief Exécute la commande test
 * @param test : ligne de commande test
 * @return 0 si tout s'est bien passé, 1 sinon
 */
int exec_test(char *cmd);

#endif