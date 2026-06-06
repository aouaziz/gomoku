#include "GameWindow.hpp"
#include "GuiConstants.hpp"

GameWindow::GameWindow() 
    : window(sf::VideoMode({(unsigned int)gui::WINDOW_WIDTH, (unsigned int)gui::WINDOW_HEIGHT}), "Gomoku Engine") 
{
    window.setFramerateLimit(60);
}

void GameWindow::run() {
    while (window.isOpen()) {
        processEvents();
        // NEW: If it's the AI's turn, let it make a move!
        if (session.isAITurn()) {
            session.handleAITurn();
        }
        renderer.render(window, session);
    }
}

// Ensure processEvents() ignores mouse clicks during the AI's turn:
void GameWindow::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (const auto* mousePtr = event->getIf<sf::Event::MouseButtonPressed>()) {
            // NEW: Ignore mouse clicks if it's the AI's turn!
            if (mousePtr->button == sf::Mouse::Button::Left && !session.isGameOver() && !session.isAITurn()) {
                auto clickedPoint = inputHandler.getBoardCoordinates((float)mousePtr->position.x, (float)mousePtr->position.y);
                if (clickedPoint) {
                    session.handlePlayerMove(clickedPoint->row, clickedPoint->col);
                }
            }
        }
    }
}
