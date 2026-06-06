#ifndef GAMESESSION_HPP
#define GAMESESSION_HPP

#include "../engine/GameEngine.hpp"
#include <string>

class GameSession {
public:
    GameSession(bool playVsAI = true, Cell aiPlayerColor = WHITE);
    
    //AI Turn handlers

    bool isAITurn() const;
    void handleAITurn();
    
    
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
    // AI State
    bool vsAI;
    Cell aiColor;
    
    // Internal helpers
    void switchTurn();
    void updateStatusAfterMove();
};

#endif // GAMESESSION_HPP