#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

void print_path() {
    char buffer[256];  // Un buffer pour stocker le chemin du répertoire
    ssize_t len;

    // Ouvre le lien symbolique /proc/self/cwd, len - 1 à cause du fonctionnement de readlink
    len = readlink("/proc/self/cwd", buffer, sizeof(buffer) - 1);

    if (len < 0) { // si readlink a échoué, sinon il retourne la taille du chemin donc 0 soucis
        perror("print_path");
        return;
    }

    // chaîne est bien terminée par un '\0' comme on travaille avec readlink
    buffer[len] = '\0';


    printf("pwd : %s\n", buffer);
}
