#include "AI.hpp"
#include "../engine/Rules.hpp"


// DUMB AI: Scan the board top-to-bottom, left-to-right to Return the very first legal move it finds.

Point AI::getBestMove(GameEngine& engine, Cell aiColor){

    const Board& board = engine.getBoard();
    for (int r = 0; r < BOARD_SIZE; r++)
    {
        for (int c = 0; c < BOARD_SIZE; c++)
        {
            if (Rules::isLegalMove(board,r,c,aiColor))
            {
                return Point{r, c};
            }
            
        }
        
    }
    return Point{BOARD_SIZE / 2, BOARD_SIZE / 2}; 
    
}