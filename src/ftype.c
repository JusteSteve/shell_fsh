/**
 * fichier contenant l'implémentation de la commande ftype
 * @file ftype.c
 */
#include "../headers/internal_cmds.h"

#include <sys/stat.h>
#include <stdio.h>

int cmd_ftype(const char *ref) {
    struct stat sb;

    // Récupère les informations sur le fichier
    if (stat(ref, &sb) == -1) {
        perror("ftype");
        return 1;  // Erreur
    }

    // Utilise switch pour déterminer le type de fichier
    printf("%s : ", ref);
    switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:  printf("block device\n");            break;
        case S_IFCHR:  printf("character device\n");        break;
        case S_IFDIR:  printf("directory\n");               break;
        case S_IFIFO:  printf("FIFO/pipe\n");               break;
        case S_IFLNK:  printf("symlink\n");                 break;
        case S_IFREG:  printf("regular file\n");            break;
        case S_IFSOCK: printf("socket\n");                  break;
        default:       printf("unknown type\n");            break;
    }

    return 0;  // Succès
}
