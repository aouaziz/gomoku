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
        renderer.render(window, session);
    }
}

void GameWindow::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left && !session.isGameOver()) {
                // Delegate mouse conversion to InputHandler
                auto clickedPoint = inputHandler.getBoardCoordinates((float)event.mouseButton.x, (float)event.mouseButton.y);

                // If a valid spot on the board was clicked, send it to the session
                if (clickedPoint) {
                    session.handlePlayerMove(clickedPoint->row, clickedPoint->col);
                }
            }
        }
    }
}