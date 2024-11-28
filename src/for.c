/**
 * fichier contenant l'implémentation de la boucle for et ses options
 * @file for.c
 */
#include "../headers/for.h"
#include "../headers/commands.h"
#include "../headers/fsh.h"
#include "../headers/internal_cmds.h"
#include "../headers/debug.h"

comFor *initialiseCommandFor()
{
    // Création d'un nouveau commandFor
    comFor *com = malloc(sizeof(comFor));
    if (com == NULL)
    {
        goto error;
    }

    com->command = NULL;
    com->dir = NULL;
    com->path = NULL;
    com->options = NULL;
    com->ligne = NULL;
    com->extention = NULL;

    return com;

error:
    clearCommandFor(com);
    return NULL;
}

comFor *fillCommandFor(command *cmd)
{ // version ne prennant pas compte des options
    comFor *com = initialiseCommandFor();
    if (com == NULL)
    {
        goto error;
    }
    int i = 0;

    // tracage
    //dprintf(STDOUT_FILENO, "fillCommandFor, cmd->args[0] : %s\n", cmd->args[0]);

    // parser les options de la commande PAS UTILISEE POUR L'INSTANT
    for (i = 3; cmd->args[i] != NULL && strcmp(cmd->args[i], "{") != 0; i++)
    {
        if (strcmp(cmd->args[i], "-A") == 0)
        {
            com->fic_caches = 1;
        }
        else if (strcmp(cmd->args[i], "-r") == 0)
        {
            com->recursive = 1;
        }
        else if (strcmp(cmd->args[i], "-e") == 0 && cmd->args[i + 1] != NULL)
        {
            com->extention = strdup(cmd->args[++i]);
        }
        else if (strcmp(cmd->args[i], "-t") == 0 && cmd->args[i + 1] != NULL)
        {
            com->type = cmd->args[++i][0];
        }
        else if (strcmp(cmd->args[i], "-p") == 0 && cmd->args[i + 1] != NULL)
        {
            com->max_parallel = atoi(cmd->args[++i]);
        }
        else
        {
            com->dir = strdup(cmd->args[i]);
        }
    }

    if (cmd->args[i] == NULL || strcmp(cmd->args[i], "{") != 0)
    {
        // savoir si l'utilisateur a imput "{CMD}" ou "{ CMD }" (version correcte)
        perror("User input");
        goto error;
    }

    // lire ce qui est entre les accolades et le mettre dans com->ligne
    i += 1; // pour passer le "{"
    int ligne_taille = 0;
    for (int j = i; cmd->args[j] != NULL && strcmp(cmd->args[j], "}") != 0; j++)
    {
        ligne_taille += strlen(cmd->args[j]) + 1; // +1 pour le caractere de fin de chaine
    }

    com->ligne = malloc(ligne_taille + 1);
    if (com->ligne == NULL)
    {
        perror("[fillCommandFor]>malloc");
        goto error;
    }

    // reconstruire la ligne de commande qui est dans les accolades
    com->ligne[0] = '\0'; // pour être sûr que la chaine soit vide
    // peut être problèmatique avec ␣ftype␣ ␣W␣
    for (; cmd->args[i] != NULL && strcmp(cmd->args[i], "}") != 0; i++)
    {
        strcat(com->ligne, cmd->args[i]);
        strcat(com->ligne, " ");
    }

    // supprimer le dernier espace
    if (ligne_taille > 0)
    {
        com->ligne[ligne_taille - 1] = '\0';
    }

    // tracage
    //dprintf(STDOUT_FILENO, "fillCommandFor, com->ligne : %s\n", com->ligne);
    return com;

error:
    clearCommandFor(com);
    return NULL;
}

int parcoursFor(comFor *cm)
{
    DIR *parent = NULL;
    struct dirent *entry;

    // tracage
    //dprintf(STDOUT_FILENO, "parcoursFor, cm->dir : %s\n", cm->dir);

    parent = opendir(cm->dir);
    if (parent == NULL)
    {
        goto error;
    }

    // tracage "before while ParcoursFor"
    //dprintf(STDOUT_FILENO, "before while ParcoursFor\n");

    while ((entry = readdir(parent)))
    {
        // tracage 
        //dprintf(STDOUT_FILENO, "entry->d_name : %s\n", entry->d_name);
        errno = 0;
        if (entry == NULL && errno != 0)
        {
            goto error;
        } // si readdir rencontre une erreur, errno est modif avec une valeur non nulle, et si fin de fichiers à lire, errno ne change pas de valeur
        // car si on ne trouve pas le répertoire que l'on cherche dans son parent, c'est une erreur
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {   
            // tracage 1er if
            //dprintf(STDOUT_FILENO, "1er if\n");
            continue;
        }
        if (!cm->fic_caches && entry->d_name[0] == '.')
        {   
            // tracage 2eme if
            //dprintf(STDOUT_FILENO, "2eme if\n");
            continue;
        }
        if (cm->extention && !strstr(entry->d_name, cm->extention))
        {   
            // tracage 3eme if
            //dprintf(STDOUT_FILENO, "3eme if\n");
            continue;
        }
        /*
        if (cm->type && cm->t   ype != entry->d_type)
        {
            // tracage 4eme if
            dprintf(STDOUT_FILENO, "4eme if\n");
            continue;
        }
        */


        char entry_path[PATH_MAX]; // hypothèse que le chemin fasse au moins PATH_MAX, ce n'est pas judicieux, mais sans c'est compliqué
        // Tracage
        //dprintf(STDOUT_FILENO, "entry->d_name : %s\n", entry->d_name);
        snprintf(entry_path, PATH_MAX, "%s/%s", cm->dir, entry->d_name);

        // tracage 
        //dprintf(STDOUT_FILENO, "rentrée dans remplacer_variable\n");

        // remplacer $F par le nom du fichier
        char *cmd_avec_f = remplacer_variable(cm->ligne, "$F", entry_path);
        execute_commande(cmd_avec_f);
        free(cmd_avec_f);

        // FIXME: PAS UTILSE POUR L'INSTANT A VOIR APRES JALON 1
        if (cm->recursive && entry->d_type == DT_DIR)
        {
            comFor *sous_cmd = initialiseCommandFor();
            sous_cmd->command = strdup(cm->command);
            sous_cmd->dir = strdup(entry_path);
            sous_cmd->fic_caches = cm->fic_caches;
            sous_cmd->recursive = cm->recursive;
            sous_cmd->extention = cm->extention ? strdup(cm->extention) : NULL;
            sous_cmd->type = cm->type;
            sous_cmd->max_parallel = cm->max_parallel;
            parcoursFor(sous_cmd);
            clearCommandFor(sous_cmd);
        }
    }
    closedir(parent);
    return 0;

error:
    if (parent != NULL)
    {
        closedir(parent);
    }
    clearCommandFor(cm);
    return 1;
}

char *remplacer_variable(char *ligne, char *var, char *valeur)
{
    char *resultat;
    int i, count = 0;
    int nouvelle_taille = strlen(valeur);
    int ancienne_taille = strlen(var);

    // tracage  affiche variable et valeur
    //dprintf(STDOUT_FILENO, "remplacer_variable, var : %s, valeur : %s\n", var, valeur);

    // Comptage du nombre de fois où la variable apparaît
    for (i = 0; ligne[i] != '\0'; i++)
    {
        if (strstr(&ligne[i], var) == &ligne[i])
        {
            count++;
            i += ancienne_taille - 1;
        }
    }

    // Réserve de mémoire pour le résultat
    resultat = malloc(i + count * (nouvelle_taille - ancienne_taille) + 1);
    if (resultat == NULL)
    {
        perror("malloc");
        exit(1);
    }

    i = 0;
    while (*ligne)
    {
        if (strstr(ligne, var) == ligne)
        {
            strcpy(&resultat[i], valeur);
            i += nouvelle_taille;
            ligne += ancienne_taille;
        }
        else
        {
            resultat[i++] = *ligne++;
        }
    }
    resultat[i] = '\0';
    return resultat;
}

void clearCommandFor(comFor *com)
{
    if (com->command != NULL)
    {
        free(com->command);
    }
    if (com->ligne != NULL)
    {
        free(com->ligne);
    }
    if (com->path != NULL)
    {
        free(com->path);
    }
    if (com->options != NULL)
    {
        free(com->options);
    }
    if (com->dir != NULL)
    {
        free(com->dir);
    }
    if (com->extention != NULL)
    {
        free(com->extention);
    }

    if (com != NULL)
    {
        free(com);
    }
}