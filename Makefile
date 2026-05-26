NAME = gomoku
TEST_NAME = run_tests

# --- Directory Modules ---
ENGINE_SRC = src/engine/Board.cpp \
             src/engine/Rules.cpp \
             src/engine/GameEngine.cpp

GAME_SRC   = src/game/GameSession.cpp

GUI_SRC    = src/gui/InputHandler.cpp \
             src/gui/Renderer.cpp \
             src/gui/GameWindow.cpp

# --- Combined Sources ---
COMMON_SRC = $(ENGINE_SRC) $(GAME_SRC) $(GUI_SRC)

# Main executable sources
SRC = $(COMMON_SRC) main.cpp
OBJ = $(SRC:.cpp=.o)

# Test executable sources
TEST_SRC = $(COMMON_SRC) test.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# --- Compiler Settings ---
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -O3
LDFLAGS  = -lsfml-graphics -lsfml-window -lsfml-system
RM = rm -f

# --- Rules ---
all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

test: $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(TEST_OBJ) -o $(TEST_NAME) $(LDFLAGS)
	./$(TEST_NAME)

clean:
	$(RM) $(OBJ) $(TEST_OBJ)

fclean: clean
	$(RM) $(NAME) $(TEST_NAME)

re: fclean all

.PHONY: all clean fclean re test