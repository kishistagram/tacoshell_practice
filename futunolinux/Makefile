# ---- Minishell Makefile ----

NAME    := minishell

# dirs
SRCDIR  := src
INCDIR  := include
OBJDIR  := .obj

# toolchain
CC      := cc
CSTD    := -std=c11
WARN    := -Wall -Wextra -Werror
OPT     := -O2
CPPFLAGS:= -I$(INCDIR)
CFLAGS  := $(CSTD) $(WARN) $(OPT) $(CPPFLAGS) -MMD -MP
LDFLAGS :=
LDLIBS  :=

# sources/objects
SRC     := $(wildcard $(SRCDIR)/*.c)
OBJ     := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
DEP     := $(OBJ:.o=.d)

# default
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $@

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

# quick helpers
run: $(NAME)
	./$(NAME)

debug: CFLAGS += -g -O0
debug: re

asan:  CFLAGS += -fsanitize=address -fno-omit-frame-pointer -g
asan:  LDFLAGS += -fsanitize=address
asan:  re

-include $(DEP)

.PHONY: all clean fclean re run debug asan
