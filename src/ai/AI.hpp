#ifndef AI_HPP
#define AI_HPP

#include "../engine/GameEngine.hpp"
#include "../core/Types.hpp"
#include "TranspositionTable.hpp" 
#include "MoveGenerator.hpp"
#include "Evaluator.hpp"
#include <chrono>


class AI {
public:
    // Takes the engine state and returns the best Point to play
    Point getBestMove(GameEngine& engine, Cell aiColor);
private:
        int minimax(GameEngine& engine, int depth, int alpha, int beta, bool isMaximizing, Cell aiColor);
  
         TranspositionTable tt;

        // Time management variables
        std::chrono::time_point<std::chrono::steady_clock> startTime;
        bool timeIsUp() const;

        // We throw this to instantly abort the search when time runs out
        class TimeOutException : public std::exception {};
        long long getElapsedTime() const ;

    
};

#endif // AI_HPP