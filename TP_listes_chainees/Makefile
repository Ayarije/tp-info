
FLAGS= -Wall -Wextra -fsanitize=address
CC= gcc

OBJ_DIR = ./obj/

MAIN = applications.c
EXEC = build.exe

INCLUDES ?= $(filter-out $(MAIN), $(wildcard *.c))
include $(INCLUDES)

OBJ_FILES := $(patsubst %.c, $(OBJ_DIR)%.o, $(INCLUDES))

all: $(EXEC)

$(OBJ_FILES): $(INCLUDES)
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
	@echo $(INCLUDES)
	@echo $(OBJ_FILES)
