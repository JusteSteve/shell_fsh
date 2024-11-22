#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

// ***=== Inclusions ===***
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * 
 * 
*/
int handle_input_redirection(char *line);

/**
 * 
 * 
*/
int redir(char *line);

#endif
