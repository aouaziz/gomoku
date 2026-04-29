#include "gomoku.hpp"

int main() {
    Gomoku game;
    Cell currentTurn = BLACK; // Move outside the loop
    MoveResult mv;

    while(true) {
        game.printBoard();
        
        if(currentTurn == WHITE)
            std::cout << "White's turn (O).\n";
        else
            std::cout << "Black's turn (X).\n";
       
        int row, col;
        std::cout << "Enter row: ";
        if (!(std::cin >> row)) break; // Exit if input is invalid (e.g. user typed a letter)
        
        std::cout << "Enter col: ";
        if (!(std::cin >> col)) break;

        try {
            // Try to apply the move. If it's invalid, it jumps straight to 'catch'
            mv = game.applyMove(row, col, currentTurn);
            
            // --- THE REFEREE: Check if the game is over ---
            
            // 1. Check for 10 captures
            if (game.hasTenCaptures(currentTurn)) {
                game.printBoard();
                std::cout << "\n🎉 WINNER! " << (currentTurn == BLACK ? "Black" : "White") 
                          << " wins by capturing 10 stones!\n";
                break; // Ends the game loop
            }

            // 2. Check for 5-in-a-row
            if (game.hasFiveAt(row, col, currentTurn)) {
                game.printBoard();
                std::cout << "\n🎉 WINNER! " << (currentTurn == BLACK ? "Black" : "White") 
                          << " wins with 5-in-a-row!\n";
                break; // Ends the game loop
            }

            // If the move was successful and no one won, switch turns!
            currentTurn = game.opponent(currentTurn);

        }
        catch(const std::invalid_argument& e) {
            // This runs if applyMove threw an error!
            std::cout << "\n⚠️ Invalid Move: " << e.what() << "\n";
            std::cout << "Please try a different spot.\n\n";
            // Notice we do NOT switch the turn here. The loop will restart
            // and ask the exact same player to go again.
        }
    }
    
    std::cout << "Game Over. Thanks for playing!\n";
    return 0;
}