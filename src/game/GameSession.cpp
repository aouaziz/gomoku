#include "GameSession.hpp"
#include "../engine/Rules.hpp"
#include "../ai/AI.hpp" 

GameSession::GameSession(bool playVsAI , Cell aiPlayerColor ) : currentTurn(BLACK), statusMsg("Black's turn"), vsAI(playVsAI), aiColor(aiPlayerColor) {} 

const GameEngine& GameSession::getEngine() const { return engine; }
Cell GameSession::getCurrentTurn() const { return currentTurn; }
std::string GameSession::getStatusMsg() const { return statusMsg; }
bool GameSession::isGameOver() const { return engine.isGameOver(); }

void GameSession::handlePlayerMove(int r, int c) {
    if (engine.isGameOver()) return;

    try {
        engine.applyMove(r, c, currentTurn);
        updateStatusAfterMove();
    } catch (const std::invalid_argument& e) {
        // If the move is illegal, catch the engine's error and display it
        statusMsg = std::string("Invalid move: ") + e.what();
    }
}

void GameSession::updateStatusAfterMove() {
    // 1. Did that move end the game?
    if (engine.isGameOver()) {
        statusMsg = engine.getWinReason();
        return;
    }

    // 2. Did that move create a *breakable* 5-in-a-row?
    // (If it was unbreakable, engine.isGameOver() would be true above)
    if (Rules::hasFive(engine.getBoard(), currentTurn)) {
        std::string oppName = (currentTurn == BLACK) ? "White" : "Black";
        statusMsg = "5-in-a-row formed! " + oppName + " must break it or lose.";
        switchTurn();
        return;
    } 

    // 3. Otherwise, normal turn switch
    switchTurn();
    statusMsg = (currentTurn == BLACK) ? "Black's turn" : "White's turn";
}

void GameSession::switchTurn() {
    currentTurn = getOpponent(currentTurn);
}

bool GameSession::isAITurn() const {
    // It's the AI's turn if vsAI is true, the game isn't over, and the current turn matches the AI's color
    return vsAI && !isGameOver() && (currentTurn == aiColor);
}

void GameSession::handleAITurn() {
    if (!isAITurn()) return;

    AI ai;
    Point bestMove = ai.getBestMove(engine, aiColor);
    
    // We can re-use the player move logic because it safely applies the move and switches turns!
    handlePlayerMove(bestMove.row, bestMove.col);
}