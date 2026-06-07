# ⛩️ Gomoku AI Engine — Pente Variant

> A high-performance, strictly-ruled C++ implementation of Gomoku (Pente variant) featuring a custom graphical interface built with **SFML 3** and a highly optimized Minimax AI engine capable of searching multiple turns ahead in **under 0.5 seconds**.

---

## 📋 Table of Contents

- [Game Rules](#-game-rules)
- [Artificial Intelligence](#-artificial-intelligence)
- [Architecture](#-architecture)
- [Prerequisites](#-prerequisites--dependencies)
- [Build & Run](#-build--run)
- [Roadmap](#-roadmap)

---

## 📜 Game Rules

This engine acts as a **strict referee**, enforcing the full professional Gomoku/Pente ruleset:

| Rule | Description |
|---|---|
| **5-in-a-row** | Align 5 or more stones in any direction to win. |
| **Capture Victory** | Capture 10 of the opponent's stones (5 pairs) to win. |
| **Capture Mechanic** | Flank a pair of opponent stones on both sides to capture them. |
| **Breakable Five** | A 5-in-a-row does **not** immediately win if the opponent can capture a stone from that line on their very next turn. |
| **Double-Three Rule** | A player **cannot** place a stone that simultaneously creates two or more open-threes — *unless* that same move also captures opponent stones. |

---

## 🧠 Artificial Intelligence

The AI is designed to be **ruthless, defensive, and lightning-fast**, strictly respecting the `< 0.5s` response time constraint through a stack of advanced techniques:

### Minimax Algorithm
The core decision-making engine. The AI simulates future moves, always assuming the human plays optimally, and navigates the game tree to find the mathematically best path.

### Alpha-Beta Pruning
Eliminates branches of the game tree that are provably worse than already-found moves — reducing billions of calculations to thousands, with no loss in decision quality.

### Zobrist Hashing & Transposition Tables
Every board state is fingerprinted with a unique **64-bit integer** via XOR bitwise operations. Scores for previously evaluated states are cached in a Transposition Table; revisiting any state via a different move order is an instant O(1) lookup.

### Move Ordering
Candidate moves are sorted before evaluation — captures and threats are explored first. This dramatically improves Alpha-Beta pruning efficiency, yielding roughly a **10× speedup**.

### Iterative Deepening
The AI searches at depth 1, then 2, then 3... When the **0.45s timer** fires, it immediately aborts the current search and returns the best move found at the last *completed* depth — guaranteeing a response is always ready on time.

---

## 🏗️ Architecture

The codebase follows a strict **decoupled layered architecture**, ensuring thread-safety and zero state corruption during deep AI simulations.

```
src/
├── core/                  # Fundamental data types
│   └── Types.hpp          # Cell enums, Point and MoveResult structs
│
├── engine/                # AI simulation playground (no UI or turn logic)
│   ├── Board              # Pure data container (bounds checking, state arrays)
│   ├── Rules              # 100% stateless static rule-validation functions
│   ├── GameEngine         # State mutator (applyMove, undoMove, win detection)
│   └── Zobrist            # 64-bit random key generator for hashing
│
├── game/                  # Orchestrator layer
│   └── GameSession        # Turn switching, AI scheduling, string/UI generation
│
├── gui/                   # SFML presentation layer
│   ├── GameWindow         # Window lifecycle and event loop polling
│   ├── InputHandler       # Converts raw pixel clicks to board coordinates
│   └── Renderer           # Pure drawing logic (grid, hoshi points, stones, text)
│
└── ai/                    # Artificial intelligence
    ├── AI                 # Time management, Minimax loop, exception safety
    ├── MoveGenerator      # Reduces 361 moves to ~30 nearby candidates & sorts them
    ├── Evaluator          # The "brain": scores board lines (+100,000 for wins)
    └── TranspositionTable # Custom cache array for Zobrist hashes
```

---

## 🛠️ Prerequisites & Dependencies

| Requirement | Details |
|---|---|
| **C++ Compiler** | C++20 support required (`g++` or `clang`) |
| **Make** | Standard build system |
| **SFML 3.x** | Uses SFML 3.0 API (`openFromFile`, `std::optional` event polling) |

**Installing SFML on Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install libsfml-dev
```

**Installing SFML on macOS (Homebrew):**
```bash
brew install sfml
```

---

## 🚀 Build & Run

**Compile:**
```bash
make
```

**Play:**
```bash
./gomoku
```
> By default, the human plays as **Black** (first move) and the AI plays as **White**.

**Run the rule-validation test suite:**
```bash
make test
```

**Clean build artifacts:**
```bash
make clean    # Removes object files
make fclean   # Removes object files and executables
```

---

## 🔮 Roadmap

- [ ] SFML main menu with **Player vs Player** and **Player vs AI** mode selection, including color choice
- [ ] Visual indicator highlighting the **last move played**
- [ ] Expanded Evaluator heuristics to recognize split threats (e.g., broken 3-in-a-row)

---

<p align="center">Built with C++20 · SFML 3 · Minimax · Alpha-Beta Pruning</p>