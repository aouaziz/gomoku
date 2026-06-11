#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "../engine/Board.hpp"
#include "../core/Types.hpp"
#include "algorithm"

class Evaluator {
public:
    // Evaluates the board from the perspective of `aiColor`.
    // Positive score means aiColor is winning, negative means the opponent is.
    //
    // NOTE: in the search, `aiColor` is always the *side to move* at this node
    // (negamax evaluates each leaf from the mover's point of view). The dynamic
    // / tempo term below relies on that.
    static int evaluate(const Board& board, Cell aiColor);

private:
    // Scores a specific line on the board
    static int evaluateLine(int count, int openEnds, bool isAI);

    // Scans a direction to count stones and open ends
    static void countConsecutive(const Board& board, int r, int c, int dr, int dc, Cell color, int& count, int& openEnds);

    // Rubric: "potential captures" — net value of capturable pairs on the board
    // (positive when we can capture an opponent pair next, negative when one of
    // our pairs is one move away from being captured).
    static int potentialCaptureScore(const Board& board, Cell aiColor);
};

#endif 