#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "../game/GameSession.hpp"

class Renderer {
public:
    Renderer();

    // --- Screen renderers ---
    void render(sf::RenderWindow& window, const GameSession& session, bool showHelp);
    void renderTitle(sf::RenderWindow& window);
    void renderModeSelect(sf::RenderWindow& window);
    void renderAISetup(sf::RenderWindow& window, const std::string& difficulty, Cell humanColor);

    // ===== BONUS (Dark / Light theme) =====
    void toggleTheme(); // switch between light and dark board themes

    // --- Clickable button rectangles (single source of truth, also hit-tested
    //     by GameWindow). Positions are in window pixels. ---
    static sf::FloatRect btnTitleStart();
    static sf::FloatRect btnModeAI();
    static sf::FloatRect btnModeHotseat();
    static sf::FloatRect btnModeBack();
    static sf::FloatRect btnDiffEasy();
    static sf::FloatRect btnDiffMedium();
    static sf::FloatRect btnDiffHard();
    static sf::FloatRect btnColorBlack();
    static sf::FloatRect btnColorWhite();
    static sf::FloatRect btnAIStart();
    static sf::FloatRect btnAIBack();
    static sf::FloatRect btnHelp();      // in-game "controls" button

private:
    sf::Font font;

    // ===== BONUS (Dark / Light theme): palette swapped by toggleTheme =====
    bool darkMode = false;
    sf::Color boardColor;   // board background
    sf::Color lineColor;    // grid lines + hoshi
    sf::Color textColor;    // UI text
    sf::Color blackOutline; // outline for black stones (visibility on dark bg)
    void applyTheme();

    // --- Text + widget helpers ---
    void drawText(sf::RenderWindow& window, const std::string& str,
                  float x, float y, unsigned size, sf::Color color);
    void drawCenteredText(sf::RenderWindow& window, const std::string& str,
                          float centerX, float y, unsigned size, sf::Color color);
    bool isHover(sf::RenderWindow& window, const sf::FloatRect& rect) const;
    void drawButton(sf::RenderWindow& window, const sf::FloatRect& rect,
                    const std::string& label, bool hovered, bool selected);
    void drawMenuBackground(sf::RenderWindow& window);

    // --- In-game pieces ---
    void drawGrid(sf::RenderWindow& window);
    void drawStones(sf::RenderWindow& window, const GameSession& session);
    void drawMarkers(sf::RenderWindow& window, const GameSession& session);
    void drawTurnIndicator(sf::RenderWindow& window, const GameSession& session);
    void drawUI(sf::RenderWindow& window, const GameSession& session);
    void drawHelpButton(sf::RenderWindow& window);
    void drawHelpOverlay(sf::RenderWindow& window);
};

#endif // RENDERER_HPP
