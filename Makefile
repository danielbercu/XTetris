CC := gcc
CFLAGS := -ansi -Wall -Wextra -Wpedantic
INCLUDE := -I include

SRC_DIR := src
OBJ_DIR := obj
EXE_DIR := build
DOCS_DIR := docs

EXE := $(EXE_DIR)/xtetris

xtetris: main.o game.o field.o tetramino.o utilities.o cpu.o
	mkdir $(EXE_DIR)
	$(CC) $(OBJ_DIR)/main.o $(OBJ_DIR)/game.o $(OBJ_DIR)/field.o $(OBJ_DIR)/tetramino.o $(OBJ_DIR)/utilities.o $(OBJ_DIR)/cpu.o -lm $(CFLAGS) -o $(EXE)
	doxygen

main.o:
	mkdir $(OBJ_DIR)
	$(CC) -c $(SRC_DIR)/main.c $(INCLUDE) $(CFLAGS) -o $(OBJ_DIR)/main.o

game.o:
	$(CC) -c $(SRC_DIR)/game.c $(INCLUDE) $(CFLAGS) -o $(OBJ_DIR)/game.o

field.o:
	$(CC) -c $(SRC_DIR)/field.c $(INCLUDE) $(CFLAGS) -o $(OBJ_DIR)/field.o

tetramino.o:
	$(CC) -c $(SRC_DIR)/tetramino.c $(INCLUDE) $(CFLAGS) -o $(OBJ_DIR)/tetramino.o

utilities.o:
	$(CC) -c $(SRC_DIR)/utilities.c $(INCLUDE) $(CFLAGS) -o $(OBJ_DIR)/utilities.o

cpu.o:
	$(CC) -c $(SRC_DIR)/cpu.c $(INCLUDE) $(CFLAGS) -o $(OBJ_DIR)/cpu.o

clean:
	rm -r $(OBJ_DIR) $(EXE_DIR) $(DOCS_DIR)
