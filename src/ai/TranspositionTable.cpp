#include "TranspositionTable.hpp"


TranspositionTable::TranspositionTable() {
        // A prime number for array size reduces hash collisions
        table.resize(1000003); 
    }


    void TranspositionTable::store(uint64_t key, int depth, int value, HashFlag flag) {
        TTEntry& entry = table[key % table.size()];
        // Always overwrite if we looked deeper, or if the spot is empty
        if (entry.depth <= depth) { 
            entry.zkey = key;
            entry.depth = depth;
            entry.value = value;
            entry.flag = flag;
        }
    }

    bool TranspositionTable::lookup(uint64_t key, int depth, int alpha, int beta, int& outValue) {
        const TTEntry& entry = table[key % table.size()];
        
        // If the key matches and we searched at least as deep as we want to now
        if (entry.zkey == key && entry.depth >= depth) {
            if (entry.flag == HashFlag::EXACT) {
                outValue = entry.value;
                return true;
            }
            if (entry.flag == HashFlag::ALPHA && entry.value <= alpha) {
                outValue = alpha;
                return true;
            }
            if (entry.flag == HashFlag::BETA && entry.value >= beta) {
                outValue = beta;
                return true;
            }
        }
        return false;
    }

    void TranspositionTable::clear() {
        table.assign(table.size(), TTEntry{});
    }
