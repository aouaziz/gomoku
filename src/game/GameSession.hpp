#ifndef GAMESESSION_HPP
#define GAMESESSION_HPP

#include "../engine/GameEngine.hpp"
#include <string>

class GameSession {
public:
    GameSession();

    // Core Interaction
    void handlePlayerMove(int r, int c);
    
    // Accessors for the GUI
    const GameEngine& getEngine() const;
    Cell getCurrentTurn() const;
    std::string getStatusMsg() const;
    bool isGameOver() const;

private:
    GameEngine engine;
    Cell currentTurn;
    std::string statusMsg;

    // Internal helpers
    void switchTurn();
    void updateStatusAfterMove();
};

#endif // GAMESESSION_HPP