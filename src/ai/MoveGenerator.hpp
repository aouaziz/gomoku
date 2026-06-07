#ifndef MOVEGENERATOR_HPP
#define MOVEGENERATOR_HPP

#include "../engine/Board.hpp"
#include "../core/Types.hpp"
#include "../engine/Rules.hpp"
#include "../engine/GameEngine.hpp"
#include <algorithm>
class MoveGenerator {
public:
    // Returns a list of sensible, legal moves to evaluate
    static std::vector<Point> generateMoves(GameEngine& engine, Cell color);

private:
    // Helper to check if an empty cell is near an existing stone
    static bool hasAdjacentStone(const Board& board, int r, int c, int distance);
    // Quick evaluation to sort moves
    static int scoreMove(GameEngine& engine, int r, int c, Cell color);

};

#endif // MOVEGENERATOR_HPP