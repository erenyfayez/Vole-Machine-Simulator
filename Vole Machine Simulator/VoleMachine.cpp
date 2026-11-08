// ============================================================
//  VoleMachine.cpp : implementation of the VoleMachine class
// ============================================================
#include "VoleMachine.h"

#include <iostream>

// sync machine state with CPU state
void VoleMachine::syncWithCPU() {
    state = cpu.getState();
    if (state == MachineState::ERROR) errorMessage = cpu.getErrorMessage();
}

VoleMachine::VoleMachine() { reset(); }

bool VoleMachine::loadProgram(const std::string& filename, uint8_t startAddress) {
    reset();
    if (!loader.loadFromFile(filename, startAddress, memory)) {
        state = MachineState::ERROR;
        errorMessage = loader.getLastError();
        return false;
    }
    cpu.reset(startAddress);
    state = MachineState::READY;
    return true;
}

// run until HALT or error
void VoleMachine::run() {
    const int MAX_STEPS = 100000;  // protection from infinite loops
    int steps = 0;
    while (state == MachineState::READY || state == MachineState::RUNNING) {
        step();
        if (++steps >= MAX_STEPS) {
            state = MachineState::ERROR;
            errorMessage = "Step limit reached (possible infinite loop)";
        }
    }
}

void VoleMachine::step() {
    if (state == MachineState::HALTED || state == MachineState::ERROR) return;
    cpu.step(memory, regFile);
    syncWithCPU();
}

void VoleMachine::reset() {
    cpu.reset();
    memory.clear();
    regFile.clear();
    state = MachineState::READY;
    errorMessage = "";
}

void VoleMachine::displayState() const {
    using std::cout;
    using std::endl;

    cout << "\n===== Machine State =====" << endl;
    cout << "State: " << stateToString(state) << endl;
    if (state == MachineState::ERROR) cout << "Error: " << errorMessage << endl;
    cout << "PC: 0x" << toHex(cpu.getPC(), 2) << "   IR: 0x" << toHex(cpu.getIR(), 4) << endl;

    cout << "\nRegisters:" << endl;
    for (int i = 0; i < 16; i++) {
        cout << "R" << toHex(i, 1) << "=" << toHex(regFile.read(i), 2) << "  ";
        if (i % 8 == 7) cout << endl;
    }

    cout << "\nMemory (hex):" << endl;
    cout << "    ";
    for (int c = 0; c < 16; c++) cout << toHex(c, 2) << " ";
    cout << endl;
    for (int r = 0; r < 16; r++) {
        cout << toHex(r * 16, 2) << ": ";
        for (int c = 0; c < 16; c++) cout << toHex(memory.read(r * 16 + c), 2) << " ";
        cout << endl;
    }
    cout << "=========================\n" << endl;
}

MachineState VoleMachine::getState() const { return state; }
std::string VoleMachine::getErrorMessage() const { return errorMessage; }
