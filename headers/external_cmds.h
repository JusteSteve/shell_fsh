#ifndef EXTERNAL_CMDS_H
#define EXTERNAL_CMDS_H

// ***=== Inclusions ===***

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_CMDS 100

char **split_cmd(char *cmd);
void free_words(char **args);
int exec_external_cmds(char **args);
int get_nb_args(char **args);


#endif

