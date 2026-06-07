#include "MoveGenerator.hpp"

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

std::vector<Point> MoveGenerator::generateMoves(GameEngine& engine, Cell color) {
    const Board& board = engine.getBoard();
    std::vector<std::pair<int, Point>> scoredMoves;
    bool isEmpty = true;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board.getCell(r, c) != EMPTY) {
                isEmpty = false; continue;
            }

            if (hasAdjacentStone(board, r, c, 2) && Rules::isLegalMove(board, r, c, color)) {
                // Score the move!
                int score = scoreMove(engine, r, c, color);
                scoredMoves.push_back({score, {r, c}});
            }
        }
    }

    if (isEmpty) return {{9, 9}};

    // SORT: Highest score first (Captures and blocks go to the front!)
    std::sort(scoredMoves.begin(), scoredMoves.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    // Extract just the points
    std::vector<Point> sortedMoves;
    for (const auto& sm : scoredMoves) {
        sortedMoves.push_back(sm.second);
    }

    return sortedMoves;
}


int MoveGenerator::scoreMove(GameEngine& engine, int r, int c, Cell color) {
    int score = 0;
    try {
        // Simulate the move
        MoveResult mv = engine.applyMove(r, c, color);
        
        // Reward captures heavily! Alpha-Beta wants to see captures FIRST.
        score += mv.captured.size() * 1000;
        
        engine.undoMove();
    } catch (...) {
        return -10000; // Invalid move
    }
    return score;
}
