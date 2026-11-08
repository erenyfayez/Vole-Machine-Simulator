// ============================================================
//  Memory.h : declaration of the Memory class
// ============================================================
#pragma once

#include <cstdint>

// Memory : 256 cells, each 8 bits
class Memory {
private:
    uint8_t cells[256];

public:
    Memory();
    uint8_t read(uint8_t address) const;
    void write(uint8_t address, uint8_t value);
    void clear();
};
