#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "../game/GameSession.hpp"

class Renderer {
public:
    Renderer();
    void render(sf::RenderWindow& window, const GameSession& session);

private:
    sf::Font font;
    sf::Color boardColor;
    
    void drawGrid(sf::RenderWindow& window);
    void drawStones(sf::RenderWindow& window, const GameSession& session);
    void drawUI(sf::RenderWindow& window, const GameSession& session);
};

#endif // RENDERER_HPP