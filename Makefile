NAME = ft_ls

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes


SRCS_DIR	= srcs
OBJS_DIR	= objs

SRCS		= main.c \
			  options.c \
			  operands.c \
			  traverse.c \
			  display.c \
			  sort.c \
			  utils.c

OBJS		= $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
HEADER		= includes/ft_ls.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADER) | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
