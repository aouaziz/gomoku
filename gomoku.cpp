#include "gomoku.hpp"

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
}

Cell Gomoku::getCell(int row, int col) const {
        return board[row][col];
}
void Gomoku::setCaptures(Cell cell)
{
    if(cell == BLACK)
        capturedBlack++;
    else if (cell == WHITE)
        capturedWhite++;
    
}
void Gomoku::setCell(int row, int col, Cell cell) {
    if (inBounds(row, col) && getCell(row,col) == EMPTY) {
        board[row][col] = cell;
        setCaptures(cell);
    }
    else
        std::cout << "Wrong Move \n";
}

bool Gomoku::inBounds(int row, int col) const {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

Cell Gomoku::opponent(Cell color) const {
    return (color == BLACK) ? WHITE : BLACK;
}

// MoveResult Gomoku::applayMove(int row, int col , Cell color)
// {
//     if (!inBounds(row, col) || getCell(row, col) != EMPTY) {
//     }
// }

void Gomoku::printBoard() const {
    std::cout << "   ";
    for (int c = 0; c < BOARD_SIZE; c++)
        std::cout << (c % 10)<< " ";
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


std::vector<Point> Gomoku::checkCaptures(int r, int c, Cell color) const {
    std::vector<Point> captured;
    Cell opp = opponent(color);

    // 8 directions (but we check each individually)
    const int dr[] = { -1, -1, -1,  0, 0,  1, 1, 1 };
    const int dc[] = { -1,  0,  1, -1, 1, -1, 0, 1 };

    for (int d = 0; d < 8; d++) {
        int r1 = r + dr[d],     c1 = c + dc[d];      // first neighbor
        int r2 = r + 2*dr[d],   c2 = c + 2*dc[d];    // second neighbor
        int r3 = r + 3*dr[d],   c3 = c + 3*dc[d];    // third cell (must be your color)

        if (inBounds(r1,c1) && inBounds(r2,c2) && inBounds(r3,c3)) {
            if (board[r1][c1] == opp &&
                board[r2][c2] == opp &&
                board[r3][c3] == color)
            {
                captured.push_back({r1, c1});
                captured.push_back({r2, c2});
            }
        }
    }
    return captured;
}