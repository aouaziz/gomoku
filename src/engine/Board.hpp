#ifndef BOARD_HPP
#define BOARD_HPP

#include "../core/Types.hpp"
#include <iostream>

class Board {
public:
    Board();

    // Accessors
    Cell getCell(int r, int c) const;
    void setCell(int r, int c, Cell cell);
    bool inBounds(int r, int c) const;

    // Captures
    int getCaptures(Cell color) const;
    void addCaptures(Cell color, int amount);
    void removeCaptures(Cell color, int amount);

    // Debugging
    void print() const;

private:
    Cell board[BOARD_SIZE][BOARD_SIZE];
    int capturedBlack;
    int capturedWhite;
};

#endif // BOARD_HPP