NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
INCLUDES	= -I includes

SRCS		= main.c \
			  parsing.c \
			  init.c \
			  init_helpers.c \
			  cleanup.c \
			  simulation.c \
			  monitor.c \
			  dongle.c \
			  dongle_helpers.c \
			  heap.c \
			  heap_helpers.c \
			  logging.c \
			  time.c \
			  utils.c

OBJ_DIR		= obj
OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

HEADERS		= includes/codexion.h includes/types.h includes/heap.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
