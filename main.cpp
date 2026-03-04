// main.cpp
#include "gomoku.hpp"

int main() {
    Gomoku game;

    game.setCell(9, 9, BLACK);
    game.setCell(9, 10,WHITE);
    game.setCell(9, 10,BLACK);
    game.printBoard();

    std::cout << "Cell (9,9) = " << game.getCell(9, 9) << std::endl;  // 1 (BLACK)
    std::cout << "inBounds(0,0) = " << game.inBounds(0, 0) << std::endl;    // 1
    std::cout << "inBounds(-1,0) = " << game.inBounds(-1, 0) << std::endl;  // 0

    return 0;
}