# Top-level Makefile
# Usage: make      -> builds all exes
#        make list -> prints found sources and targets
#        make clean-> removes all build/ directories

CC := gcc
INCDIR := includes
CFLAGS := -g -Wall -Wextra -fsanitize=address -I$(INCDIR)
LIBFLAGS := -lm

# find all .c files but skip includes/ and any build/ directories
SOURCES := $(shell find . -type f -name '*.c' -not -path './$(INCDIR)/*' -not -path '*/build/*' -print)
# normalize leading ./ if present
SOURCES := $(patsubst ./%,%,$(SOURCES))

# for each src like dir/file.c produce dir/build/file.exe
EXES := $(foreach s,$(SOURCES),$(dir $(s))build/$(notdir $(s:.c=.exe)))

# find all header file in includes/file.h
HEADERS := $(wildcard includes/*)

# unique list of build directories (without trailing slash)
BUILD_DIRS := $(sort $(patsubst %/,%,$(dir $(EXES))))

.PHONY: all clean list

all: $(EXES)

$(EXES): $(HEADERS)

# create build dirs (one rule per dir)
$(foreach d,$(BUILD_DIRS),$(eval $(d): ; mkdir -p $(d)))

# helper to create per-source compile rule:
# target : source | build-dir
#     $(CC) $(CFLAGS) -o $@ $<
define MK_RULE
$1: $2 | $3
	$(CC) $(CFLAGS) -o $$@ $$< $(HEADERS) $(LIBFLAGS)
endef

# emit a rule for each source -> target
$(foreach s,$(SOURCES), \
  $(eval TARGET := $(dir $(s))build/$(notdir $(s:.c=.exe))) \
  $(eval DIR := $(patsubst %/,%,$(dir $(TARGET)))) \
  $(eval $(call MK_RULE,$(TARGET),$(s),$(DIR))) \
)

list:
	@printf "Sources:\n"
	@printf "%s\n" $(SOURCES)
	@printf "\nTargets (executables):\n"
	@printf "%s\n" $(EXES)

clean:
	@echo "Removing all build/ directories..."
	@rm -rf $(BUILD_DIRS)
