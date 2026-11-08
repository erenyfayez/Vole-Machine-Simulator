// ============================================================
//  ProgramLoader.h : declaration of the ProgramLoader class
//  File format: each instruction = 4 hex digits (e.g. 2005)
//  Ignores empty lines and anything after ';' (comments)
// ============================================================
#pragma once

#include <cstdint>
#include <string>

#include "Memory.h"

class ProgramLoader {
private:
    std::string lastError;

    bool isValidHexLine(const std::string& line) const;

public:
    std::string getLastError() const;
    bool loadFromFile(const std::string& filename, uint8_t startAddress, Memory& memory);
};
