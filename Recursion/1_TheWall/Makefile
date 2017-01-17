WALL	= thewall
CORR	= correction

CC	= gcc

CXXFLAGS += -Wall -Wextra -Werror
CXXFLAGS += -I./include/

SRCDIR = ./src/
OBJDIR = ./obj/

SRC_WALL=	$(SRCDIR)main.c	\
		$(SRCDIR)funcs.c \
		$(SRCDIR)findpath.c

SRC_CORR=	$(SRCDIR)main_correct.c	\
		$(SRCDIR)funcs.c	\
		$(SRCDIR)correct_map.c

OBJ_WALL= $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(SRC_WALL))
OBJ_CORR= $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(SRC_CORR))

all: $(WALL) $(CORR)

$(WALL): $(OBJ_WALL)
	$(CC) -o $(WALL) $(OBJ_WALL) $(CXXFLAGS)

$(CORR): $(OBJ_CORR)
	$(CC) -o $(CORR) $(OBJ_CORR) $(CXXFLAGS)

$(OBJ_WALL): | $(OBJDIR)
$(OBJ_CORR): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)%.o : $(SRCDIR)%.c
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(WALL) $(CORR)

re: fclean all

.PHONY: all clean fclean re
