NAME=dns
SRCFOLDER := src
OBJFOLDER := obj
SRCFILES := $(wildcard $(SRCFOLDER)/*.c)
OBJFILES := $(patsubst %.c,$(OBJFOLDER)/%.o,$(notdir $(SRCFILES)))
CC=gcc
CFLAGS=-pedantic -Wall -Wextra -g

# Startovací pravidlo - pro přehlednost
all: dep $(NAME)

# vzorové pravidlo pro generování všech objektových souborů
$(OBJFOLDER)/%.o : $(SRCFOLDER)/%.c
	@mkdir -p $(OBJFOLDER)
	@$(CC) $(CFLAGS) -c -o $@ $<


clean:
	@rm -rf $(OBJFOLDER)/
	@rm -f $(NAME)
	@rm -f dep.list

# Generování závislostí
dep:
	@$(CC) -MM $(SRCFOLDER)/*.c >dep.list

-include dep.list

# závěrečné slinkování
$(NAME): $(OBJFILES)
	@$(CC) $(CFLAGS) $(OBJFILES) -o $@
