# Gomoku (Pente-Variant) AI Project

## 📌 Project Overview
This project is a C++ implementation of Gomoku with specialized rules (similar to Pente/Renju variants). The goal is to build a high-performance, strictly-ruled game engine, followed by a graphical user interface (GUI) using SFML, and finally a highly optimized Minimax AI with Alpha-Beta pruning.

## 📜 Game Rules Enforced
The game is played on a 19x19 grid. The engine strictly enforces the following complex rules:
1. **Win Condition 1 (5-in-a-row):** A player wins by aligning 5 or more stones in any direction.
2. **Win Condition 2 (Captures):** A player wins by capturing 10 of the opponent's stones (5 pairs).
3. **Capture Mechanic:** A player captures a pair of opponent stones by flanking them on both sides. (e.g., placing `Black` on `Black - White - White - [Empty]` captures the two White stones).
4. **The "Breakable Five" Rule:** If a player gets 5-in-a-row, they do not instantly win IF the opponent can play a move on their very next turn that captures a stone from that 5-in-a-row, breaking the line.
5. **The Double-Three Rule:** A player cannot place a single stone that creates two or more "Free-Threes" (Open-Threes) simultaneously. 
   * *Exception:* A move that creates a Double-Three is perfectly legal IF that specific move also captures opponent stones.

## 🏗️ Code Architecture & State
**Current Phase:** Phase 1 Complete (Core Engine & Terminal Logic).

### Data Structures (`gomoku.hpp`)
* `Cell` Enum: `EMPTY`, `BLACK`, `WHITE`
* `Point` Struct: Holds `row` and `col`.
* `MoveResult` Struct: Crucial for the AI's `undo` functionality. It stores the `Point` placed, the `color`, and a `std::vector<Point>` of any stones captured during that move.

### Core Files
1. **`gomoku.cpp` (Physics & Basic Mechanics)**
   * `applyMove`: Validates rules, places stones, handles capture removal, logs to history.
   * `undoMove`: Reverts a move and restores captured stones exactly as they were (used for AI simulation).
   * `checkCaptures`: Scans 8 directions for the flanking capture pattern.
   * `hasFiveAt` / `hasTenCaptures`: Basic win state detection.
   
2. **`Logic.cpp` (Advanced Refereeing & Simulations)**
   * `isOpenFourAt`: Detects if exactly 4 stones are flanked by empty space on both sides.
   * `isFreeThreeAt`: Uses board simulation to see if an empty space would create an Open Four.
   * `isDoubleThree`: Checks 4 axes to prevent illegal Double-Three placements.
   * `isFiveBreakable` & `checkIfCorrectFive`: Scans the entire board to simulate opponent captures to see if a 5-in-a-row can be broken.

3. **`main.cpp` (The Game Loop)**
   * Terminal-based loop handling alternating turns, taking input, wrapping moves in `try/catch` for validation, and resolving end-game states.

## 🚀 Roadmap & Next Steps
- [x] **Phase 1: Core Engine** (Board, Rules, Captures, Undo, Double-Three, Breakable Five).
- [ ] **Phase 2: GUI (SFML)**
  - Draw 19x19 grid and stones.
  - Implement mouse-click to board-coordinate mapping.
  - Display current turn, captures count, and win pop-ups.
- [ ] **Phase 3: Artificial Intelligence**
  - Implement Minimax algorithm with Alpha-Beta Pruning.
  - Implement Iterative Deepening to ensure moves return in under 0.5 seconds.
  - Design a strong heuristic evaluation function (scoring open fours, captures, etc.).
  - Implement Zobrist Hashing (Transposition Tables) for speed optimization.