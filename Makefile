# Makefile corrigé — échappe correctement $< $@ $^ dans les eval
SHELL := /bin/bash

CC := gcc
# Ajuste CFLAGS si besoin ; retire -fsanitize=address si tu ne veux pas ASan.
CFLAGS := -g -Wall -Wextra -fsanitize=address
CPPFLAGS := -Iincludes -I.
DEPFLAGS := -MMD -MP
LDFLAGS := -lm

INCDIR := includes

# ---------------------------
# Library: headers and prefer .c if present
# ---------------------------
LIB_HEADERS := $(wildcard $(INCDIR)/*.h)
LIB_SOURCES := $(foreach h,$(LIB_HEADERS),$(if $(wildcard $(h:.h=.c)),$(h:.h=.c),$(h)))
LIB_OBJS := $(patsubst %.c,%.o,$(patsubst %.h,%.o,$(LIB_SOURCES)))

# ---------------------------
# Other sources (exclude includes/ and any build/ dirs)
# ---------------------------
SRC_SOURCES := $(shell find . -type f -name '*.c' -not -path "./$(INCDIR)/*" -not -path "*/build/*" -print)
SRC_SOURCES := $(patsubst ./%,%,$(SRC_SOURCES))

# object and exe paths for each source
SRC_OBJS := $(foreach s,$(SRC_SOURCES),$(dir $(s))build/$(notdir $(s:.c=.o)))
EXES     := $(foreach s,$(SRC_SOURCES),$(dir $(s))build/$(notdir $(s:.c=.exe)))

# build dirs (normalized, without trailing slash)
BUILD_DIRS := $(sort $(patsubst %/,%,$(dir $(SRC_OBJS))))
ALL_DIRS := $(BUILD_DIRS) $(INCDIR)

# dependency files
DEPS := $(LIB_OBJS:.o=.d) $(SRC_OBJS:.o=.d)

# directories with sources (for "make <dir>" convenience)
SRC_DIRS := $(sort $(dir $(SRC_SOURCES)))
DIR_TARGETS := $(sort $(patsubst %/,%,$(SRC_DIRS)))

.PHONY: all clean list $(DIR_TARGETS)

# Default: build all exes
all: $(EXES)

# ---------------------------
# Ensure build dirs and includes dir exist (explicit rules)
# ---------------------------
$(foreach d,$(ALL_DIRS),$(eval $(d): ; @mkdir -p $(d)))

# ---------------------------
# Generate explicit rules for library objects (includes/)
# For each entry in LIB_SOURCES generate either:
#   includes/name.o: includes/name.c   (compile .c)
# or
#   includes/name.o: includes/name.h   (compile header as TU)
# Use $$@ to defer expansion to recipe-execution time.
# ---------------------------
$(foreach src,$(LIB_SOURCES), \
  $(eval _obj := $(patsubst %.c,%.o,$(patsubst %.h,%.o,$(src)))) \
  $(if $(filter %.c,$(src)), \
    $(eval $(_obj): $(src) | $(INCDIR) ; $(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $$< -o $$@) , \
    $(eval $(_obj): $(src) | $(INCDIR) ; $(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -x c -c $$< -o $$@) \
  ) \
)

# ---------------------------
# Generate explicit rules for each source file (non includes)
# For each src:
#   dir/build/name.o : dir/name.c
#   dir/build/name.exe: dir/build/name.o $(LIB_OBJS)
# Use $$< $$@ $$^ inside eval recipes.
# ---------------------------
$(foreach src,$(SRC_SOURCES), \
  $(eval _src := $(src)) \
  $(eval _dir := $(patsubst %/,%,$(dir $(_src)))) \
  $(eval _obj := $(_dir)/build/$(notdir $(_src:.c=.o))) \
  $(eval _exe := $(_dir)/build/$(notdir $(_src:.c=.exe))) \
  \
  $(eval $(_obj): $(_src) | $(_dir)/build ; $(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $$< -o $$@) \
  \
  $(eval $(_exe): $(_obj) $(LIB_OBJS) | $(_dir)/build ; $(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $$^ -o $$@) \
)

# Root-level build dir (if any top-level .c exist)
build:
	@mkdir -p build

# ---------------------------
# Provide "make <directory>" convenience targets
# ---------------------------
$(foreach d,$(sort $(patsubst %/,%,$(SRC_DIRS))), \
  $(eval $(d): ; @$(MAKE) $(filter $(d)%,$(EXES))) \
)

# ---------------------------
# Include dependency files (silent if missing)
# ---------------------------
-include $(DEPS)

# ---------------------------
# Utilities
# ---------------------------
list:
	@echo "INCDIR:       $(INCDIR)"
	@echo "LIB_HEADERS:  $(LIB_HEADERS)"
	@echo "LIB_SOURCES:  $(LIB_SOURCES)"
	@echo "LIB_OBJS:     $(LIB_OBJS)"
	@echo "SRC_SOURCES:  $(SRC_SOURCES)"
	@echo "SRC_OBJS:     $(SRC_OBJS)"
	@echo "EXES:         $(EXES)"
	@echo "BUILD_DIRS:   $(BUILD_DIRS)"
	@echo "DIR_TARGETS:  $(DIR_TARGETS)"
	@echo "DEPS:         $(DEPS)"

clean:
	@echo "Cleaning build directories, includes objects and dependency files..."
	@rm -rf $(BUILD_DIRS)
	@rm -f $(LIB_OBJS) $(DEPS)
	@echo "Done."
