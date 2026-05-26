#include "Board.hpp"

Board::Board() : capturedBlack(0), capturedWhite(0) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
}

Cell Board::getCell(int r, int c) const {
    if (!inBounds(r, c)) return EMPTY;
    return board[r][c];
}

void Board::setCell(int r, int c, Cell cell) {
    if (inBounds(r, c)) {
        board[r][c] = cell;
    }
}

bool Board::inBounds(int r, int c) const {
    return r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE;
}

int Board::getCaptures(Cell color) const {
    return (color == BLACK) ? capturedBlack : capturedWhite;
}

void Board::addCaptures(Cell color, int amount) {
    if (color == BLACK) capturedBlack += amount;
    else if (color == WHITE) capturedWhite += amount;
}

void Board::removeCaptures(Cell color, int amount) {
    if (color == BLACK) capturedBlack -= amount;
    else if (color == WHITE) capturedWhite -= amount;
}

void Board::print() const {
    std::cout << "   ";
    for (int c = 0; c < BOARD_SIZE; c++) std::cout << (c % 10) << " ";
    std::cout << "\n";

    for (int r = 0; r < BOARD_SIZE; r++) {
        std::cout << (r < 10 ? " " : "") << r << " ";
        for (int c = 0; c < BOARD_SIZE; c++) {
            char ch = '.';
            if (board[r][c] == BLACK) ch = 'X';
            if (board[r][c] == WHITE) ch = 'O';
            std::cout << ch << " ";
        }
        std::cout << "\n";
    }
    std::cout << "Captures  B:" << capturedBlack << "  W:" << capturedWhite << "\n";
}