/**
 * Fichier contenant l'implémentation de la commande exit
 * @file cat.c
 */
#include "../headers/internal_cmds.h"

int cmd_cat(char *path) {
    int fd = 0;
    char buffer[4096];
    ssize_t bytes_read = 0; 

    // Ouvre le fichier en lecture seule
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("cat");
        return 1;
    }

    // Lit le fichier et affiche le contenu
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    // saut de ligne sinon cat fsh par exemple bah rip
    write (STDOUT_FILENO, "\n", 1);

    // Ferme le fichier
    close(fd);

    return 0;
}