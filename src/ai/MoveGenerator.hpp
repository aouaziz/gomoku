#ifndef MOVEGENERATOR_HPP
#define MOVEGENERATOR_HPP

#include "../engine/Board.hpp"
#include "../core/Types.hpp"

class MoveGenerator {
public:
    // Returns a list of sensible, legal moves to evaluate
    static std::vector<Point> generateMoves(const Board& board, Cell color);

private:
    // Helper to check if an empty cell is near an existing stone
    static bool hasAdjacentStone(const Board& board, int r, int c, int distance);
};

#endif // MOVEGENERATOR_HPP