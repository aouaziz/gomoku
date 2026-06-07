#include "AI.hpp"
#include "MoveGenerator.hpp"
#include "Evaluator.hpp"
#include <iostream>
#include <algorithm>

const int INF = std::numeric_limits<int>::max();
const int MAX_TIME_MS = 450; // 0.45 seconds limit

long long AI::getElapsedTime() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
}
bool AI::timeIsUp() const {
    return getElapsedTime() >= MAX_TIME_MS;
}

Point AI::getBestMove(GameEngine& engine, Cell aiColor) {
    startTime = std::chrono::steady_clock::now();
    
    // We generate moves from the ENGINE now
    std::vector<Point> candidates = MoveGenerator::generateMoves(engine, aiColor);
    if (candidates.empty()) return Point{BOARD_SIZE / 2, BOARD_SIZE / 2};

    Point bestMoveOverall = candidates[0];
    int currentDepth = 1;

    std::cout << "AI is thinking..." << std::endl;

    // ITERATIVE DEEPENING LOOP
    try {
        while (currentDepth <= 10) { // Max safeguard depth
            Point bestMoveForThisDepth = candidates[0];
            int bestScore = -INF;

            for (const Point& move : candidates) {
                if (timeIsUp()) throw TimeOutException(); // Abort!

                // 1. Try to apply the move
                try {
                    engine.applyMove(move.row, move.col, aiColor);
                } catch (const std::invalid_argument&) {
                    continue; // Skip invalid moves
                }

                // 2. Safely run minimax and guarantee cleanup
                try {
                    int score = minimax(engine, currentDepth - 1, -INF, INF, false, aiColor);
                    engine.undoMove(); // Clean up normally

                    if (score > bestScore) {
                        bestScore = score;
                        bestMoveForThisDepth = move;
                    }
                } catch (const TimeOutException& e) {
                    engine.undoMove(); // EMERGENCY CLEANUP before panicking!
                    throw e;           // Re-throw to break out of Iterative Deepening
                }
            }


            // If we completed this depth without timing out, save the result!
            bestMoveOverall = bestMoveForThisDepth;
            std::cout << "[Time: " << getElapsedTime() << " ms] Depth " 
                    << currentDepth << " completed. Best move: (" << bestMoveOverall.row << "," 
                            << bestMoveOverall.col << ") Score: " << bestScore << std::endl;
            
            // If we found a guaranteed win, stop searching!
            if (bestScore > 90000) break;

            currentDepth++;
        }
    } 
    catch (const TimeOutException&) {
            std::cout << "[Time: " << getElapsedTime() << " ms] Time limit reached! Aborted depth " 
              << currentDepth << "." << std::endl;    }

    return bestMoveOverall;
}

int AI::minimax(GameEngine& engine, int depth, int alpha, int beta, bool isMaximizing, Cell aiColor) {
    if (timeIsUp()) throw TimeOutException(); // Abort deep recursion!

    uint64_t boardHash = engine.getBoard().getHash();
    int ttValue;

    if (tt.lookup(boardHash, depth, alpha, beta, ttValue)) {
        return ttValue;
    }

    if (depth == 0 || engine.isGameOver()) {
        int eval = Evaluator::evaluate(engine.getBoard(), aiColor);
        tt.store(boardHash, depth, eval, HashFlag::EXACT);
        return eval;
    }

    Cell currentColor = isMaximizing ? aiColor : getOpponent(aiColor);
    // Pass engine to generator
    std::vector<Point> candidates = MoveGenerator::generateMoves(engine, currentColor);
    
    int bestEval = isMaximizing ? -INF : INF;
    int originalAlpha = alpha;

for (const Point& move : candidates) {
        // 1. Try to apply the move
        try {
            engine.applyMove(move.row, move.col, currentColor);
        } catch (const std::invalid_argument&) {
            continue; // Skip invalid moves
        }

        // 2. Safely recurse and guarantee cleanup
        try {
            int eval = minimax(engine, depth - 1, alpha, beta, !isMaximizing, aiColor);
            engine.undoMove(); // Clean up normally

            if (isMaximizing) {
                bestEval = std::max(bestEval, eval);
                alpha = std::max(alpha, eval);
            } else {
                bestEval = std::min(bestEval, eval);
                beta = std::min(beta, eval);
            }

            if (beta <= alpha) break; // Alpha-Beta Pruning
            
        } catch (const TimeOutException& e) {
            engine.undoMove(); // EMERGENCY CLEANUP before panicking!
            throw e;           // Re-throw to bubble up the timeout
        }
    }

    HashFlag flag = HashFlag::EXACT;
    if (bestEval <= originalAlpha) flag = HashFlag::ALPHA;
    else if (bestEval >= beta)     flag = HashFlag::BETA;

    tt.store(boardHash, depth, bestEval, flag);

    return bestEval;
}