#include "gomoki.hpp"

Gomoku::Gomoku() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
    turn = BLACK;
    capturedBlack = 0;
    capturedWhite = 0;
    std::cout << "Gomoku game created" << std::endl;
}

Gomoku::~Gomoku() {
    // nothing to do
}

Cell Gomoku::getCell(int row, int col) const {
        return board[row][col];
}

void Gomoku::setCell(int row, int col, Cell cell) {
    if (inBounds(row, col)) {
        board[row][col] = cell;
    }
}

bool Gomoku::inBounds(int row, int col) const {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

Cell Gomoku::opponent(Cell color) const {
    return (color == BLACK) ? WHITE : BLACK;
}

MoveResult Gomoku::applayMove(int row, int col , Cell color)
{
    if (!inBounds(row, col) || getCell(row, col) != EMPTY) {
    }
}