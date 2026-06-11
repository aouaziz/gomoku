#include "Evaluator.hpp"
#include "../engine/Rules.hpp"



int Evaluator::evaluate(const Board& board, Cell aiColor) {
    int score = 0;
    Cell oppColor = getOpponent(aiColor);

    // ---- Rubric: "current captures" — captured pairs already scored ----------
    // (Huge priority for the Pente variant.)
    int aiCaptures = board.getCaptures(aiColor);
    int oppCaptures = board.getCaptures(oppColor);

    if (aiCaptures >= 10) return 1000000;  // Instant win
    if (oppCaptures >= 10) return -1000000; // Instant loss

    score += aiCaptures * 2000;
    score -= oppCaptures * 2000;

    // ---- Rubric: "alignments" + "freedom" + "potential win by alignment" -----
    // Scan every line (Horizontal, Vertical, 2 Diagonals). evaluateLine() weighs
    // each run by length AND by its number of open ends (free / half-free /
    // flanked), and a flanked run with no room scores 0.
    const int dr[] = {0, 1, 1, 1};
    const int dc[] = {1, 0, 1, -1};

    // Tallies used by the "figures" (combinations) and "dynamic" (tempo) terms.
    int aiThreats = 0, oppThreats = 0; // number of strong threats (open-3 / four)
    int aiBestDev = 0, oppBestDev = 0; // strongest developing threat per side

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            Cell cell = board.getCell(r, c);
            if (cell == EMPTY) continue;

            bool isAI = (cell == aiColor);

            for (int d = 0; d < 4; ++d) {
                int count = 0;
                int openEnds = 0;

                countConsecutive(board, r, c, dr[d], dc[d], cell, count, openEnds);

                // Only score lines from their starting point to avoid double-counting
                int prevR = r - dr[d];
                int prevC = c - dc[d];
                if (board.inBounds(prevR, prevC) && board.getCell(prevR, prevC) == cell) {
                    continue;
                }

                score += evaluateLine(count, openEnds, isAI);

                // Classify the threat for the combination / tempo terms.
                int level = 0;
                if (count >= 4 && openEnds >= 1)      level = 4; // one move from five
                else if (count == 3 && openEnds == 2) level = 3; // open three
                if (level > 0) {
                    if (isAI) { aiThreats++;  if (level > aiBestDev)  aiBestDev  = level; }
                    else      { oppThreats++; if (level > oppBestDev) oppBestDev = level; }
                }
            }
        }
    }

    // ---- Rubric: "figures" — advantageous combinations (forks) ---------------
    // Two or more simultaneous strong threats are (usually) unstoppable, so a
    // double-threat is worth far more than the sum of its individual lines.
    if (aiThreats  >= 2) score += 3000;
    if (oppThreats >= 2) score -= 3300; // slightly more wary of the opponent's forks

    // ---- Rubric: "potential captures" ---------------------------------------
    score += potentialCaptureScore(board, aiColor);

    // ---- Rubric: "dynamic part" — tempo / initiative ------------------------
    // `aiColor` is the side to move at this node, so it can act on its threats
    // FIRST. The same static shape is therefore worth more for whoever's turn it
    // is — a turn-dependent (dynamic) weighting of the board state.
    if (aiBestDev == 4)      score += 4000; // can complete a five on the next move
    else if (aiBestDev == 3) score += 400;  // can push an open-three to an open-four

    return score;
}

// ---- Rubric: "potential captures" ------------------------------------------
// Detects pairs that are one move away from being captured. For every existing
// stone A we look for the pattern  A B B _  along all 8 directions: the owner of
// A could play the empty cell to flank and capture the B pair. We value the
// side-to-move's (aiColor's) opportunities higher than the opponent's, since the
// mover can take the pair immediately (another small dynamic / tempo effect).
int Evaluator::potentialCaptureScore(const Board& board, Cell aiColor) {
    const int dr[] = {-1, -1, -1,  0, 0,  1, 1, 1};
    const int dc[] = {-1,  0,  1, -1, 1, -1, 0, 1};

    int score = 0;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            Cell a = board.getCell(r, c);
            if (a == EMPTY) continue;
            Cell b = getOpponent(a); // the colour of the pair A would capture

            for (int d = 0; d < 8; ++d) {
                int r1 = r + dr[d],     c1 = c + dc[d];
                int r2 = r + 2 * dr[d], c2 = c + 2 * dc[d];
                int r3 = r + 3 * dr[d], c3 = c + 3 * dc[d];
                if (board.inBounds(r3, c3) &&
                    board.getCell(r1, c1) == b &&
                    board.getCell(r2, c2) == b &&
                    board.getCell(r3, c3) == EMPTY) {
                    // Owner of A can play (r3,c3) to capture the B pair.
                    if (a == aiColor) score += 700;  // we threaten an opponent pair
                    else              score -= 450;  // one of our pairs is vulnerable
                }
            }
        }
    }
    return score;
}

void Evaluator::countConsecutive(const Board& board, int r, int c, int dr, int dc, Cell color, int& count, int& openEnds) {
    count = 0;
    openEnds = 0;

    // Check backwards for an open end
    int prevR = r - dr;
    int prevC = c - dc;
    if (board.inBounds(prevR, prevC) && board.getCell(prevR, prevC) == EMPTY) {
        openEnds++;
    }

    // Count forward
    int currR = r;
    int currC = c;
    while (board.inBounds(currR, currC) && board.getCell(currR, currC) == color) {
        count++;
        currR += dr;
        currC += dc;
    }

    // Check forwards for an open end
    if (board.inBounds(currR, currC) && board.getCell(currR, currC) == EMPTY) {
        openEnds++;
    }
}

int Evaluator::evaluateLine(int count, int openEnds, bool isAI) {
    if (count >= 5) return isAI ? 100000 : -100000;
    
    int score = 0;
    if (count == 4) {
        if (openEnds == 2) score = 10000; // Open 4 (Unstoppable win)
        else if (openEnds == 1) score = 1000;  // Blocked 4
    } 
    else if (count == 3) {
        if (openEnds == 2) score = 1000;  // Open 3
        else if (openEnds == 1) score = 100;   // Blocked 3
    }
    else if (count == 2) {
        if (openEnds == 2) score = 100;   // Open 2
        else if (openEnds == 1) score = 10;    // Blocked 2
    }

    // If it's the opponent's line, we want to subtract the score
    // We multiply by 1.2 to make the AI slightly defensive (prefers blocking)
    return isAI ? score : -(score * 1.2); 
}