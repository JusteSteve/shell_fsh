/**
 * Fichier contenant l'implémentation de la commande exit
 * @file ls.c
 */
#include "../headers/internal_cmds.h"

int cmd_ls(char *path) {
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    int fst = 1;

    // Si le paramètre est NULL ou vide, on va au répertoire courant
    if (path == NULL || strlen(path) == 0) {
        path = ".";
    }

    // Ouvre le répertoire
    dir = opendir(path);
    if (dir == NULL) {
        perror("ls");
        return 1;
    }

    // Lit le contenu du répertoire et ignore les fichiers commençant par "."
    while ((entry = readdir(dir)) != NULL) {
        // pour ne pas afficher les répertoires . et .. ou bien les fichiers comme .gitignore etc
        if (entry->d_name[0] != '.' && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if (!fst) {
                write(STDOUT_FILENO, " ", 1); // pour avoir un affichage sur une ligne comme le vrai ls
            }
            fst = 0;
            write(STDOUT_FILENO, entry->d_name, strlen(entry->d_name));  // Affichage du nom du fichier
        }
    }

    // Afficher un retour à la ligne après le dernier fichier
    write(STDOUT_FILENO, "\n", 1);

    // Ferme le répertoire
    closedir(dir);
    return 0;
}
