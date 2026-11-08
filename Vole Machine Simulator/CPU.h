
#pragma once

#include <cstdint>
#include <string>
#include "Instruction.h"
#include "Memory.h"
#include "RegisterFile.h"

enum class MachineState {
    READY,   
    RUNNING,  
    HALTED,   
    ERROR
};

std::string stateToString(MachineState s);


std::string toHex(unsigned value, int width);

class CPU {
private:
    uint8_t pc;                
    uint16_t ir;                  
    MachineState state;
    std::string lastErrorMessage;

    void setError(const std::string& msg);

    static double decodeFloat(uint8_t v);
    static uint8_t encodeFloat(double x);

public:
    CPU();

    void reset(uint8_t startAddress = 0);
    void fetch(Memory& memory);
    void decodeAndExecute(Memory& memory, RegisterFile& regFile);
    void step(Memory& memory, RegisterFile& regFile);

    uint8_t getPC() const;
    uint16_t getIR() const;
    MachineState getState() const;
    std::string getErrorMessage() const;
};
//
