# Makefile
CC = gcc
CFLAGS = -Wall -g -Werror -Wextra -D_DEFAULT_SOURCE -I./include

# fichiers sources
SRCS = src/main.c src/prompt.c src/pwd.c src/cd.c src/exit.c src/ftype.c src/ls.c src/cat.c src/external_cmds.c src/cmd_utils.c src/for.c fsh_core/commands.c

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

# Compilation des fichiers .c en fichiers .o

#objects/%.o: src/%.c
#	@mkdir -p objects  # Crée le dossier objects si nécessaire
#	$(CC) $(CFLAGS) -c $< -o $@

# `make clean` supprime tous les fichiers compilés
clean:
	rm -rf objects fsh .sy5-2024-2025-projet-fsh-autotests.nosync

.PHONY: all clean
