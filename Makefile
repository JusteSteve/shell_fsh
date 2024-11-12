CFLAGS = -Wall -g -Werror -Wextra  -I./include

# fichiers sources
SRCS = src/main.c src/prompt.c 

# fichiers objets
OBJDIR = objects
OBJETS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

# exécution de `make` crée l'exécutable `fsh`
all: fsh

fsh: $(OBJETS)
	@echo $(OBJETS)
	gcc $(CFLAGS) $(OBJETS) -o $@ -lreadline

$(OBJDIR)/%.o: %.c 
	@echo $^
	@mkdir -p $(@D)
	gcc $(CFLAGS) -c $< -o $@




# `make clean` supprime tous les fichiers compilés
clean:
	rm -rf objects fsh .sy5-2024-2025-projet-fsh-autotests.nosync

.PHONY: all clean
