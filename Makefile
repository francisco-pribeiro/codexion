NAME	= codexion
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -pthread
SRCDIR = src

SRCS = $(SRCDIR)/main.c $(SRCDIR)/parser.c $(SRCDIR)/monitor.c \
       $(SRCDIR)/queue.c $(SRCDIR)/coder.c $(SRCDIR)/utils.c \
	   $(SRCDIR)/dongle.c $(SRCDIR)/simulation.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
