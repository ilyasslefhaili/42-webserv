FLAGS = 

NAME = webserv

HEADERS = main.hpp

SRC  = main.cpp 	\
		src/Server.cpp 	\
		src/Request.cpp 	\
		Response/Response.cpp \
		Response/response_utils.cpp	\
		src/Config.cpp			\

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
