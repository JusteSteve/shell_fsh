#ifndef REDIR_H
#define REDIR_H

// ***=== Inclusions ===***
#include "internal_cmds.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>

/**
 * 
 * 
*/
int handle_input_redirection(char *line);

/**
 * 
 * 
*/
int redir_stdin(char *line);

/**
 * 
 * 
*/
int is_redirection(char *line);

/**
 * 
 * 
*/
int redir_handler(char *line);

/**
 * 
 * 
*/
int redir_stdout(char *line);

/**
 * 
 * 
*/
int redir_stdout_trunc(char *line);

/**
 * 
 * 
*/
int redir_stdout_append(char *line);

#endif
