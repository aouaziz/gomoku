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
**Current Phase:** Phase 2 Complete (Modular Architecture & GUI Integration). Ready for AI.

The codebase has been heavily refactored into a strict, decoupled Layered Architecture to support high-performance AI simulations:

### 1. Core Layer (`src/core/`)
* **`Types.hpp`**: Contains fundamental data structures (`Cell` Enum, `Point`, `MoveResult`).

### 2. Engine Layer (`src/engine/`) *[AI Playground]*
* **`Board`**: A pure data container. Handles 2D array access, bounds checking, and capture counting. Zero game logic.
* **`Rules`**: A 100% stateless class of `static` functions. Evaluates rule checks (`isDoubleThree`, `isFiveBreakable`, `checkCaptures`) using temporary board copies to ensure absolute thread-safety and zero state corruption during AI tree-search.
* **`GameEngine`**: The state mutator. Validates and applies moves, strictly tracks history, manages precise `undoMove` operations (crucial for Minimax), and evaluates win/loss states.

### 3. Game Layer (`src/game/`)
* **`GameSession`**: The bridge between the Engine and the GUI/AI. Manages whose turn it is, catches illegal move exceptions, processes text generation for the UI (e.g., "5-in-a-row formed!"), and will act as the orchestrator for alternating Human vs. AI turns.

### 4. GUI Layer (`src/gui/`)
* **`GameWindow`**: Manages the SFML 3.0 window lifecycle and event polling.
* **`Renderer`**: Strictly handles drawing shapes, board lines, stones, and text to the screen. 
* **`InputHandler`**: Safely translates raw screen pixel clicks into strict `Point` (row/col) board coordinates.

## 🚀 Roadmap & Next Steps
- [x] **Phase 1: Core Engine** (Board, Rules, Captures, Undo, Double-Three, Breakable Five).
- [x] **Phase 2: GUI (SFML) & Refactor** (Decoupled architecture, Event Loop, Visuals).
- [ ] **Phase 3: Artificial Intelligence (Current Target)**
  - Implement Minimax algorithm with Alpha-Beta Pruning.
  - Implement Iterative Deepening to ensure moves return in under 0.5 seconds.
  - Design a strong heuristic evaluation function (scoring open fours, captures, etc.).
  - Implement Zobrist Hashing (Transposition Tables) for speed optimization.