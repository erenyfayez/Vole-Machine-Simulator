// ============================================================
//  Instruction.h : declaration of the Instruction class
//  Format: [opcode 4 bits][R 4 bits][S 4 bits][T 4 bits]
//  XY = last 8 bits (S and T together)
// ============================================================
#pragma once

#include <cstdint>

class Instruction {
private:
    uint16_t rawInstruction;
    uint8_t opcode;
    uint8_t regR;
    uint8_t regS;
    uint8_t regT;
    uint8_t addressXY;
    uint8_t immediateValue;

public:
    Instruction(uint16_t raw);

    bool isValid() const;

    uint8_t getOpcode() const;
    uint8_t getR() const;
    uint8_t getS() const;
    uint8_t getT() const;
    uint8_t getAddressXY() const;
    uint8_t getImmediateValue() const;
    uint16_t getRaw() const;
};
