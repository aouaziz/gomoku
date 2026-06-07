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
    uint64_t boardHash = engine.getBoard().getHash();
    int ttValue;

    // 1. CACHE LOOKUP: Did we already calculate this board state?
    if (tt.lookup(boardHash, depth, alpha, beta, ttValue)) {
        return ttValue; // INSTANT RETURN! Saved thousands of calculations!
    }

    if (depth == 0 || engine.isGameOver()) {
        int eval = Evaluator::evaluate(engine.getBoard(), aiColor);
        // Cache the exact evaluation
        tt.store(boardHash, depth, eval, HashFlag::EXACT); 
        return eval;   
    }

    Cell currentColor = isMaximizing ? aiColor : getOpponent(aiColor);
    std::vector<Point> candidates = MoveGenerator::generateMoves(engine.getBoard(), currentColor);
    
    int bestEval = isMaximizing ? -INF : INF;
    int originalAlpha = alpha;
    
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

    // 2. CACHE STORE: Save what we learned so we don't calculate it again
    HashFlag flag = HashFlag::EXACT;
    if (bestEval <= originalAlpha) flag = HashFlag::ALPHA;
    else if (bestEval >= beta)     flag = HashFlag::BETA;

    tt.store(boardHash, depth, bestEval, flag);

    return bestEval;
}

