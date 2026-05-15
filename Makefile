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

ARGS ?= 5 800 200 200 200 5 50 fifo

run: all
	./$(NAME) $(ARGS)

valgrind: all
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all \
		./$(NAME) $(ARGS)

helgrind: all
	valgrind --tool=helgrind ./$(NAME) $(ARGS)

docker-build:
	docker build -t codexion-valgrind .

docker-helgrind: docker-build
	docker run --rm -v $(PWD):/app codexion-valgrind make re helgrind ARGS="$(ARGS)"

docker-valgrind: docker-build
	docker run --rm -v $(PWD):/app codexion-valgrind make re valgrind ARGS="$(ARGS)"

.PHONY: all clean fclean re run valgrind helgrind docker-build docker-helgrind docker-valgrind
