#ifndef AI_HPP
#define AI_HPP

#include "../engine/GameEngine.hpp"
#include "../core/Types.hpp"

class AI {
public:
    // Takes the engine state and returns the best Point to play
    Point getBestMove(GameEngine& engine, Cell aiColor);
};

#endif // AI_HPP