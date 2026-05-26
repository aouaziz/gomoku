#include "src/engine/GameEngine.hpp"
#include "src/engine/Rules.hpp"
#include "src/core/Types.hpp"
#include <iostream>
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// Mini test framework
// ─────────────────────────────────────────────────────────────────────────────
static int passed = 0;
static int failed = 0;

#define TEST(name, expr) do { \
    if (expr) { \
        std::cout << "[PASS] " << name << "\n"; \
        ++passed; \
    } else { \
        std::cout << "  [FAIL] " << name << "  (line " << __LINE__ << ")\n"; \
        ++failed; \
    } \
} while(0)

#define TEST_THROWS(name, engine, r, c, color) do { \
    bool threw = false; \
    try { (engine).applyMove(r, c, color); } \
    catch (const std::invalid_argument&) { threw = true; } \
    TEST(name, threw); \
} while(0)

#define TEST_NO_THROW(name, engine, r, c, color) do { \
    bool threw = false; \
    try { MoveResult _mv = (engine).applyMove(r, c, color); (void)_mv; } \
    catch (const std::invalid_argument& e) { \
        threw = true; \
        std::cout << "     -> exception: " << e.what() << "\n"; } \
    TEST(name, !threw); \
} while(0)

static void section(const std::string& title) {
    std::cout << "\n══════════════════════════════════════════\n";
    std::cout << "  " << title << "\n";
    std::cout << "══════════════════════════════════════════\n";
}

// ─────────────────────────────────────────────────────────────────────────────
void test_basic_placement() {
    section("1. Basic Placement");
    GameEngine g;
    TEST_NO_THROW("Place on empty cell (9,9)",          g, 9,  9, BLACK);
    TEST_THROWS  ("Place on occupied cell same color",  g, 9,  9, BLACK);
    TEST_THROWS  ("Place on occupied cell other color", g, 9,  9, WHITE);
    TEST_THROWS  ("Row -1 out of bounds",               g, -1,  0, BLACK);
    TEST_THROWS  ("Col -1 out of bounds",               g,  0, -1, BLACK);
    TEST_THROWS  ("Row 19 out of bounds",               g, 19,  0, BLACK);
    TEST_THROWS  ("Col 19 out of bounds",               g,  0, 19, BLACK);
}

// ─────────────────────────────────────────────────────────────────────────────
void test_captures() {
    section("2. Captures");
    {
        GameEngine g;
        g.applyMove(5, 5, BLACK);
        g.applyMove(5, 6, WHITE);
        g.applyMove(5, 7, WHITE);
        MoveResult mv = g.applyMove(5, 8, BLACK);
        TEST("Horizontal: 2 stones in MoveResult",   mv.captured.size() == 2);
        TEST("Horizontal: getCaptures(BLACK) == 2",  g.getBoard().getCaptures(BLACK) == 2);
        TEST("Captured cell (5,6) is EMPTY",         g.getBoard().getCell(5,6) == EMPTY);
        TEST("Captured cell (5,7) is EMPTY",         g.getBoard().getCell(5,7) == EMPTY);
    }
    {
        GameEngine g;
        g.applyMove(2, 2, BLACK);
        g.applyMove(3, 2, WHITE);
        g.applyMove(4, 2, WHITE);
        MoveResult mv = g.applyMove(5, 2, BLACK);
        TEST("Vertical capture: 2 captured", mv.captured.size() == 2);
    }
    {
        GameEngine g;
        g.applyMove(1, 1, BLACK);
        g.applyMove(2, 2, WHITE);
        g.applyMove(3, 3, WHITE);
        MoveResult mv = g.applyMove(4, 4, BLACK);
        TEST("Diagonal (down-right) capture: 2 captured", mv.captured.size() == 2);
    }
    {
        GameEngine g;
        g.applyMove(1, 5, BLACK);
        g.applyMove(2, 4, WHITE);
        g.applyMove(3, 3, WHITE);
        MoveResult mv = g.applyMove(4, 2, BLACK);
        TEST("Anti-diagonal (down-left) capture: 2 captured", mv.captured.size() == 2);
    }
    {
        GameEngine g;
        g.applyMove(5, 5, BLACK);
        g.applyMove(5, 6, WHITE);
        MoveResult mv = g.applyMove(5, 7, BLACK);
        TEST("Single stone not captured",  mv.captured.size() == 0);
        TEST("getCaptures still 0",        g.getBoard().getCaptures(BLACK) == 0);
    }
    {
        GameEngine g;
        g.applyMove(5, 5, BLACK);
        g.applyMove(5, 6, WHITE);
        g.applyMove(5, 7, WHITE);
        g.applyMove(5, 8, WHITE);
        MoveResult mv = g.applyMove(5, 9, BLACK);
        TEST("Triple not captured", mv.captured.size() == 0);
    }
    {
        GameEngine g;
        g.applyMove(5, 5, WHITE);
        g.applyMove(5, 7, WHITE);
        TEST_NO_THROW("Moving into flanked position is LEGAL", g, 5, 6, BLACK);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
void test_ten_captures_win() {
    section("3. Win by 10 Captures");
    GameEngine g;
    int rows[] = {1, 3, 5, 7, 9}; // Space out the rows to avoid accidental 5-in-a-row!
    
    for (int i = 0; i < 4; ++i) {
        int r = rows[i];
        g.applyMove(r, 1, BLACK);
        g.applyMove(r, 2, WHITE);
        g.applyMove(r, 3, WHITE);
        g.applyMove(r, 4, BLACK);
    }
    TEST("8 captures: hasTenCaptures == false", !Rules::hasTenCaptures(g.getBoard(), BLACK));
    
    // 5th capture
    int r = rows[4];
    g.applyMove(r, 1, BLACK);
    g.applyMove(r, 2, WHITE);
    g.applyMove(r, 3, WHITE);
    g.applyMove(r, 4, BLACK);
    
    TEST("10 captures: hasTenCaptures == true",  Rules::hasTenCaptures(g.getBoard(), BLACK));
    TEST("White never reached 10",              !Rules::hasTenCaptures(g.getBoard(), WHITE));
}

void test_white_wins() {
    section("8. White Can Also Win");
    {
        GameEngine g;
        for (int c = 0; c < 5; ++c) g.applyMove(5, c, WHITE);
        TEST("White five-in-a-row detected", Rules::hasFiveAt(g.getBoard(), 5, 4, WHITE));
    }
    {
        GameEngine g;
        int rows[] = {1, 3, 5, 7, 9}; // Space them out!
        for (int i = 0; i < 5; ++i) {
            int r = rows[i];
            g.applyMove(r, 1, WHITE);
            g.applyMove(r, 2, BLACK);
            g.applyMove(r, 3, BLACK);
            g.applyMove(r, 4, WHITE);
        }
        TEST("White wins by 10 captures", Rules::hasTenCaptures(g.getBoard(), WHITE));
    }
}


// ─────────────────────────────────────────────────────────────────────────────
void test_five_in_a_row() {
    section("4. Five-in-a-Row");
    {
        GameEngine g;
        for (int c = 0; c < 4; ++c) g.applyMove(9, c, BLACK);
        g.applyMove(9, 4, BLACK);
        TEST("Horizontal 5-in-a-row", Rules::hasFiveAt(g.getBoard(), 9, 4, BLACK));
    }
    {
        GameEngine g;
        for (int r = 0; r < 4; ++r) g.applyMove(r, 9, BLACK);
        g.applyMove(4, 9, BLACK);
        TEST("Vertical 5-in-a-row", Rules::hasFiveAt(g.getBoard(), 4, 9, BLACK));
    }
    {
        GameEngine g;
        for (int i = 0; i < 4; ++i) g.applyMove(i, i, BLACK);
        g.applyMove(4, 4, BLACK);
        TEST("Diagonal (down-right) 5-in-a-row", Rules::hasFiveAt(g.getBoard(), 4, 4, BLACK));
    }
    {
        GameEngine g;
        for (int i = 0; i < 4; ++i) g.applyMove(i, 8 - i, BLACK);
        g.applyMove(4, 4, BLACK);
        TEST("Diagonal (down-left) 5-in-a-row", Rules::hasFiveAt(g.getBoard(), 4, 4, BLACK));
    }
    {
        GameEngine g;
        // To test 6-in-a-row, we connect a group of 2 and a group of 3
        g.applyMove(9, 0, BLACK);
        g.applyMove(9, 1, BLACK); // Group of 2
        
        g.applyMove(9, 3, BLACK);
        g.applyMove(9, 4, BLACK);
        g.applyMove(9, 5, BLACK); // Group of 3
        
        g.applyMove(9, 2, BLACK); // Drops in the middle to form 6!
        TEST("Six-in-a-row also wins (5 or more)", Rules::hasFiveAt(g.getBoard(), 9, 2, BLACK));
    }
    {
        GameEngine g;
        for (int c = 0; c < 3; ++c) g.applyMove(9, c, BLACK);
        g.applyMove(9, 3, BLACK);
        TEST("Four-in-a-row is NOT a win", !Rules::hasFiveAt(g.getBoard(), 9, 3, BLACK));
    }
    {
        GameEngine g;
        for (int c = 0; c < 4; ++c) g.applyMove(9, c, BLACK);
        g.applyMove(9, 4, BLACK); // 5th stone
        TEST("hasFive (board scan) agrees", Rules::hasFive(g.getBoard(), BLACK));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// ─────────────────────────────────────────────────────────────────────────────
void test_breakable_five() {
    section("5. Breakable Five");
    {
        GameEngine g;
        
        // 1. Setup the vulnerability FIRST.
        // We put a Black stone at (10,4) and a White stone at (8,4).
        // Later, if Black plays at (9,4), White can play at (11,4) to capture both!
        g.applyMove(10, 4, BLACK);
        g.applyMove(8,  4, WHITE);
        
        // 2. Build 4 stones of the 5-in-a-row
        g.applyMove(9, 3, BLACK);
        g.applyMove(9, 5, BLACK);
        g.applyMove(9, 6, BLACK);
        g.applyMove(9, 7, BLACK);
        
        // 3. Black plays the 5th stone right in the vulnerable spot.
        // The engine will see the 5-in-a-row, but realize White can break it,
        // so it will NOT end the game!
        TEST_NO_THROW("Placing 5th breakable stone", g, 9, 4, BLACK);
        
        TEST("Five is detected",  Rules::hasFiveAt(g.getBoard(), 9, 4, BLACK));
        TEST("Five is breakable", Rules::isFiveBreakable(g.getBoard(), BLACK));
        TEST("Game is NOT over",  !g.isGameOver()); // Proves the game kept going!
    }
    {
        GameEngine g;
        // Build an isolated 4-in-a-row
        for (int c = 0; c < 4; ++c) g.applyMove(9, c, BLACK);
        
        // Place the 5th stone
        g.applyMove(9, 4, BLACK);
        
        TEST("Isolated five detected",       Rules::hasFiveAt(g.getBoard(), 9, 4, BLACK));
        TEST("Isolated five not breakable", !Rules::isFiveBreakable(g.getBoard(), BLACK));
        TEST("Game IS over",                 g.isGameOver()); // Proves the game ended!
    }
}

// ─────────────────────────────────────────────────────────────────────────────
void test_double_three() {
    section("6. Double-Three Rule");
    {
        GameEngine g;
        g.applyMove(9, 7, BLACK);
        g.applyMove(9, 8, BLACK);
        g.applyMove(7,  9, BLACK);
        g.applyMove(8,  9, BLACK);
        TEST_THROWS("Double-three at centre is forbidden", g, 9, 9, BLACK);
    }
    {
        GameEngine g;
        g.applyMove(9, 7, BLACK);
        g.applyMove(9, 8, BLACK);
        g.applyMove(7,  9, BLACK);
        g.applyMove(8,  9, BLACK);
        TEST("isDoubleThree(9,9,BLACK) == true", Rules::isDoubleThree(g.getBoard(), 9, 9, BLACK));
    }
    {
        GameEngine g;
        g.applyMove(9, 7, BLACK);
        g.applyMove(9, 8, BLACK);
        TEST_NO_THROW("Single free-three is allowed", g, 9, 9, BLACK);
    }
    {
        GameEngine g;
        g.applyMove(9, 7, BLACK);
        g.applyMove(9, 8, BLACK);
        g.applyMove(7,  9, BLACK);
        g.applyMove(8,  9, BLACK);
        
        g.applyMove(12, 12, BLACK);
        g.applyMove(11, 11, WHITE);
        g.applyMove(10, 10, WHITE);
        
        TEST_NO_THROW("Double-three + capture is LEGAL (exception)", g, 9, 9, BLACK);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
void test_undo() {
    section("7. Undo Move [undoMove()]");
    {
        GameEngine g;
        g.applyMove(9, 9, BLACK);
        TEST("Cell occupied after move", g.getBoard().getCell(9,9) == BLACK);
        g.undoMove(); // No longer takes parameters!
        TEST("Cell empty after undo",    g.getBoard().getCell(9,9) == EMPTY);
    }
    {
        GameEngine g;
        g.applyMove(5, 5, BLACK);
        g.applyMove(5, 6, WHITE);
        g.applyMove(5, 7, WHITE);
        g.applyMove(5, 8, BLACK);
        TEST("Captures before undo: 2",          g.getBoard().getCaptures(BLACK) == 2);
        g.undoMove();
        TEST("Captures after undo: 0",           g.getBoard().getCaptures(BLACK) == 0);
        TEST("W(5,6) restored",                  g.getBoard().getCell(5,6) == WHITE);
        TEST("W(5,7) restored",                  g.getBoard().getCell(5,7) == WHITE);
        TEST("B(5,8) anchor removed",            g.getBoard().getCell(5,8) == EMPTY);
    }
    {
        GameEngine g;
        g.applyMove(3, 3, WHITE);
        g.undoMove();
        TEST("Non-capture undo: cell empty",        g.getBoard().getCell(3,3) == EMPTY);
        TEST("Non-capture undo: captures still 0",  g.getBoard().getCaptures(WHITE) == 0);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
void test_helpers() {
    section("9. Helper Methods");
    GameEngine g;
    TEST("inBounds(0,0)   == true",  g.getBoard().inBounds(0,  0));
    TEST("inBounds(18,18) == true",  g.getBoard().inBounds(18, 18));
    TEST("inBounds(-1,0)  == false", !g.getBoard().inBounds(-1, 0));
    TEST("inBounds(19,0)  == false", !g.getBoard().inBounds(19, 0));
    TEST("inBounds(0,19)  == false", !g.getBoard().inBounds(0, 19));
    TEST("getOpponent(BLACK) == WHITE", getOpponent(BLACK) == WHITE);
    TEST("getOpponent(WHITE) == BLACK", getOpponent(WHITE) == BLACK);
    g.applyMove(0, 0, BLACK);
    TEST("getCell after applyMove == BLACK", g.getBoard().getCell(0,0) == BLACK);
}

// ─────────────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "\n╔══════════════════════════════════════════╗\n";
    std::cout << "║         GOMOKU RULE TEST SUITE           ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n";

    test_basic_placement();
    test_captures();
    test_ten_captures_win();
    test_five_in_a_row();
    test_breakable_five();
    test_double_three();
    test_undo();
    test_white_wins();
    test_helpers();

    std::cout << "\n══════════════════════════════════════════\n";
    std::cout << "  RESULTS: " << passed << " passed, "
              << failed << " failed\n";
    std::cout << "══════════════════════════════════════════\n\n";
    return (failed == 0) ? 0 : 1;
}