/**
 * fichier contenant l'implémentation de la commande ftype
 * @file ftype.c
 */
#include "../headers/internal_cmds.h"

int cmd_ftype(const char *ref) {
    struct stat sb;

    // Récupère les informations sur le fichier
    if (stat(ref, &sb) == -1) {
        perror("ftype Error");
        return 1;  // Erreur
    }

    // Utilise switch pour déterminer le type de fichier
    dprintf(STDOUT_FILENO, "%s : ", ref);
    switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:  dprintf(STDOUT_FILENO, "block device\n");    break;
        case S_IFCHR:  dprintf(STDOUT_FILENO, "character device\n"); break;
        case S_IFDIR:  dprintf(STDOUT_FILENO, "directory\n");        break;
        case S_IFIFO:  dprintf(STDOUT_FILENO, "FIFO/pipe\n");        break;
        case S_IFLNK:  dprintf(STDOUT_FILENO, "symlink\n");          break;
        case S_IFREG:  dprintf(STDOUT_FILENO, "regular file\n");     break;
        case S_IFSOCK: dprintf(STDOUT_FILENO, "socket\n");           break;
        default:       dprintf(STDOUT_FILENO, "unknown type\n");     break;
    }

    return 0;  // Succès
}
