
FLAGS= -Wall -Wextra -fsanitize=address
CC= gcc

OBJ_DIR = ./obj/

MAIN = main.c
EXEC = build.exe

SRC_FILES := $(filter-out $(MAIN), $(wildcard *.c))
OBJ_FILES := $(patsubst %.c, $(OBJ_DIR)%.o, $(SRC_FILES))

all: $(EXEC)

$(OBJ_FILES): $(SRC_FILES)
	$(CC) $(FLAGS) -c -o $(@) $(<)

$(EXEC): $(MAIN) $(OBJ_FILES)
	@echo $(^)
	$(CC) $(FLAGS) $(^) -o $(EXEC)

clean:
	rm -f $(OBJ_FILES)
	@echo Removed successfuly: $(OBJ_FILES)

clean-all: clean
	@rm -f $(EXEC)
	@echo Removed successfuly: $(EXEC)

list:
	@echo $(SRC_FILES)
	@echo $(OBJ_FILES)
