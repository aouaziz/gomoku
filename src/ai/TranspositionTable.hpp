#ifndef TRANSPOSITIONTABLE_HPP
#define TRANSPOSITIONTABLE_HPP

#include <cstdint>
#include <vector>

enum class HashFlag { EXACT, ALPHA, BETA };

struct TTEntry {
    uint64_t zkey = 0;
    int depth = -1;
    int value = 0;
    HashFlag flag = HashFlag::EXACT;
};

class TranspositionTable {
public:
    TranspositionTable();

    void store(uint64_t key, int depth, int value, HashFlag flag) ;

    bool lookup(uint64_t key, int depth, int alpha, int beta, int& outValue) ;

    void clear() ;

private:
    std::vector<TTEntry> table;
};

#endif // TRANSPOSITIONTABLE_HPP