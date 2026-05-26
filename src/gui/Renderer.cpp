#include "Renderer.hpp"
#include "GuiConstants.hpp"
#include <iostream>

Renderer::Renderer() : boardColor(220, 179, 92) {
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cerr << "Warning: Font not found!" << std::endl;
    }
}

void Renderer::render(sf::RenderWindow& window, const GameSession& session) {
    window.clear(boardColor);
    drawGrid(window);
    drawStones(window, session);
    drawUI(window, session);
    window.display();
}

void Renderer::drawGrid(sf::RenderWindow& window) {
    // Lines
    for (int i = 0; i < BOARD_SIZE; i++) {
        sf::Vertex vLine[] = {
            { sf::Vector2f(gui::MARGIN + i * gui::CELL_SIZE, gui::MARGIN), sf::Color::Black },
            { sf::Vector2f(gui::MARGIN + i * gui::CELL_SIZE, gui::MARGIN + gui::BOARD_PIXELS), sf::Color::Black }
        };
        sf::Vertex hLine[] = {
            { sf::Vector2f(gui::MARGIN, gui::MARGIN + i * gui::CELL_SIZE), sf::Color::Black },
            { sf::Vector2f(gui::MARGIN + gui::BOARD_PIXELS, gui::MARGIN + i * gui::CELL_SIZE), sf::Color::Black }
        };
        window.draw(vLine, 2, sf::PrimitiveType::Lines);
        window.draw(hLine, 2, sf::PrimitiveType::Lines);
    }

    // Star points (Hoshi)
    const int hoshi[] = {3, 9, 15};
    for (int r : hoshi) {
        for (int c : hoshi) {
            sf::CircleShape dot(4);
            dot.setFillColor(sf::Color::Black);
            dot.setOrigin({4.f, 4.f});
            dot.setPosition({(float)gui::MARGIN + c * gui::CELL_SIZE, (float)gui::MARGIN + r * gui::CELL_SIZE});
            window.draw(dot);
        }
    }
}

void Renderer::drawStones(sf::RenderWindow& window, const GameSession& session) {
    const Board& board = session.getEngine().getBoard();

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Cell cell = board.getCell(r, c);
            if (cell == EMPTY) continue;

            sf::CircleShape stone((float)gui::STONE_RADIUS);
            stone.setOrigin({(float)gui::STONE_RADIUS, (float)gui::STONE_RADIUS});
            stone.setPosition({(float)gui::MARGIN + c * gui::CELL_SIZE, (float)gui::MARGIN + r * gui::CELL_SIZE});

            if (cell == BLACK) {
                stone.setFillColor(sf::Color::Black);
                stone.setOutlineColor(sf::Color(50, 50, 50));
            } else {
                stone.setFillColor(sf::Color::White);
                stone.setOutlineColor(sf::Color(180, 180, 180));
            }
            stone.setOutlineThickness(1.5f);
            window.draw(stone);
        }
    }
}

void Renderer::drawUI(sf::RenderWindow& window, const GameSession& session) {
    const Board& board = session.getEngine().getBoard();
    int blackCaps = board.getCaptures(BLACK);
    int whiteCaps = board.getCaptures(WHITE);
    bool gameOver = session.isGameOver();

    auto drawText = [&](const std::string& str, float x, float y, unsigned size, sf::Color color) {
        sf::Text t;
        t.setFont(font);
        t.setString(str);
        t.setCharacterSize(size);
        t.setFillColor(color);
        t.setPosition({x, y});
        window.draw(t);
    };

    float barY = (float)gui::WINDOW_WIDTH + 8;
    drawText("Black captures: " + std::to_string(blackCaps), gui::MARGIN, barY, 16, sf::Color::Black);
    drawText("White captures: " + std::to_string(whiteCaps), gui::MARGIN + 280, barY, 16, sf::Color::Black);
    drawText(session.getStatusMsg(), gui::MARGIN, barY + 26, 18, gameOver ? sf::Color(180, 0, 0) : sf::Color::Black);
}