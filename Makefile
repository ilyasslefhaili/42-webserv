
CFLAGS = 

NAME = webserv

HEADERS = main.hpp

FILES  = main.cpp

OBJS = $(FILES:.cpp=.o)

all:	$(NAME)

$(NAME): $(OBJS)
	c++ $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS): $(FILES)
	c++ $(CFLAGS) -c $(FILES)


exec: all
	./$(NAME)

clean: 
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
