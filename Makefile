
CFLAGS = 

NAME = webserv

FILES  = main.c

OBJS = $(FILES:.c=.o)

all:	$(NAME)

$(NAME): $(OBJS)
	cc $(CFLAGS) $(OBJS)-o $(NAME)

$(OBJS): $(SRC)
	cc $(CFLAGS) -c $(SRC)


clean: 
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
