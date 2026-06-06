#include "MoveGenerator.hpp"
#include "../engine/Rules.hpp"

bool MoveGenerator::hasAdjacentStone(const Board& board, int r, int c, int distance) {
    for (int dr = -distance; dr <= distance; ++dr) {
        for (int dc = -distance; dc <= distance; ++dc) {
            if (dr == 0 && dc == 0) continue; // Skip the cell itself
            int nr = r + dr;
            int nc = c + dc;
            // If we find ANY stone within the radius, this is an "interesting" cell

            if (board.inBounds(nr, nc) && board.getCell(nr, nc) != EMPTY) {
                return true;
            }
        }
    }
    return false;

}

std::vector<Point> MoveGenerator::generateMoves(const Board& board, Cell color) {
    std::vector<Point> moves;
    bool isEmpty = true;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board.getCell(r, c) != EMPTY) {
                isEmpty = false;
                continue;
            }
            // Only consider empty cells that are near existing stones
            if (hasAdjacentStone(board, r, c, 2) && Rules::isLegalMove(board, r, c, color)) {
                moves.push_back({r, c});
            }
        }
    }
        // Special Case: If the board is completely empty, the best first move is the exact center
        if (isEmpty) {
        moves.push_back({9, 9});
    }
    return moves;
}