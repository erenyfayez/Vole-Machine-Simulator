// ============================================================
//  CPU.cpp : implementation of the CPU class + helpers
// ============================================================
#include "CPU.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

// ---------- helper functions ----------
std::string stateToString(MachineState s) {
    switch (s) {
        case MachineState::READY:   return "READY";
        case MachineState::RUNNING: return "RUNNING";
        case MachineState::HALTED:  return "HALTED";
        case MachineState::ERROR:   return "ERROR";
    }
    return "UNKNOWN";
}

std::string toHex(unsigned value, int width) {
    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setw(width) << std::setfill('0') << value;
    return ss.str();
}

// ---------- private ----------
void CPU::setError(const std::string& msg) {
    state = MachineState::ERROR;
    lastErrorMessage = msg;
}

double CPU::decodeFloat(uint8_t v) {
    if (v == 0) return 0.0;
    int sign = (v & 0x80) ? -1 : 1;
    int exponent = ((v >> 4) & 0x7) - 4;
    double mantissa = (v & 0xF) / 16.0;
    return sign * mantissa * std::pow(2.0, exponent);
}

uint8_t CPU::encodeFloat(double x) {
    if (x == 0.0) return 0;
    uint8_t signBit = (x < 0) ? 0x80 : 0;
    double ax = std::fabs(x);
    int e;
    double m = std::frexp(ax, &e);  // ax = m * 2^e, 0.5 <= m < 1
    int expField = e + 4;
    int mantField = static_cast<int>(std::round(m * 16));
    if (mantField == 16) { mantField = 8; expField++; }  // rounding fix
    if (expField < 0) return 0;                          // too small -> zero
    if (expField > 7) { expField = 7; mantField = 15; }  // too big -> max value
    return signBit | (expField << 4) | mantField;
}

// ---------- public ----------
CPU::CPU() { reset(); }

void CPU::reset(uint8_t startAddress) {
    pc = startAddress;
    ir = 0;
    state = MachineState::READY;
    lastErrorMessage = "";
}

// Fetch: read 2 bytes, build 16-bit instruction, PC += 2
void CPU::fetch(Memory& memory) {
    uint8_t high = memory.read(pc);
    uint8_t low = memory.read(static_cast<uint8_t>(pc + 1));
    ir = (static_cast<uint16_t>(high) << 8) | low;
    pc = static_cast<uint8_t>(pc + 2);
}

// Decode + Execute
void CPU::decodeAndExecute(Memory& memory, RegisterFile& regFile) {
    Instruction inst(ir);

    if (!inst.isValid()) {
        setError("Invalid instruction 0x" + toHex(ir, 4) + " at address 0x" +
                 toHex(static_cast<uint8_t>(pc - 2), 2));
        return;
    }

    uint8_t R = inst.getR();
    uint8_t S = inst.getS();
    uint8_t T = inst.getT();
    uint8_t XY = inst.getAddressXY();

    switch (inst.getOpcode()) {
        case 0x1:  // 1RXY : load R from memory[XY]
            regFile.write(R, memory.read(XY));
            break;

        case 0x2:  // 2RXY : load R with immediate XY
            regFile.write(R, inst.getImmediateValue());
            break;

        case 0x3:  // 3RXY : store R into memory[XY]  (XY == 00 means screen)
            if (XY == 0xFF) {
                uint8_t v = regFile.read(R);
                std::cout << "[SCREEN] 0x" << toHex(v, 2);
                if (v >= 32 && v < 127) std::cout << " ('" << static_cast<char>(v) << "')";
                std::cout << std::endl;
            } else {
                memory.write(XY, regFile.read(R));
            }
            break;

        case 0x4:  // 40RS : copy R to S (digits: 4 0 R S)
            regFile.write(T, regFile.read(S));
            break;

        case 0x5:  // 5RST : R = S + T (two's complement)
            regFile.write(R, static_cast<uint8_t>(regFile.read(S) + regFile.read(T)));
            break;

        case 0x6: {  // 6RST : R = S + T (floating point)
            double a = decodeFloat(regFile.read(S));
            double b = decodeFloat(regFile.read(T));
            regFile.write(R, encodeFloat(a + b));
            break;
        }

        case 0x7:  // 7RST : R = S OR T
            regFile.write(R, regFile.read(S) | regFile.read(T));
            break;

        case 0x8:  // 8RST : R = S AND T
            regFile.write(R, regFile.read(S) & regFile.read(T));
            break;

        case 0x9:  // 9RST : R = S XOR T
            regFile.write(R, regFile.read(S) ^ regFile.read(T));
            break;

        case 0xA: {  // AR0X : rotate R right X times
            uint8_t v = regFile.read(R);
            int times = T % 8;
            if (times != 0) v = static_cast<uint8_t>((v >> times) | (v << (8 - times)));
            regFile.write(R, v);
            break;
        }

        case 0xB:  // BRXY : if R == R0 jump to XY
            if (regFile.read(R) == regFile.read(0)) pc = XY;
            break;

        case 0xC:  // C000 : HALT
            state = MachineState::HALTED;
            break;
    }
}

void CPU::step(Memory& memory, RegisterFile& regFile) {
    if (state == MachineState::HALTED || state == MachineState::ERROR) return;
    state = MachineState::RUNNING;
    fetch(memory);
    decodeAndExecute(memory, regFile);
}

uint8_t CPU::getPC() const { return pc; }
uint16_t CPU::getIR() const { return ir; }
MachineState CPU::getState() const { return state; }
std::string CPU::getErrorMessage() const { return lastErrorMessage; }