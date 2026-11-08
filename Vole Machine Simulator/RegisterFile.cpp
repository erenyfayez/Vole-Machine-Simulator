// ============================================================
//  RegisterFile.cpp : implementation of the RegisterFile class
// ============================================================
#include "RegisterFile.h"

RegisterFile::RegisterFile() { clear(); }

// & 0xF guarantees the register number is between 0 and 15
uint8_t RegisterFile::read(uint8_t regNum) const { return registers[regNum & 0xF]; }

void RegisterFile::write(uint8_t regNum, uint8_t value) { registers[regNum & 0xF] = value; }

void RegisterFile::clear() {
    for (int i = 0; i < 16; i++) registers[i] = 0;
}
