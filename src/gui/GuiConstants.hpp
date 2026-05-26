#ifndef GUICONSTANTS_HPP
#define GUICONSTANTS_HPP

#include "../core/Types.hpp"

namespace gui {
    const int CELL_SIZE    = 40;
    const int MARGIN       = 40;
    const int BOARD_PIXELS = (BOARD_SIZE - 1) * CELL_SIZE;
    const int WINDOW_WIDTH = BOARD_PIXELS + 2 * MARGIN;
    const int WINDOW_HEIGHT= WINDOW_WIDTH + 100;
    const int STONE_RADIUS = CELL_SIZE / 2 - 3;
}

#endif // GUICONSTANTS_HPP