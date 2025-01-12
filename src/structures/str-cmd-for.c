/**
 * fichier contenant l'implémentation de la boucle for et ses options
 * @file for.c
 */

#include "../../headers/cmd-utils.h"
#include "../../headers/signal.h"


int is_type1(char type, struct stat *stat);

comFor *initialiseCommandFor()
{
    // Création d'un nouveau commandFor
    comFor *com = malloc(sizeof(comFor));
    if (com == NULL)
    {
        goto error;
    }
    com->command = NULL;
    com->var = NULL;
    com->dir = NULL;
    com->path = NULL;
    com->options = NULL;
    com->ligne = NULL;
    com->extention = NULL;
    com->type = 0;
    com->fic_caches = 0;
    com->recursive = 0;
    return com;

error:
    clearCommandFor(com);
    return NULL;
}

comFor *fillCommandFor(command *cmd)
{
    comFor *com = initialiseCommandFor();
    if (com == NULL)
    {
        goto error;
    }
    int i = 0;
    // récupérer la variable et rajouter un $ derrière
    com->var = malloc(strlen(cmd->args[1]) + 2);
    if (com->var == NULL)
    {
        perror("[fillCommandFor]>malloc");
        goto error;
    }
    com->var[0] = '$';
    strcpy(com->var + 1, cmd->args[1]);

    // TODO: Gérer les options
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
    int cmt_accolades = 1;
    for (int j = i; cmd->args[j] != NULL; j++)
    {
        if (strcmp(cmd->args[j], "{") == 0)
        {
            cmt_accolades++;
        }
        else if (strcmp(cmd->args[j], "}") == 0)
        {
            cmt_accolades--;
            if (cmt_accolades == 0)
            {
                break;
            }
        }
        ligne_taille += strlen(cmd->args[j]) + 1; // +1 pour le caractere de fin de chaine
    }

    if (cmt_accolades != 0)
    {
        perror("Manque d'accolades");
        goto error;
    }

    com->ligne = malloc(ligne_taille + 1);
    if (com->ligne == NULL)
    {
        perror("[fillCommandFor]>malloc");
        goto error;
    }

    // reconstruire la ligne de commande qui est dans les accolades
    com->ligne[0] = '\0'; // pour être sûr que la chaine soit vide
    cmt_accolades = 1;
    for (; cmd->args[i] != NULL; i++)
    {
        if (strcmp(cmd->args[i], "{") == 0)
        {
            cmt_accolades++;
        }
        else if (strcmp(cmd->args[i], "}") == 0)
        {
            cmt_accolades--;
            if (cmt_accolades == 0)
            {
                break;
            }
        }

        strcat(com->ligne, cmd->args[i]);
        strcat(com->ligne, " ");
    }
    return com;

error:
    clearCommandFor(com);
    return NULL;
}

int parcoursFor(comFor *cmd_for)
{
    DIR *parent = opendir(cmd_for->dir);
    if (parent == NULL)
    {
        fprintf(stderr, "command_for_run: %s\n", cmd_for->dir);
        perror("command_for_run");
        return 1;
    }
    struct dirent *entry;
    int last_return_value = 0;

    while ((entry = readdir(parent)))
    {
        errno = 0;
        // si readdir rencontre une erreur, errno est modif avec une valeur non nulle, et si fin de fichiers à lire, errno ne change pas de valeur
        // car si on ne trouve pas le répertoire que l'on cherche dans son parent, c'est une erreur
        if (entry == NULL && errno != 0)
        {
            perror("[parcoursFor]>readdir");
            goto error;
        }

        char *fichier = entry->d_name;

        // on ignore les fichiers . et ..
        if (strcmp(fichier, ".") == 0 || strcmp(fichier, "..") == 0)
        {
            continue;
        }

        // on ignore les fichiers cachés si l'option -A n'est pas activée
        if (!cmd_for->fic_caches && fichier[0] == '.')
        {
            continue;
        }

        //! FIXME: option -r ne marche pas
        // si c'est un répertoire et que l'option -r est activée, on parcours le répertoire
        if (cmd_for->recursive && entry->d_type == DT_DIR)
        {
            char new_dir[PATH_MAX];
            snprintf(new_dir, PATH_MAX, "%s/%s", cmd_for->dir, fichier);
            comFor *new_cmd_for = initialiseCommandFor();
            if (new_cmd_for == NULL)
            {
                goto error;
            }
            new_cmd_for->var = strdup(cmd_for->var);
            new_cmd_for->dir = new_dir;
            new_cmd_for->ligne = strdup(cmd_for->ligne);
            new_cmd_for->extention = cmd_for->extention;
            new_cmd_for->type = cmd_for->type;
            new_cmd_for->fic_caches = cmd_for->fic_caches;
            new_cmd_for->recursive = cmd_for->recursive;
            new_cmd_for->max_parallel = cmd_for->max_parallel;
            parcoursFor(new_cmd_for);
            clearCommandFor(new_cmd_for);
            parallel = 1;
        }

        // si l'option -e est activée, on vérifie que le fichier a l'extention voulue
        if (cmd_for->extention && !has_extention(fichier, cmd_for->extention))
        {
            continue;
        }

        // construire le chemin complet du fichier
        // hypothèse que le chemin fasse au moins PATH_MAX, ce n'est pas judicieux, mais sans c'est compliqué
        char entry_path[PATH_MAX];
        snprintf(entry_path, PATH_MAX, "%s/%s", cmd_for->dir, fichier);

        struct stat sb;
        if (stat(entry_path, &sb) == -1)
        {
            perror("[parcoursFor]>stat");
            goto error;
        }

        // si l'option -t est activée, on vérifie que le fichier est du type voulu
        if (cmd_for->type != 0 && !is_type1(cmd_for->type, &sb))
        {
            continue;
        }

        if (cmd_for->extention)
        {
            char *nom_sans_ext = supprimer_extention(fichier);
            snprintf(entry_path, PATH_MAX, "%s/%s", cmd_for->dir, nom_sans_ext);
        }

        // remplacer $F par le nom du fichier
        char *cmd_avec_f = remplacer_variable(cmd_for->ligne, cmd_for->var, entry_path);
        if (cmd_avec_f == NULL)
        {
            goto error;
        }

        int return_value = execute_commande(cmd_avec_f);
        // si la commande a échoué, on garde le code de retour le plus élevé
        if (return_value != 0 && last_return_value < return_value)
        {
            last_return_value = return_value;
        }

        free(cmd_avec_f);
    }
    closedir(parent);
    return last_return_value;

error:
    if (parent != NULL)
    {
        closedir(parent);
    }
    clearCommandFor(cmd_for);
    return 1;
}

char *remplacer_variable(char *ligne, char *var, char *valeur)
{
    char *resultat;
    int i, count = 0;
    int nouvelle_taille = strlen(valeur);
    int ancienne_taille = strlen(var);

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

char *supprimer_extention(char *fichier)
{
    char *point = strrchr(fichier, '.');
    if (!point)
    {
        return strdup(fichier);
    }
    size_t nom_taille = point - fichier;
    char *nom_sans_ext = malloc(nom_taille + 1);
    if (nom_sans_ext == NULL)
    {
        perror("[supprimer_extention]>malloc");
        return NULL;
    }
    strncpy(nom_sans_ext, fichier, nom_taille);
    nom_sans_ext[nom_taille] = '\0';
    return nom_sans_ext;
}

bool has_extention(char *fichier, char *extention)
{
    size_t extention_taille = strlen(extention);
    size_t fichier_taille = strlen(fichier);
    if (fichier_taille < extention_taille)
    {
        return false;
    }
    char *extention_debut = fichier + fichier_taille - extention_taille;
    bool rst = fichier[fichier_taille - extention_taille - 1] == '.' && strcmp(extention_debut, extention) == 0;
    return rst;
}

int is_type1(char type, struct stat *stat)
{
    switch (type)
    {
    case 'f': // fichier
        return S_ISREG(stat->st_mode);
    case 'd': // répertoire
        return S_ISDIR(stat->st_mode);
    case 'l': // lien symbolique
        return S_ISLNK(stat->st_mode);
    case 'p': // tube nommé
        return S_ISFIFO(stat->st_mode);
    default:
        fprintf(stderr, "Type de fichier inconnu\n");
        return 0;
    }
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