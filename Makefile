NAME = gomoku

SRC =  main.cpp  gomoku.cpp

OBJ = $(SRC:.cpp=.o)
CXX = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++20 -O3
RM = rm -f


all: $(NAME)

$(NAME): $(OBJ) 
	 $(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)


clean:
	 $(RM) $(OBJ)

fclean: clean
	 $(RM) $(NAME)

re: fclean $(NAME)