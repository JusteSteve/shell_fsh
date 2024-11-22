/**
 * @file redir.c
 * Contient l'implémentation de CMD > FIC
 */
#include "../../headers/redirections.h"

int redir(char *line) {
    char *output_file = NULL;
    int fd;
    char *redir_pos;
    int saved_stdout;

    // Recherche de la redirection de sortie " > " mais normalement on devrait plus en avoir besoin bientôt
    redir_pos = strstr(line, " > ");
    if (redir_pos == NULL) {
        return 0; // Aucune redirection de sortie trouvée à voir comment on gère prochainement
    }

    // Sauvegarder la sortie standard actuelle
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        dprintf (STDERR_FILENO, "Error saving stdout: %s\n", strerror(errno));
        return 1;
    }

    // Extraire le nom du fichier après " > "
    output_file = strtok(redir_pos + 3, " ");
    if (output_file == NULL) {
        dprintf (STDERR_FILENO, "Error: no output file specified\n");
        return 1; // Erreur, fichier non spécifié du genre "ls > " mais ça fait que ls dans la manière dont gère le prompt
    }

    // Vérifier si le fichier existe déjà
    struct stat stat_buf;
    if (stat(output_file, &stat_buf) == 0) {
        // on empêche l'écrasement du fichier
        dprintf(STDERR_FILENO, "Error: %s already exists\n", output_file);
        return 1; 
    }

    // yapa trunc on prend excl
    fd = open(output_file, O_WRONLY | O_CREAT | O_EXCL, 0644); 
    if (fd == -1) {
        dprintf(STDERR_FILENO, "Error opening %s: %s\n", output_file, strerror(errno));
        return 1;
    }

    // Rediriger la sortie standard vers le fichier
    if (dup2(fd, STDOUT_FILENO) == -1) {
        dprintf(STDERR_FILENO, "Error redirecting stdout: %s\n", strerror(errno));
        close(fd);
        dup2(saved_stdout, STDOUT_FILENO); // Restaurer stdout en cas d'échec
        return 1;
    }

    close(fd); // On peut fermer fd car il est maintenant associé à STDOUT_FILENO

    // Retirer la partie de redirection de la ligne de commande pour exécuter la commande sans " > FIC"
    *redir_pos = '\0'; // Terminer la ligne avant la redirection

    // Exécuter la commande avec la sortie redirigée
    int result = system(line);

    // Restaurer la sortie standard pour le shell
    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        dprintf(STDERR_FILENO, "Error restoring stdout: %s\n", strerror(errno));
        close(saved_stdout);
        return 1;
    }

    close(saved_stdout); // Fermer le descripteur sauvegardé
    return result; // Retourne le résultat de la commande
}
