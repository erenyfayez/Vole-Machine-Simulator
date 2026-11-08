// ============================================================
//  Memory.cpp : implementation of the Memory class
// ============================================================
#include "Memory.h"

Memory::Memory() { clear(); }

// uint8_t address is always 0..255, so no out-of-range access
uint8_t Memory::read(uint8_t address) const { return cells[address]; }

void Memory::write(uint8_t address, uint8_t value) { cells[address] = value; }

void Memory::clear() {
    for (int i = 0; i < 256; i++) cells[i] = 0;
}
