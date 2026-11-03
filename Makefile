# =============================================================================
#  Projet C — C11 strict, sans VLA
#
#  Un fichier = un programme.
#  Chaque src/<nom>.c produit son propre exécutable bin/<mode>/<nom>.
#  Le code partagé entre plusieurs exercices va dans lib/ (sans main).
#
#  Usage :  make                  compile tous les programmes
#           make run FILE=exo-01  compile puis exécute src/exo-01.c
#           make list             liste les programmes disponibles
#           make help             toutes les cibles
# =============================================================================

SRC_DIR   := src
LIB_DIR   := lib
INC_DIR   := include

# ---- Mode de compilation ----------------------------------------------------
# debug (défaut) | release | san
MODE      ?= debug

ifeq ($(MODE),debug)
  MODE_CFLAGS  := -g3 -O0 -fno-omit-frame-pointer
  MODE_LDFLAGS :=
else ifeq ($(MODE),release)
  MODE_CFLAGS  := -O2 -DNDEBUG
  MODE_LDFLAGS :=
else ifeq ($(MODE),san)
  MODE_CFLAGS  := -g3 -O1 -fno-omit-frame-pointer -fsanitize=address,undefined
  MODE_LDFLAGS := -fsanitize=address,undefined
else
  $(error MODE invalide : « $(MODE) » (attendu : debug, release ou san))
endif

BUILD_DIR := build/$(MODE)
BIN_DIR   := bin/$(MODE)

# ---- Chaîne de compilation --------------------------------------------------
CC        := gcc

# -std=c11 : C11 « pur » (et non gnu11) — pas d'extensions GNU.
# -Wvla + -Werror=vla : les tableaux à taille variable deviennent une ERREUR.
STD       := -std=c11
WARNINGS  := -Wall -Wextra -Wpedantic -Wshadow -Wvla -Werror=vla -Wvla-parameter

CPPFLAGS  := -I$(INC_DIR) -MMD -MP
CFLAGS    := $(STD) $(WARNINGS) $(MODE_CFLAGS)
LDFLAGS   := $(MODE_LDFLAGS)
LDLIBS    := -lm

# ---- Sources ----------------------------------------------------------------
# Tout nouveau .c déposé dans src/ devient automatiquement un programme.
SRCS      := $(wildcard $(SRC_DIR)/*.c)
LIB_SRCS  := $(wildcard $(LIB_DIR)/*.c)

SRC_OBJS  := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/src/%.o,$(SRCS))
LIB_OBJS  := $(patsubst $(LIB_DIR)/%.c,$(BUILD_DIR)/lib/%.o,$(LIB_SRCS))
BINS      := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(SRCS))
DEPS      := $(SRC_OBJS:.o=.d) $(LIB_OBJS:.o=.d)

# Programme ciblé par run / debug / memcheck.
# Accepte « exo-01 », « exo-01.c » ou « src/exo-01.c ».
FILE      ?= main
PROG      := $(basename $(notdir $(FILE)))

# Arrête tôt, avec un message lisible, si FILE ne désigne aucun fichier.
ifneq ($(filter run debug memcheck,$(MAKECMDGOALS)),)
  ifeq ($(wildcard $(SRC_DIR)/$(PROG).c),)
    $(error $(SRC_DIR)/$(PROG).c introuvable — lancez « make list » pour voir les programmes disponibles)
  endif
endif

# Arguments passés au programme
ARGS      ?=

# -----------------------------------------------------------------------------
.PHONY: all run debug memcheck list clean fclean re help
.DEFAULT_GOAL := all

# Conserve les .o : sans ça make les traite comme intermédiaires, les supprime
# après l'édition de liens, et recompile tout à chaque appel.
.SECONDARY: $(SRC_OBJS) $(LIB_OBJS)

all: $(BINS)

# Édition de liens : un objet src/ + tout le code partagé de lib/
$(BIN_DIR)/%: $(BUILD_DIR)/src/%.o $(LIB_OBJS)
	@mkdir -p $(dir $@)
	@echo "  LD      $@"
	@$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "  CC      $<"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/lib/%.o: $(LIB_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "  CC      $<"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: $(BIN_DIR)/$(PROG)
	@echo "  RUN     $(BIN_DIR)/$(PROG)"
	@./$(BIN_DIR)/$(PROG) $(ARGS)

debug: $(BIN_DIR)/$(PROG)
	@gdb --quiet --args ./$(BIN_DIR)/$(PROG) $(ARGS)

memcheck: $(BIN_DIR)/$(PROG)
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	          --error-exitcode=1 ./$(BIN_DIR)/$(PROG) $(ARGS)

list:
	@echo "Programmes dans $(SRC_DIR)/ :"
	@for s in $(SRCS); do echo "  $$(basename $$s .c)"; done
	@if [ -n "$(LIB_SRCS)" ]; then \
	    echo "Code partagé dans $(LIB_DIR)/ :"; \
	    for s in $(LIB_SRCS); do echo "  $$(basename $$s)"; done; \
	fi

clean:
	@echo "  CLEAN   build/"
	@rm -rf build

fclean: clean
	@echo "  CLEAN   bin/"
	@rm -rf bin

re: fclean all

help:
	@echo "Un fichier src/<nom>.c = un programme bin/$(MODE)/<nom>"
	@echo
	@echo "  make                      compile tous les programmes"
	@echo "  make bin/$(MODE)/exo-01    compile seulement celui-là"
	@echo "  make run FILE=exo-01      compile puis exécute (ARGS=\"...\")"
	@echo "  make debug FILE=exo-01    compile puis lance gdb"
	@echo "  make memcheck FILE=exo-01 exécute sous valgrind"
	@echo "  make list                 liste les programmes"
	@echo "  make MODE=san             build instrumenté ASan/UBSan"
	@echo "  make MODE=release         build optimisé"
	@echo "  make clean / fclean / re"
	@echo
	@echo "FILE accepte « exo-01 », « exo-01.c » ou « src/exo-01.c »."

-include $(DEPS)
