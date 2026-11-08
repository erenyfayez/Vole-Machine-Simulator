// ============================================================
//  VoleMachine.h : declaration of the VoleMachine class
//  The "conductor" that connects all the components
// ============================================================
#pragma once

#include <cstdint>
#include <string>

#include "CPU.h"
#include "Memory.h"
#include "ProgramLoader.h"
#include "RegisterFile.h"

class VoleMachine {
private:
    CPU cpu;
    Memory memory;
    RegisterFile regFile;
    ProgramLoader loader;
    MachineState state;
    std::string errorMessage;

    void syncWithCPU();

public:
    VoleMachine();

    bool loadProgram(const std::string& filename, uint8_t startAddress);
    void run();
    void step();
    void reset();
    void displayState() const;

    MachineState getState() const;
    std::string getErrorMessage() const;
};
