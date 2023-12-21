CC = cc
CFLAGS = -Iinclude
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# List all source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

TARGET = $(BIN_DIR)/test

# Main target
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: run

run: $(TARGET)
	./$(TARGET)


# Phony target for cleaning
.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(TARGET)

