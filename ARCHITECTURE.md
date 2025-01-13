# Architecture du projet fsh

## Introduction

Ce document décrit l'architecture logicielle, les structures de données et les algorithmes utilisés dans le projet.

## Architecture logicielle

Le projet est organisé en plusieurs fichiers source et en-têtes, chacun responsable d'une partie spécifique de la fonctionnalité du shell. Voici un aperçu des principaux fichiers et de leurs rôles :

- `main.c` : Point d'entrée du programme, gère la boucle principale du shell.
- `prompt.c` : Génère le prompt du shell.
- `cmd-utils.c` : Contient les fonctions utilitaires pour exécuter des commandes.
- `external_cmds.c` : Implémente les fonctions pour gérer les commandes externes.
- `str-cmd.c` : Implémente les fonctions pour les commandes simples.
- `str-cmd-if.c` : Implémente les fonctions pour les commandes structurées de type `if`.
- `str-cmd-for.c` : Implémente les fonctions pour les boucles `for`.
- `pipelines.c` : Implémente les fonctions pour gérer les pipelines.
- `redir_in_out.c` : Implémente les fonctions de redirection d'entrée et de sortie.
- `redir_handler.c` : Gère l'appel des fonctions de redirection.
- `signal.c` : Gère la capture et la gestion des signaux `SIGINT` et `SIGTERM`.

```txt
src/
├── main.c
├── autres/
│   ├── prompt.c
│   └── signal.c
├── commandes/
│   ├── internes/
│   │   ├── pwd.c
│   │   ├── cd.c
│   │   ├── exit.c
│   │   └── ftype.c
│   ├── external_cmds.c
│   ├── cmd_utils.c
│   └── pipelines.c
├── redirections/
│   ├── redir_handler.c
│   └── redir_in_out.c
└── structures/
    ├── str-cmd-for.c
    ├── str-cmd.c
    └── str-cmd-if.c
```

## Structures de données

### Commande simple

La structure `command` représente une commande simple avec ses arguments.

```c
typedef struct {
    char *nom;      // Nom de la commande
    char **args;    // Arguments de la commande
    char *ligne;    // Ligne de commande complète
    int taille;     // Nombre d'arguments
} command;
```

### Commande if

La structure `cmd_if` représente une commande structurée de type if.

```c
typedef struct {
    char *ligne;    // Ligne de commande complète
    char *test;     // Commande de test
    char *cmd_if;   // Commande à exécuter si le test est vrai
    char *cmd_else; // Commande à exécuter si le test est faux
} cmd_if;
```

La fonction `exec_cmd_if` exécute une commande if. Elle exécute d'abord la commande de test, puis exécute la commande if ou else en fonction du résultat du test.

### Commande for

La structure `cmd_for` représente une boucle for.

```c
typedef struct {
    char *command;      // Commande à exécuter
    char *var;          // Variable de boucle
    char *dir;          // Répertoire à parcourir
    char *path;         // Chemin du fichier
    char *ligne;        // Ligne de commande complète
    char type;          // Type de fichier à filtrer
    int fic_caches;     // Inclure les fichiers cachés
    int recursive;      // Parcourir récursivement les répertoires
    int max_parallel;   // Nombre maximum de processus parallèles
} comFor;
```

La fonction `parcoursFor` parcourt un répertoire et exécute une commande pour chaque fichier correspondant aux critères spécifiés (extension, type, etc.). Elle gère également l'exécution parallèle des commandes en utilisant `fork` et `wait`.

#### Redirection

La structure `redirection` représente une redirection d'entrée ou de sortie.

```c
typedef struct {
    redir_type type; // type de redirection
    char *fichier;   // nom du fichier
    int fd;          // le descripteur du flux (STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO)
} redirection;
```

Les fonctions `redir_in` et `redir_out` implémentent les redirections d'entrée et de sortie. La fonction `redir_handler` gère l'appel des fonctions de redirection appropriées en fonction des arguments de la commande.

### Algorithmes implémentés

#### Prompt

La fonction `display_prompt` génère le prompt du shell, affichant le code de retour de la dernière commande exécutée et le répertoire courant.

#### Exécution de commandes

La fonction `execute_commande` est responsable de l'exécution des commandes. Elle crée une structure de commande à partir de la ligne de commande, puis appelle la fonction appropriée en fonction du type de commande (simple, if, for, pipeline, etc.).

#### Pipelines

La fonction `exec_pipeline_cmds` gère l'exécution des pipelines. Elle divise la ligne de commande en plusieurs commandes, crée des pipes pour rediriger les entrées et sorties, et exécute chaque commande dans un processus enfant.

#### Gestion des signaux

La fonction `gestion_signal` capture les signaux `SIGINT` et `SIGTERM` et met à jour des variables globales pour indiquer qu'un signal a été reçu. La fonction `restaurer_signal` réinitialise les gestionnaires de signaux à leur comportement par défaut dans les processus enfants.
