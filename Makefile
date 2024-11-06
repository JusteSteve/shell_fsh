CC = gcc
CFLAGS = -Wall -g 

# fichiers sources
SRCS = src/main.c 

# fichiers objets
OBJETS = $(SRCS:src/%.c=objects/%.o)

# exécution de `make` crée l'exécutable `fsh`

all: fsh

fsh: $(OBJETS)
	gcc $(CFLAGS) $< -o $@

$(OBJETS): $(SRCS) 
	@mkdir -p objects
	gcc $(CFLAGS) -c $< -o $@


# `make clean` supprime tous les fichiers compilés
clean:
	rm -rf objects fsh .sy5-2024-2025-projet-fsh-autotests.nosync

.PHONY: all clean
