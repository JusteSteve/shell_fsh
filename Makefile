# Makefile
CC = gcc
CFLAGS = -Wall -g -Werror -Wextra -D_DEFAULT_SOURCE -I./include

# fichiers sources
SRCS = src/main.c \
	src/autres/prompt.c \
	src/commandes/internes/pwd.c \
	src/commandes/internes/cd.c \
	src/commandes/internes/exit.c \
	src/commandes/internes/ftype.c \
	src/commandes/external_cmds.c \
	src/commandes/cmd_utils.c \
	src/redirections/stdin/redir_stdin.c \
	src/redirections/stdout/redir_stdout.c \
	src/redirections/stdout/redir_stdout_trunc.c \
	src/redirections/stdout/redir_stdout_append.c \
	src/redirections/stderr/redir_stderr.c \
	src/redirections/stderr/redir_stderr_trunc.c \
	src/redirections/redir_handler.c \
	src/structures/str-cmd-for.c \
	src/structures/str-cmd.c 


# fichiers objets
OBJDIR = objects
OBJETS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

# exécution de `make` crée l'exécutable `fsh`
all: fsh

# Crée l'exécutable fsh
fsh: $(OBJETS)
	gcc $(CFLAGS) $(OBJETS) -o $@ -lreadline

$(OBJDIR)/%.o: %.c 
	@mkdir -p $(@D)
	gcc $(CFLAGS) -c $< -o $@

test :
	./test.sh

# `make clean` supprime tous les fichiers compilés
clean:
	rm -rf objects fsh .sy5-2024-2025-projet-fsh-autotests.nosync

.PHONY: all clean test	
