CC = cc
NAME = server
# CFLAGS = -Wall -Wextra -Werror -fsanitize=address
# LDFLAGS = -fsanitize=address -I/usr/include/mysql -lmysqlclient
CFLAGS = -Wall -Wextra -Werror -lmysqlclient

INCS = incs
SRCS = $(wildcard srcs/*.c)
OBJS = $(SRCS:.c=.o)

.c.o :
	$(CC) -c $(CFLAGS) -o $@ $^ -I $(INCS)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(CFLAGS) $(LDFLAGS)

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re :
	make fclean
	make all

only :
	make re
	make clean

.PHONY : all clean fclean re .c.o
