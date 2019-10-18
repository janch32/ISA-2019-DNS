NAME=dns
SRCFOLDER := src
OBJFOLDER := obj
SRCFILES := $(wildcard $(SRCFOLDER)/*.cpp)
OBJFILES := $(patsubst %.cpp,$(OBJFOLDER)/%.o,$(notdir $(SRCFILES)))
CC=gcc
CFLAGS= -std=c++11 -pedantic -Wall -Wextra -g

# Startovací pravidlo - pro přehlednost
all: dep $(NAME)

# vzorové pravidlo pro generování všech objektových souborů
$(OBJFOLDER)/%.o : $(SRCFOLDER)/%.cpp
	@mkdir -p $(OBJFOLDER)
	@$(CC) $(CFLAGS) -c -o $@ $<


clean:
	@rm -rf $(OBJFOLDER)/
	@rm -f $(NAME)
	@rm -f dep.list

# Generování závislostí
dep:
	@$(CC) -MM $(SRCFOLDER)/*.cpp >dep.list

-include dep.list

# závěrečné slinkování
$(NAME): $(OBJFILES)
	@$(CC) $(CFLAGS) $(OBJFILES) -lstdc++ -o $@
