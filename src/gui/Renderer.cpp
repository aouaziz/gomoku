#include "Renderer.hpp"
#include "GuiConstants.hpp"
#include <iostream>

Renderer::Renderer() {
    applyTheme(); // initialise the light palette

    // Try a bundled font first (keeps the repo self-contained), then fall back to
    // common system fonts so the program runs on both Linux and macOS.
    const char* candidates[] = {
        "assets/font.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/Library/Fonts/Arial.ttf",
        "/System/Library/Fonts/Geneva.ttf",
    };
    bool loaded = false;
    for (const char* path : candidates) {
        if (font.openFromFile(path)) { loaded = true; break; }
    }
    if (!loaded) {
        std::cerr << "Warning: no usable font found! Text will not render." << std::endl;
    }
}

// ===== BONUS (Dark / Light theme) =====
// Light = classic wood board with black ink; Dark = slate board with light ink.
void Renderer::applyTheme() {
    if (darkMode) {
        boardColor   = sf::Color(45, 48, 56);
        lineColor    = sf::Color(150, 150, 160);
        textColor    = sf::Color(230, 230, 235);
        blackOutline = sf::Color(120, 120, 130);
    } else {
        boardColor   = sf::Color(220, 179, 92);
        lineColor    = sf::Color::Black;
        textColor    = sf::Color::Black;
        blackOutline = sf::Color(50, 50, 50);
    }
}

void Renderer::toggleTheme() {
    darkMode = !darkMode;
    applyTheme();
}

void Renderer::drawText(sf::RenderWindow& window, const std::string& str,
                        float x, float y, unsigned size, sf::Color color) {
    sf::Text t(font, str, size);
    t.setFillColor(color);
    t.setPosition({x, y});
    window.draw(t);
}

// ---------------------------------------------------------------------------
//  Shared menu look & feel
// ---------------------------------------------------------------------------
namespace {
    const sf::Color MENU_BG   (18, 20, 28);
    const sf::Color PANEL_BG  (32, 36, 48);
    const sf::Color ACCENT    (0, 200, 170);   // teal highlight
    const sf::Color ACCENT_DK (15, 18, 24);    // text on top of accent fills
    const sf::Color INK       (224, 228, 234); // light text
    const sf::Color INK_DIM   (150, 156, 168);
    const float CENTER_X = gui::WINDOW_WIDTH / 2.0f;
}

void Renderer::drawCenteredText(sf::RenderWindow& window, const std::string& str,
                                float centerX, float y, unsigned size, sf::Color color) {
    sf::Text t(font, str, size);
    t.setFillColor(color);
    sf::FloatRect b = t.getLocalBounds();
    t.setPosition({centerX - (b.size.x / 2.0f) - b.position.x, y});
    window.draw(t);
}

bool Renderer::isHover(sf::RenderWindow& window, const sf::FloatRect& rect) const {
    sf::Vector2f m = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    return rect.contains(m);
}

void Renderer::drawButton(sf::RenderWindow& window, const sf::FloatRect& rect,
                          const std::string& label, bool hovered, bool selected) {
    bool hot = hovered || selected;
    sf::RectangleShape box(rect.size);
    box.setPosition(rect.position);
    box.setFillColor(hot ? ACCENT : PANEL_BG);
    box.setOutlineColor(ACCENT);
    box.setOutlineThickness(hovered ? 3.0f : 2.0f);
    window.draw(box);

    sf::Text t(font, label, 22);
    t.setFillColor(hot ? ACCENT_DK : INK);
    sf::FloatRect b = t.getLocalBounds();
    t.setPosition({rect.position.x + (rect.size.x - b.size.x) / 2.0f - b.position.x,
                   rect.position.y + (rect.size.y - b.size.y) / 2.0f - b.position.y});
    window.draw(t);
}

void Renderer::drawMenuBackground(sf::RenderWindow& window) {
    window.clear(MENU_BG);
    // A faint accent bar under the title area for a bit of "UI" flavour.
    sf::RectangleShape bar({(float)gui::WINDOW_WIDTH, 4.0f});
    bar.setPosition({0.0f, 150.0f});
    bar.setFillColor(sf::Color(ACCENT.r, ACCENT.g, ACCENT.b, 120));
    window.draw(bar);
}

// ---------------------------------------------------------------------------
//  Button layout (single source of truth)
// ---------------------------------------------------------------------------
sf::FloatRect Renderer::btnTitleStart()  { return {{210.f, 520.f}, {380.f, 74.f}}; }
sf::FloatRect Renderer::btnModeAI()       { return {{210.f, 300.f}, {380.f, 76.f}}; }
sf::FloatRect Renderer::btnModeHotseat()  { return {{210.f, 400.f}, {380.f, 76.f}}; }
sf::FloatRect Renderer::btnModeBack()     { return {{210.f, 540.f}, {380.f, 50.f}}; }
sf::FloatRect Renderer::btnDiffEasy()     { return {{120.f, 300.f}, {170.f, 64.f}}; }
sf::FloatRect Renderer::btnDiffMedium()   { return {{315.f, 300.f}, {170.f, 64.f}}; }
sf::FloatRect Renderer::btnDiffHard()     { return {{510.f, 300.f}, {170.f, 64.f}}; }
sf::FloatRect Renderer::btnColorBlack()   { return {{175.f, 430.f}, {210.f, 64.f}}; }
sf::FloatRect Renderer::btnColorWhite()   { return {{415.f, 430.f}, {210.f, 64.f}}; }
sf::FloatRect Renderer::btnAIStart()      { return {{210.f, 560.f}, {380.f, 74.f}}; }
sf::FloatRect Renderer::btnAIBack()       { return {{300.f, 656.f}, {200.f, 46.f}}; }
sf::FloatRect Renderer::btnHelp()         { return {{10.f, 8.f}, {40.f, 32.f}}; }
sf::FloatRect Renderer::btnGameOverMenu() { return {{(float)gui::WINDOW_WIDTH / 2.f - 150.f, 540.f}, {300.f, 64.f}}; }

// ---------------------------------------------------------------------------
//  Screen: Title
// ---------------------------------------------------------------------------
void Renderer::renderTitle(sf::RenderWindow& window) {
    drawMenuBackground(window);
    // Title with a drop shadow for a bit of depth.
    drawCenteredText(window, "GOMOKU", CENTER_X + 3, 73, 84, sf::Color(0, 0, 0, 160));
    drawCenteredText(window, "GOMOKU", CENTER_X, 70, 84, ACCENT);
    drawCenteredText(window, "Pente variant  -  Min-Max AI", CENTER_X, 175, 22, INK_DIM);

    drawButton(window, btnTitleStart(), "START GAME", isHover(window, btnTitleStart()), false);
    drawCenteredText(window, "Click START or press Enter", CENTER_X, 620, 18, INK_DIM);
    drawCenteredText(window, "[T] toggle Light / Dark theme", CENTER_X, 650, 16, INK_DIM);
    window.display();
}

// ---------------------------------------------------------------------------
//  Screen: Mode selection
// ---------------------------------------------------------------------------
void Renderer::renderModeSelect(sf::RenderWindow& window) {
    drawMenuBackground(window);
    drawCenteredText(window, "SELECT MODE", CENTER_X, 80, 48, ACCENT);

    drawButton(window, btnModeAI(),      "Play vs AI",          isHover(window, btnModeAI()), false);
    drawButton(window, btnModeHotseat(), "Hotseat (2 Players)", isHover(window, btnModeHotseat()), false);
    drawButton(window, btnModeBack(),    "< Back",              isHover(window, btnModeBack()), false);
    window.display();
}

// ---------------------------------------------------------------------------
//  Screen: AI setup (difficulty + your color)  [BONUS: AI difficulty selector]
// ---------------------------------------------------------------------------
void Renderer::renderAISetup(sf::RenderWindow& window, const std::string& difficulty, Cell humanColor) {
    drawMenuBackground(window);
    drawCenteredText(window, "AI SETUP", CENTER_X, 80, 48, ACCENT);

    drawCenteredText(window, "Difficulty", CENTER_X, 255, 24, INK);
    drawButton(window, btnDiffEasy(),   "Easy",   isHover(window, btnDiffEasy()),   difficulty == "Easy");
    drawButton(window, btnDiffMedium(), "Medium", isHover(window, btnDiffMedium()), difficulty == "Medium");
    drawButton(window, btnDiffHard(),   "Hard",   isHover(window, btnDiffHard()),   difficulty == "Hard");

    drawCenteredText(window, "You play as", CENTER_X, 388, 24, INK);
    drawButton(window, btnColorBlack(), "Black (1st)", isHover(window, btnColorBlack()), humanColor == BLACK);
    drawButton(window, btnColorWhite(), "White (2nd)", isHover(window, btnColorWhite()), humanColor == WHITE);

    drawButton(window, btnAIStart(), "START GAME", isHover(window, btnAIStart()), false);
    drawButton(window, btnAIBack(),  "< Back",     isHover(window, btnAIBack()), false);
    window.display();
}

// ---------------------------------------------------------------------------
//  Screen: in-game
// ---------------------------------------------------------------------------
void Renderer::render(sf::RenderWindow& window, const GameSession& session, bool showHelp) {
    window.clear(boardColor);
    drawGrid(window);
    drawStones(window, session);
    drawMarkers(window, session);
    drawTurnIndicator(window, session);
    drawUI(window, session);
    drawHelpButton(window);
    if (showHelp) drawHelpOverlay(window);
    if (session.isGameOver()) drawGameOverOverlay(window, session);
    window.display();
}

// Big "GAME OVER" banner + the win reason + a Back-to-Menu button.
void Renderer::drawGameOverOverlay(sf::RenderWindow& window, const GameSession& session) {
    sf::RectangleShape dim({(float)gui::WINDOW_WIDTH, (float)gui::WINDOW_HEIGHT});
    dim.setFillColor(sf::Color(0, 0, 0, 190));
    window.draw(dim);

    float cx = gui::WINDOW_WIDTH / 2.0f;
    drawCenteredText(window, "GAME OVER", cx + 4, 304, 92, sf::Color(0, 0, 0, 180)); // shadow
    drawCenteredText(window, "GAME OVER", cx, 300, 92, sf::Color(230, 70, 70));

    // The result text (e.g. "Black wins with 5-in-a-row!") comes from the session.
    drawCenteredText(window, session.getStatusMsg(), cx, 430, 26, INK);

    drawButton(window, btnGameOverMenu(), "Back to Menu", isHover(window, btnGameOverMenu()), false);
}

// Shows whose turn it is: a large colored stone + label, centered at the top.
void Renderer::drawTurnIndicator(sf::RenderWindow& window, const GameSession& session) {
    Cell turn = session.getCurrentTurn();

    std::string who = (turn == BLACK) ? "Black" : "White";
    std::string label;
    if (session.isGameOver()) {
        label = "Game over";
    } else if (session.isVsAI()) {
        label = (turn == session.getAIColor()) ? ("AI thinking... (" + who + ")")
                                                : ("Your turn  (" + who + ")");
    } else {
        label = who + " to move";
    }

    const float discR = 15.0f;
    const float gap   = 14.0f;
    const float midY  = 22.0f;

    // Measure the text so the whole "disc + label" group can be centered.
    sf::Text t(font, label, 26);
    t.setFillColor(textColor);
    sf::FloatRect b = t.getLocalBounds();
    float total  = discR * 2.0f + gap + b.size.x;
    float startX = CENTER_X - total / 2.0f;

    sf::CircleShape disc(discR);
    disc.setOrigin({discR, discR});
    disc.setPosition({startX + discR, midY});
    disc.setFillColor(turn == BLACK ? sf::Color::Black : sf::Color::White);
    disc.setOutlineColor(turn == BLACK ? blackOutline : sf::Color(150, 150, 150));
    disc.setOutlineThickness(2.0f);
    window.draw(disc);

    t.setPosition({startX + discR * 2.0f + gap - b.position.x,
                   midY - b.size.y / 2.0f - b.position.y});
    window.draw(t);
}

// ===== BONUS (controls help): top-left hamburger button that opens the overlay =====
void Renderer::drawHelpButton(sf::RenderWindow& window) {
    sf::FloatRect r = btnHelp();
    bool hov = isHover(window, r);

    sf::RectangleShape box(r.size);
    box.setPosition(r.position);
    box.setFillColor(hov ? ACCENT : PANEL_BG);
    box.setOutlineColor(ACCENT);
    box.setOutlineThickness(2.0f);
    window.draw(box);

    // Three horizontal bars (the classic "menu" icon).
    sf::Color barColor = hov ? ACCENT_DK : INK;
    float barW = r.size.x - 16.0f;
    float barX = r.position.x + 8.0f;
    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape bar({barW, 3.0f});
        bar.setPosition({barX, r.position.y + 8.0f + i * 8.0f});
        bar.setFillColor(barColor);
        window.draw(bar);
    }
}

void Renderer::drawHelpOverlay(sf::RenderWindow& window) {
    sf::RectangleShape dim({(float)gui::WINDOW_WIDTH, (float)gui::WINDOW_HEIGHT});
    dim.setFillColor(sf::Color(0, 0, 0, 185));
    window.draw(dim);

    sf::FloatRect panel({160.f, 230.f}, {480.f, 380.f});
    sf::RectangleShape box(panel.size);
    box.setPosition(panel.position);
    box.setFillColor(PANEL_BG);
    box.setOutlineColor(ACCENT);
    box.setOutlineThickness(2.f);
    window.draw(box);

    float cx = panel.position.x + panel.size.x / 2.f;
    drawCenteredText(window, "CONTROLS", cx, 255, 34, ACCENT);

    float x = panel.position.x + 36.f;
    float y = 320.f;
    auto line = [&](const std::string& s) { drawText(window, s, x, y, 20, INK); y += 40.f; };
    line("Left click  -  place a stone");
    line("H  -  suggest a move (hint)");
    line("U  -  undo        Y  -  redo");
    line("T  -  toggle Light / Dark theme");
    drawCenteredText(window, "Click the menu icon again to close", cx, 565, 16, INK_DIM);
}

void Renderer::drawGrid(sf::RenderWindow& window) {
    // Lines
    for (int i = 0; i < BOARD_SIZE; i++) {
        sf::Vertex vLine[] = {
            { sf::Vector2f(gui::MARGIN + i * gui::CELL_SIZE, gui::MARGIN), lineColor },
            { sf::Vector2f(gui::MARGIN + i * gui::CELL_SIZE, gui::MARGIN + gui::BOARD_PIXELS), lineColor }
        };
        sf::Vertex hLine[] = {
            { sf::Vector2f(gui::MARGIN, gui::MARGIN + i * gui::CELL_SIZE), lineColor },
            { sf::Vector2f(gui::MARGIN + gui::BOARD_PIXELS, gui::MARGIN + i * gui::CELL_SIZE), lineColor }
        };
        window.draw(vLine, 2, sf::PrimitiveType::Lines);
        window.draw(hLine, 2, sf::PrimitiveType::Lines);
    }

    // Star points (Hoshi)
    const int hoshi[] = {3, 9, 15};
    for (int r : hoshi) {
        for (int c : hoshi) {
            sf::CircleShape dot(4);
            dot.setFillColor(lineColor);
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
                stone.setOutlineColor(blackOutline);
            } else {
                stone.setFillColor(sf::Color::White);
                stone.setOutlineColor(sf::Color(180, 180, 180));
            }
            stone.setOutlineThickness(1.5f);
            window.draw(stone);
        }
    }
}

void Renderer::drawMarkers(sf::RenderWindow& window, const GameSession& session) {
    auto cellPos = [](int r, int c) {
        return sf::Vector2f((float)gui::MARGIN + c * gui::CELL_SIZE,
                            (float)gui::MARGIN + r * gui::CELL_SIZE);
    };

    // ===== BONUS (Last-move marker): ring the most recently placed stone =====
    const GameEngine& engine = session.getEngine();
    if (engine.hasLastMove()) {
        Point lm = engine.getLastMove();
        sf::CircleShape ring((float)gui::STONE_RADIUS + 2);
        ring.setOrigin({(float)gui::STONE_RADIUS + 2, (float)gui::STONE_RADIUS + 2});
        ring.setPosition(cellPos(lm.row, lm.col));
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineColor(sf::Color(200, 0, 0));
        ring.setOutlineThickness(2.5f);
        window.draw(ring);
    }

    // Highlight an AI move suggestion (green crosshair-style ring).
    if (auto hint = session.getSuggestion()) {
        sf::CircleShape ring((float)gui::STONE_RADIUS);
        ring.setOrigin({(float)gui::STONE_RADIUS, (float)gui::STONE_RADIUS});
        ring.setPosition(cellPos(hint->row, hint->col));
        ring.setFillColor(sf::Color(0, 200, 0, 90));
        ring.setOutlineColor(sf::Color(0, 160, 0));
        ring.setOutlineThickness(2.5f);
        window.draw(ring);
    }
}

void Renderer::drawUI(sf::RenderWindow& window, const GameSession& session) {
    const Board& board = session.getEngine().getBoard();
    int blackCaps = board.getCaptures(BLACK);
    int whiteCaps = board.getCaptures(WHITE);
    bool gameOver = session.isGameOver();

    float barY = (float)gui::WINDOW_WIDTH + 8;
    drawText(window, "Black captures: " + std::to_string(blackCaps), gui::MARGIN, barY, 16, textColor);
    drawText(window, "White captures: " + std::to_string(whiteCaps), gui::MARGIN + 280, barY, 16, textColor);

    // Mandatory: a visible timer showing how long the AI took on its last move.
    std::string mode = session.isVsAI() ? "Mode: vs AI" : "Mode: Hotseat";
    drawText(window, mode + "   |   AI last move: " + std::to_string(session.getLastThinkMs()) + " ms",
             gui::MARGIN, barY + 26, 16, textColor);

    drawText(window, session.getStatusMsg(), gui::MARGIN, barY + 52, 18,
             gameOver ? sf::Color(220, 60, 60) : textColor);
}