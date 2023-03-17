CFLAGS = -Wall -Werror -Wextra
NAME = webserv

HEADERS = main.hpp src/ClientInfo.hpp src/Config.hpp src/Location.hpp src/Request.hpp src/Server.hpp src/ServerConfig.hpp \
			Response/Response.hpp Response/Mimetypes.hpp

SRC  = main.cpp 	\
		src/Server.cpp 	\
		src/Request.cpp 	\
		Response/Response.cpp \
		Response/response_utils.cpp	\
		src/Config.cpp			\
		Response/Mimetypes.cpp	\
		main_utils.cpp			\

OBJ = $(SRC:.cpp=.o)

all:	$(NAME)

$(NAME): $(SRC) $(HEADERS)
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
