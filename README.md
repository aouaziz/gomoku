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
| **Breakable Five** | A 5-in-a-row does **not** immediately win if, on the very next turn, the opponent can either capture a stone out of that line **or** capture their 5th pair (reaching 10 captures). |
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
The AI searches at depth 1, then 2, then 3... When the **~420ms timer** fires, it immediately aborts the current search (via an exception that unwinds cleanly) and returns the best move found at the last *completed* depth — guaranteeing a response is always ready well under the 0.5s limit.

### Principal Variation Search & Late Move Reductions
On top of plain alpha-beta, later moves are first probed with a **null window** (PVS) and quiet, low-ranked moves are searched at **reduced depth** (LMR), with a full re-search only when one unexpectedly beats the current best. These are what let the nominal search reach **depth 9–10** within the time budget — tactical/forcing positions routinely hit 10.

> The search is implemented as **negamax** (a single-perspective formulation of Min-Max where each node negates the child score), with alpha-beta pruning, PVS, LMR, a transposition table, and killer-move / history move ordering.

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
./Gomoku
```
> **Flow:** Title screen → **Start Game** → pick **Mode** (vs AI / Hotseat) → if vs AI, choose **difficulty + your color** → play.
> During a game the bottom bar shows the AI's last move-time, the top-left shows **whose turn** it is, the last stone is ringed in red, and the **? CONTROLS** button (top-right) lists every shortcut.

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

## 🎮 Controls (cheat-sheet)

| Input | Action |
|---|---|
| **Mouse (left click)** | Navigate menus / place a stone |
| **Enter** | Start (on the title screen) |
| **`H`** | Suggest a move (hint, shown — not played) |
| **`U`** | Undo |
| **`Y`** | Redo |
| **`T`** | Toggle Dark / Light theme (works on every screen) |
| **`R`** | Return to the main menu |
| **Esc** | Go back one menu step |
| **`E` / `M` / `D`** | Easy / Medium / Hard (on the AI-setup screen) |
| **? CONTROLS** (top-right button) | Open/close the in-game controls overlay |

> All bonus code is tagged in the source with a `// BONUS (...)` comment — run `grep -rn "BONUS" src/` to find every one.

## ✨ Bonuses (detailed)

**1. AI difficulty selector.** On the *AI setup* screen you choose **Easy / Medium / Hard**, which maps to the AI's per-move time budget (**100 / 250 / 420 ms**). A larger budget lets iterative deepening finish more plies, so the AI literally thinks further ahead on higher difficulty. The choice flows `GameWindow → GameSession::setAITime → AI::setTimeLimit`, where `timeIsUp()` enforces it.
*Test:* pick Easy vs Hard and watch the “AI last move: N ms” field and the `Depth N done` lines in the terminal — Hard reaches deeper depths.

**2. Undo (`U`).** Takes back the last move, fully restoring captured stones and the capture counter. In **vs-AI** mode it rewinds a whole round (the AI's reply *and* your move) so it's your turn again; in **hotseat** it rewinds one ply. `GameEngine::undoMove()` returns the undone move so the session can restore whose turn it is.
*Test:* set up a capture, undo, and confirm the captured pair reappears and the counter drops back.

**3. Redo (`Y`).** Re-applies moves you just undid (mirroring undo's one-ply / one-round behavior). Undone moves are kept on a redo stack that is cleared the moment you make a new move, so you can't redo into an abandoned line.
*Test:* undo a move, press `Y` to bring it back; then undo, play elsewhere, and note that `Y` now does nothing.

**4. Dark / Light theme (`T`).** Re-skins the entire board — background, grid lines, hoshi dots, text, and stone outlines — between a light wood theme and a dark slate theme. Works on the menus too.
*Test:* press `T` repeatedly on any screen.

**5. Last-move marker.** The most recently placed stone is ringed in **red**, so it's obvious what just happened (especially after the AI moves). *(The separate green ring is the `H` move-suggestion.)*
*Test:* play any move — a red ring appears on it and follows the latest stone.

> Plus quality-of-life UI: a **gamer-style multi-screen menu**, a **turn indicator** (colored stone + label) top-left, and the **? CONTROLS** overlay.

## 🔮 Roadmap

- [ ] Expanded Evaluator heuristics to recognize split threats (e.g., broken 3-in-a-row)
- [ ] Alternative opening rules (Pro / Swap / Swap2)
- [ ] Highlight the winning five stones on game over

---

<p align="center">Built with C++20 · SFML 3 · Negamax · Alpha-Beta · PVS · LMR · Transposition Table</p>