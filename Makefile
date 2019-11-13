EXE=dns

SRC_DIR := src
OBJ_DIR := obj

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CFLAGS += -std=c++11 -pedantic -Wall -Wextra
LDFLAGS += -std=c++11
LDLIBS += -lstdc++

.PHONY: all debug clean test

all: $(EXE)

debug: CFLAGS += -DDEBUG -g
debug: all

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: debug
	@./test/test.py

clean:
	$(RM) $(OBJ)
