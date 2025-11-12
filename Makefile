# Makefile mis à jour pour C et OCaml
SHELL := /bin/bash

# ---------------------------
# C Toolchain
# ---------------------------
CC := gcc
# Ajuste CFLAGS si besoin ; retire -fsanitize=address si tu ne veux pas ASan.
CFLAGS := -g -Wall -Wextra -fsanitize=address
CPPFLAGS := -Iincludes -I.
DEPFLAGS := -MMD -MP
LDFLAGS := -lm

# ---------------------------
# OCaml Toolchain
# ---------------------------
OCAMLOPT := ocamlopt
# -I $(INCDIR) est utile si OCaml utilise des "external" vers la lib C
OCAMLFLAGS := -g -I $(INCDIR)
# Utilise les mêmes LDFLAGS (ex: -lm) pour la liaison C
OCAMLLDFLAGS := $(LDFLAGS)

INCDIR := includes

# ---------------------------
# Library (C): headers and prefer .c if present
# ---------------------------
LIB_HEADERS := $(wildcard $(INCDIR)/*.h)
LIB_SOURCES := $(foreach h,$(LIB_HEADERS),$(if $(wildcard $(h:.h=.c)),$(h:.h=.c),$(h)))
LIB_OBJS := $(patsubst %.c,%.o,$(patsubst %.h,%.o,$(LIB_SOURCES)))

# ---------------------------
# C sources (exclude includes/ and any build/ dirs)
# ---------------------------
SRC_SOURCES := $(shell find . -type f -name '*.c' -not -path "./$(INCDIR)/*" -not -path "*/build/*" -print)
SRC_SOURCES := $(patsubst ./%,%,$(SRC_SOURCES))

# object and exe paths for each C source
SRC_OBJS := $(foreach s,$(SRC_SOURCES),$(dir $(s))build/$(notdir $(s:.c=.o)))
EXES     := $(foreach s,$(SRC_SOURCES),$(dir $(s))build/$(notdir $(s:.c=.exe)))

# ---------------------------
# OCaml sources (exclude includes/ and any build/ dirs)
# ---------------------------
ML_SOURCES := $(shell find . -type f -name '*.ml' -not -path "./$(INCDIR)/*" -not -path "*/build/*" -print)
ML_SOURCES := $(patsubst ./%,%,$(ML_SOURCES))

# object and exe paths for each OCaml source
# Note: .ml.exe pour éviter les conflits avec les .exe C
ML_OBJS_CMX := $(foreach s,$(ML_SOURCES),$(dir $(s))build/$(notdir $(s:.ml=.cmx)))
ML_OBJS_CMI := $(foreach s,$(ML_SOURCES),$(dir $(s))build/$(notdir $(s:.ml=.cmi)))
ML_EXES     := $(foreach s,$(ML_SOURCES),$(dir $(s))build/$(notdir $(s:.ml=.ml.exe)))

# ---------------------------
# Common paths and targets
# ---------------------------
# build dirs (normalized, without trailing slash)
C_BUILD_DIRS := $(sort $(patsubst %/,%,$(dir $(SRC_OBJS))))
ML_BUILD_DIRS := $(sort $(patsubst %/,%,$(dir $(ML_OBJS_CMX))))
BUILD_DIRS := $(sort $(C_BUILD_DIRS) $(ML_BUILD_DIRS))
ALL_DIRS := $(BUILD_DIRS) $(INCDIR)

# dependency files (C only)
DEPS := $(LIB_OBJS:.o=.d) $(SRC_OBJS:.o=.d)

# directories with sources (for "make <dir>" convenience)
C_SRC_DIRS := $(sort $(dir $(SRC_SOURCES)))
ML_SRC_DIRS := $(sort $(dir $(ML_SOURCES)))
DIR_TARGETS := $(sort $(patsubst %/,%,$(C_SRC_DIRS)) $(patsubst %/,%,$(ML_SRC_DIRS)))

.PHONY: all clean list $(DIR_TARGETS)

# Default: build all exes
all: $(EXES) $(ML_EXES)

# ---------------------------
# Ensure build dirs and includes dir exist (explicit rules)
# ---------------------------
$(foreach d,$(ALL_DIRS),$(eval $(d): ; @mkdir -p $(d)))

# ---------------------------
# Generate explicit rules for C library objects (includes/)
# ---------------------------
$(foreach src,$(LIB_SOURCES), \
  $(eval _obj := $(patsubst %.c,%.o,$(patsubst %.h,%.o,$(src)))) \
  $(if $(filter %.c,$(src)), \
    $(eval $(_obj): $(src) | $(INCDIR) ; $(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $$< -o $$@) , \
    $(eval $(_obj): $(src) | $(INCDIR) ; $(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -x c -c $$< -o $$@) \
  ) \
)

# ---------------------------
# Generate explicit rules for each C source file (non includes)
# For each src:
#   dir/build/name.o : dir/name.c
#   dir/build/name.exe: dir/build/name.o $(LIB_OBJS)
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

# ---------------------------
# Generate explicit rules for each OCaml source file (non includes)
# For each src:
#   dir/build/name.cmx : dir/name.ml  (also makes .cmi)
#   dir/build/name.ml.exe: dir/build/name.cmx $(LIB_OBJS)
# ---------------------------
$(foreach src,$(ML_SOURCES), \
  $(eval _src := $(src)) \
  $(eval _dir := $(patsubst %/,%,$(dir $(_src)))) \
  $(eval _obj_cmx := $(_dir)/build/$(notdir $(_src:.ml=.cmx))) \
  $(eval _obj_cmi := $(_dir)/build/$(notdir $(_src:.ml=.cmi))) \
  $(eval _exe := $(_dir)/build/$(notdir $(_src:.ml=.ml.exe))) \
  \
  $(eval $(_obj_cmx): $(_src) | $(_dir)/build ; $(OCAMLOPT) $(OCAMLFLAGS) -c $$< -o $$@) \
  \
  $(eval $(_exe): $(_obj_cmx) $(LIB_OBJS) | $(_dir)/build ; $(OCAMLOPT) $(OCAMLFLAGS) $$^ $(OCAMLLDFLAGS) -o $$@) \
)

# ---------------------------
# Provide "make <directory>" convenience targets
# ---------------------------
$(foreach d,$(sort $(patsubst %/,%,$(C_SRC_DIRS)) $(patsubst %/,%,$(ML_SRC_DIRS))), \
  $(eval $(d): ; @$(MAKE) $(filter $(d)%,$(EXES)) $(filter $(d)%,$(ML_EXES))) \
)

# ---------------------------
# Include dependency files (C only - silent if missing)
# ---------------------------
-include $(DEPS)

# ---------------------------
# Utilities
# ---------------------------
list:
	@echo "--- C Build ---"
	@echo "INCDIR:       $(INCDIR)"
	@echo "LIB_HEADERS:  $(LIB_HEADERS)"
	@echo "LIB_SOURCES:  $(LIB_SOURCES)"
	@echo "LIB_OBJS:     $(LIB_OBJS)"
	@echo "SRC_SOURCES:  $(SRC_SOURCES)"
	@echo "SRC_OBJS:     $(SRC_OBJS)"
	@echo "EXES:         $(EXES)"
	@echo "DEPS (C):     $(DEPS)"
	@echo ""
	@echo "--- OCaml Build ---"
	@echo "ML_SOURCES:   $(ML_SOURCES)"
	@echo "ML_OBJS_CMX:  $(ML_OBJS_CMX)"
	@echo "ML_OBJS_CMI:  $(ML_OBJS_CMI)"
	@echo "ML_EXES:      $(ML_EXES)"
	@echo ""
	@echo "--- Common ---"
	@echo "BUILD_DIRS:   $(BUILD_DIRS)"
	@echo "DIR_TARGETS:  $(DIR_TARGETS)"

clean:
	@echo "Cleaning build directories, includes objects and dependency files..."
    # Supprime tous les produits de build (.o, .cmx, .cmi, .exe, .ml.exe) dans les build/ dirs
	@rm -rf $(BUILD_DIRS)
    # Supprime la lib C et les dépendances C
	@rm -f $(LIB_OBJS) $(DEPS)
	@echo "Done."