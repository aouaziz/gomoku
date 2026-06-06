#include "AI.hpp"
#include "MoveGenerator.hpp"
#include "Evaluator.hpp"


const int INF = std::numeric_limits<int>::max();

Point AI::getBestMove(GameEngine& engine, Cell aiColor){

    const Board& board = engine.getBoard();
    
    std::vector<Point> candidates = MoveGenerator::generateMoves(board, aiColor);
    if (candidates.empty()) {
        return Point{BOARD_SIZE / 2, BOARD_SIZE / 2};
    }
    
    int bestScore = -INF;
    Point bestMove = candidates[0]; //Default to first candidate
    std::cout << "AI is thinking..." << std::endl;

    for (const Point& move : candidates){
        try{
            engine.applyMove(move.row, move.col, aiColor);
            int score = minimax(engine, SEARCH_DEPTH - 1, -INF, INF, false, aiColor);
            engine.undoMove();
            if (score > bestScore){
                bestScore = score;
                bestMove = move;
            }
        }
        catch(const std::invalid_argument& e){
            continue; // Skip this move and try the next one
        }
    }
    std::cout << "AI chose move (" << bestMove.row << ", " << bestMove.col 
              << ") with score " << bestScore << std::endl;

    return bestMove;
    
}

int AI::minimax(GameEngine& engine, int depth, int alpha, int beta, bool isMaximizing, Cell aiColor) {
    if (depth == 0 || engine.isGameOver()) {
        return Evaluator::evaluate(engine.getBoard(), aiColor);
    }

    Cell currentColor = isMaximizing ? aiColor : getOpponent(aiColor);
    std::vector<Point> candidates = MoveGenerator::generateMoves(engine.getBoard(), currentColor);
    
    int bestEval = isMaximizing ? -INF : INF;

    for (const Point& move : candidates) {
        try {
            engine.applyMove(move.row, move.col, currentColor);
            
            // Notice the !isMaximizing here! We MUST switch turns.
            int eval = minimax(engine, depth - 1, alpha, beta, !isMaximizing, aiColor);
            
            engine.undoMove();

            if (isMaximizing) {
                bestEval = std::max(bestEval, eval);
                alpha = std::max(alpha, eval);
            } else {
                bestEval = std::min(bestEval, eval);
                beta = std::min(beta, eval);
            }

            // Alpha-Beta Pruning
            if (beta <= alpha) break; 
            
        } catch (const std::invalid_argument&) {
            continue; // Skip forbidden moves
        }
    }
    return bestEval;
}

