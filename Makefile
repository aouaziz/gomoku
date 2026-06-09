NAME = Gomoku
TEST_NAME = run_tests

# --- Directory Modules ---
ENGINE_SRC = src/engine/Board.cpp \
             src/engine/Rules.cpp \
             src/engine/GameEngine.cpp

GAME_SRC   = src/game/GameSession.cpp

GUI_SRC    = src/gui/InputHandler.cpp \
             src/gui/Renderer.cpp \
             src/gui/GameWindow.cpp

AI_SRC     = src/ai/AI.cpp \
			 src/ai/MoveGenerator.cpp \
			 src/ai/Evaluator.cpp	\
			 src/engine/Zobrist.cpp \
			 src/ai/TranspositionTable.cpp


# --- Combined Sources ---
# MAKE SURE $(AI_SRC) IS AT THE END OF THIS LINE!
COMMON_SRC = $(ENGINE_SRC) $(GAME_SRC) $(GUI_SRC) $(AI_SRC)

# Main executable sources
SRC = $(COMMON_SRC) main.cpp
OBJ = $(SRC:.cpp=.o)

# Test executable sources
TEST_SRC = $(COMMON_SRC) test.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# --- Compiler Settings ---
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -O3 -MMD -MP -I/usr/local/include -I/opt/homebrew/include
LDFLAGS  = -L/usr/local/lib -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system
RM = rm -f

# Auto-generated header dependency files (so editing a .hpp triggers a rebuild
# of exactly the objects that need it — and nothing relinks otherwise).
DEPS = $(OBJ:.o=.d) $(TEST_OBJ:.o=.d)

# --- Rules ---
all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

test: $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(TEST_OBJ) -o $(TEST_NAME) $(LDFLAGS)
	./$(TEST_NAME)

clean:
	$(RM) $(OBJ) $(TEST_OBJ) $(DEPS)

fclean: clean
	$(RM) $(NAME) $(TEST_NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re test