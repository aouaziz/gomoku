#ifndef ZOBRIST_HPP
#define ZOBRIST_HPP


#include "../core/Types.hpp"
#include <cstdint>
#include <random>

class Zobrist {

    public:
        
        static void init();

        // Get the random 64-bit number for a specific piece at a specific location
        static uint64_t getKey(int r, int c, Cell color);
private:
    // [19][19][3] -> Row, Col, Color (0=Empty, 1=Black, 2=White)
    static uint64_t keys[BOARD_SIZE][BOARD_SIZE][3];

};

#endif // ZOBRIST_HPP