#include <vector>
#include <iostream>


const int BOARD_SIZE = 19;
enum Cell { EMPTY, BLACK, WHITE };

struct Point {
    int row , col; // row and column
};

struct MoveResult{
    Point position;
    Cell color;
    std::vector<Point> captured_stones; // stones captured by this move
    
};

class Gomoku{
    private:
    Cell board[BOARD_SIZE][BOARD_SIZE];
    Cell turn;
    int capturedBlack;
    int capturedWhite;
    std::vector<MoveResult> history;

    public:
        Gomoku();
        ~Gomoku();

        // ── core actions ──
        Cell getCell(int row, int col) const;
        void setCell(int row, int col, Cell cell);
        bool inBounds(int row, int col) const;
        Cell opponent(Cell color) const;


        // ── game logic (implement step by step) ──
        MoveResult applayMove(int row, int col, Cell color);
        void undoMove(const MoveResult &move);
        
        std::vector<Point> checkCaptures(int row, int col, Cell color) const;
        bool hasFive(Cell color) const;
        bool hasTenCaptures(Cell color) const;
        // bool            isDoubleThree(int r, int c, Cell color) const;  // later

        // ── game state ──
        Cell getCurrentTurn() const ;
        int getCaptured(Cell color) const;
        bool isGameOver() const;
        void printBoard() const;
};