# Makefile
CC = gcc
CFLAGS = -Wall -g

# fichiers sources
SRCS = src/main.c src/pwd.c src/cd.c src/exit.c src/ftype.c

# fichiers objets
OBJETS = $(SRCS:src/%.c=objects/%.o)

# exécution de `make` crée l'exécutable `fsh`
all: fsh

# Crée l'exécutable fsh
fsh: $(OBJETS)
	$(CC) $(CFLAGS) $(OBJETS) -o fsh

# Compilation des fichiers .c en fichiers .o
objects/%.o: src/%.c
	@mkdir -p objects  # Crée le dossier objects si nécessaire
	$(CC) $(CFLAGS) -c $< -o $@

# `make clean` supprime tous les fichiers compilés
clean:
	rm -rf objects fsh .sy5-2024-2025-projet-fsh-autotests.nosync

.PHONY: all clean
