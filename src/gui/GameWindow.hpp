#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <SFML/Graphics.hpp>
#include "Renderer.hpp"
#include "InputHandler.hpp"
#include "../game/GameSession.hpp"

class GameWindow {
public:
    GameWindow();
    void run();

private:
    sf::RenderWindow window;
    Renderer renderer;
    InputHandler inputHandler;
    GameSession session;

    void processEvents();
};

#endif // GAMEWINDOW_HPP