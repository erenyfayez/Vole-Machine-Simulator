// ============================================================
//  ProgramLoader.cpp : implementation of the ProgramLoader class
// ============================================================
#include "ProgramLoader.h"

#include <cctype>
#include <fstream>

// Line must be all hex digits and its length divisible by 4
bool ProgramLoader::isValidHexLine(const std::string& line) const {
    if (line.empty() || line.size() % 4 != 0) return false;
    for (char c : line) {
        if (!std::isxdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

std::string ProgramLoader::getLastError() const { return lastError; }

bool ProgramLoader::loadFromFile(const std::string& filename, uint8_t startAddress,
                                 Memory& memory) {
    lastError = "";
    std::ifstream file(filename);
    if (!file.is_open()) {
        lastError = "Cannot open file: " + filename;
        return false;
    }

    int address = startAddress;  // int so we can detect going past 255
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;

        // 1) remove comment (anything after ;)
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) line = line.substr(0, commentPos);

        // 2) remove spaces, tabs, \r, \n
        std::string clean;
        for (char c : line) {
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n') clean += c;
        }

        // 3) skip empty line
        if (clean.empty()) continue;

        // 4) check format
        if (!isValidHexLine(clean)) {
            lastError = "Invalid hex data at line " + std::to_string(lineNumber);
            return false;
        }

        // 5) every 4 chars = one instruction = 2 bytes in memory
        for (size_t i = 0; i < clean.size(); i += 4) {
            if (address + 1 > 255) {
                lastError = "Program does not fit in memory (line " +
                            std::to_string(lineNumber) + ")";
                return false;
            }
            unsigned word = std::stoul(clean.substr(i, 4), nullptr, 16);
            memory.write(static_cast<uint8_t>(address), (word >> 8) & 0xFF);
            memory.write(static_cast<uint8_t>(address + 1), word & 0xFF);
            address += 2;
        }
    }
    return true;
}
