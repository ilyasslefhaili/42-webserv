FLAGS = 

NAME = webserv

HEADERS = main.hpp

SRC  = main.cpp models/Server.cpp

OBJ = $(SRC:.cpp=.o)

all:	$(NAME)

$(NAME): $(SRC)
	c++ $(CFLAGS) $(SRC) -o $(NAME)

# $(OBJ): $(SRC)
# 	c++ $(CFLAGS) -c $(SRC)

# exec: all
# 	./$(NAME)

clean: 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
