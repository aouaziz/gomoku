#include "gomoku.hpp"

int main() {
    Gomoku game;

    game.setCell(5, 3, WHITE);
    game.setCell(5, 4, WHITE);
    game.setCell(5, 5, BLACK);

    std::cout << "=== Before move ===" << std::endl;
    game.printBoard();

    MoveResult mr = game.applyMove(5, 2, BLACK);
    std::cout << "=== After move (should capture 2 whites) ===" << std::endl;
    game.printBoard();

    game.undoMove(mr);
    std::cout << "=== After undo (whites restored) ===" << std::endl;
    game.printBoard();

    return 0;
}