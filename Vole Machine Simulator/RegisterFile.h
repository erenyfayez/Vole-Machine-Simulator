// ============================================================
//  RegisterFile.h : declaration of the RegisterFile class
// ============================================================
#pragma once

#include <cstdint>

// RegisterFile : 16 registers (R0..RF), each 8 bits
class RegisterFile {
private:
    uint8_t registers[16];

public:
    RegisterFile();
    uint8_t read(uint8_t regNum) const;
    void write(uint8_t regNum, uint8_t value);
    void clear();
};
