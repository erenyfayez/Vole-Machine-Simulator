# Vole Machine Simulator

An object-oriented simulator of the Vole machine, written in C++.

## Features
- 256 bytes of memory and 16 registers (8-bit each)
- Fetch–decode–execute cycle for 12 instructions
- 8-bit floating-point addition
- Loads programs from hex text files (`;` starts a comment)
- Step-by-step or full-run execution, with a machine state display
- Error handling for invalid instructions and invalid program files

## Project Structure
| File | Purpose |
|------|---------|
| `Memory` | 256-cell memory |
| `RegisterFile` | 16 registers (R0–RF) |
| `Instruction` | Splits and validates a 16-bit instruction |
| `CPU` | Fetch, decode and execute |
| `ProgramLoader` | Reads a hex program file into memory |
| `VoleMachine` | Connects all components |
| `main.cpp` | Menu |

## How to Run
```bash
g++ -std=c++17 *.cpp -o vole
./vole
```
Choose **1) Load program**, enter `example_program.txt` and start address `00`, then choose **2) Run**.

Expected output: `[SCREEN] 0x41 ('A')` and the machine state `HALTED`.
