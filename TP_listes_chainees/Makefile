CONF ?= immutable.mk
include $(CONF)

FLAGS= -Wall -Wextra -fsanitize=address
CC= gcc

OBJ_DIR = ./obj/

MAIN = applications.c
EXEC = build.exe

#INCLUDES ?= $(filter-out $(MAIN), $(wildcard *.c))
OBJ_FILES := $(patsubst %.c, $(OBJ_DIR)%.o, $(INCLUDES))

all: $(EXEC)

$(OBJ_FILES): $(INCLUDES)
	$(CC) $(FLAGS) -c -o $(@) $(<)

$(EXEC): $(MAIN) $(OBJ_FILES)
	$(CC) $(FLAGS) $(^) -o $(EXEC)

.PHONY: clean clean-all list mutable immutable all

mutable:
	@$(MAKE) all CONF=mutable.mk

immutable:
	@$(MAKE) all CONF=immutable.mk

clean:
	rm -f $(OBJ_FILES)
	@echo Removed successfuly: $(OBJ_FILES)

clean-all: clean
	@rm -f $(EXEC)
	@echo Removed successfuly: $(EXEC)

list:
	@echo $(INCLUDES)
	@echo $(OBJ_FILES)
