
FLAGS= -Wall -Wextra -fsanitize=address
CC= gcc
ML = ocamlc

OBJ_DIR = ./obj/

MAIN_C = main.c
C_EXEC = c_build.exe

MAIN_ML = main.ml
ML_EXEC = ml_build.exe

C_SRC_FILES := $(filter-out $(MAIN_FILE), $(wildcard *.c))
C_OBJ_FILES := $(patsubst %.c, $(OBJ_DIR)%.o, $(C_SRC_FILES))

all: $(C_EXEC)

$(OBJ_FILES): $(SRC_FILES)
	$(CC) $(FLAGS) -c -o $(@) $(<)

$(EXEC): $(MAIN_FILE) $(OBJ_FILES)
	@echo $(^)
	$(CC) $(FLAGS) $(^) -o $(EXEC)

clean:
	rm -f $(OBJ_FILES)
	@echo Removed successfuly: $(OBJ_FILES)

clean-all: clean
	@rm -f $(C_EXEC)
	@echo Removed successfuly: $(EXEC)

list:
	@echo $(SRC_FILES)
	@echo $(OBJ_FILES)
