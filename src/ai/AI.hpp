#ifndef AI_HPP
#define AI_HPP

#include "../engine/GameEngine.hpp"
#include "../core/Types.hpp"
#include "TranspositionTable.hpp" 


#include <limits>

class AI {
public:
    // Takes the engine state and returns the best Point to play
    Point getBestMove(GameEngine& engine, Cell aiColor);
private:
        int minimax(GameEngine& engine, int depth, int alpha, int beta, bool isMaximizing, Cell aiColor);
        const int SEARCH_DEPTH = 4; 
        TranspositionTable tt; 
    
};

#endif // AI_HPP