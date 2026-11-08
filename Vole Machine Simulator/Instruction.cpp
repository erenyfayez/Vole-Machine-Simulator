// ============================================================
//  Instruction.cpp : implementation of the Instruction class
// ============================================================
#include "Instruction.h"

// Split the raw 16-bit word using shift and mask
Instruction::Instruction(uint16_t raw) {
    rawInstruction = raw;
    opcode         = (raw >> 12) & 0xF;
    regR           = (raw >> 8) & 0xF;
    regS           = (raw >> 4) & 0xF;
    regT           = raw & 0xF;
    addressXY      = raw & 0xFF;
    immediateValue = raw & 0xFF;
}

// Valid opcode + the fields that must be zero are really zero
bool Instruction::isValid() const {
    if (opcode < 0x1 || opcode > 0xC) return false;
    if (opcode == 0x4 && regR != 0) return false;                 // 40RS
    if (opcode == 0xA && regS != 0) return false;                 // AR0X
    if (opcode == 0xC && rawInstruction != 0xC000) return false;  // HALT
    return true;
}

uint8_t Instruction::getOpcode() const { return opcode; }
uint8_t Instruction::getR() const { return regR; }
uint8_t Instruction::getS() const { return regS; }
uint8_t Instruction::getT() const { return regT; }
uint8_t Instruction::getAddressXY() const { return addressXY; }
uint8_t Instruction::getImmediateValue() const { return immediateValue; }
uint16_t Instruction::getRaw() const { return rawInstruction; }
