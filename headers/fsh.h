/**
 * Fichier d'en-tête
 * Contient les prototypes des fonctions et les inclusions nécessaires.
 * Permet de centraliser les inclusions et les prototypes.
 */
#ifndef FSH_H
#define FSH_H

// ***=== Inclusions ===***
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// ***=== Prototypes ===***

/**
 * Affiche le prompt et récupère la saisie de l'utilisateur
 * @param line la ligne saisie par l'utilisateur
 * @param line_num le numéro de la ligne
 * @param line_size la taille de la ligne
 * @return 1 si l'utilisateur n'a pas saisi 'exit', 0 sinon
 */
int display_prompt_test(char *line, int line_num, size_t line_size);

#endif