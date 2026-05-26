#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include "Board.hpp"
#include "Rules.hpp"
#include <vector>
#include <stdexcept>
#include <string>

class GameEngine {
public:
    GameEngine();

    // Core Actions
    MoveResult applyMove(int r, int c, Cell color);
    void undoMove(); // critical for AI

    // State accessors
    const Board& getBoard() const;
    bool isGameOver() const;
    Cell getWinner() const;
    std::string getWinReason() const;

private:
    Board board;
    std::vector<MoveResult> history;
    
    bool gameOver;
    Cell winner;
    std::string winReason;

    // Internal helper to evaluate game state after a move
    void checkWinConditions(Cell lastColor);
};

#endif // GAMEENGINE_HPP