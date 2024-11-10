//#include <linux/limits.h> // on va le define nous même car sur macos ça marche pas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#define PATH_MAX 4096

int cmd_pwd() {
    char *cwd_buffer = malloc(PATH_MAX);

    // ça devrait pas arriver mais au cas où
    if (cwd_buffer == NULL || getcwd(cwd_buffer, PATH_MAX) == NULL) {
        perror ("pwd : malloc failed");
        goto error;
    }

    if (getcwd(cwd_buffer, PATH_MAX) != NULL) {
        printf("%s\n", cwd_buffer);
        free(cwd_buffer);
        return 0;
    } 
    else {
        perror ("print_path");
        goto error;
    }

    // gestion d'erreur
    error :
    free(cwd_buffer);
    return 1;
}
