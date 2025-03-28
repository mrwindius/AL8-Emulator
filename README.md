# AL1 Emulator and Assembler

A simple emulator for the AL1 processor and accompanying assembler, written in C++. This project allows you to write assembly programs, compile them to binary, and execute them on the emulated AL1 CPU.

## Features

- Emulates a 8-bit CPU with 256 bytes of memory
- Supports basic instructions (LDA, STA, ADD, SUB, JMP, HLT)
- Simple assembler to convert assembly code to binary format
- Memory manipulation and execution tracing

## Build Instructions

1. Ensure you have a C++ compiler installed (e.g., g++)
2. Compile the source code:
   ```bash
   g++ -o al1_emulator main.cpp
   ```

## Usage

The emulator has two modes of operation:

### Compile Assembly to Binary
```bash
./al1_emulator make program.asm
```
Outputs: `program.bin`

### Run Binary Program
```bash
./al1_emulator run program.bin
```

## Example Program

Create `add.asm`:
```asm
LDA 10    ; Load value from address 10 (5)
ADD 11    ; Add value from address 11 (3)
STA 12    ; Store result in address 12
HLT       ; Halt
```

Compile and run:
```bash
./al1_emulator make add.asm
./al1_emulator run add.bin
```

The result of 5 + 3 will be stored in memory address 12.

## Instruction Set

| Mnemonic | Opcode | Description                          |
|----------|--------|--------------------------------------|
| HLT      | 0x00   | Stop execution                       |
| LDA      | 0x01   | Load accumulator from memory address |
| STA      | 0x02   | Store accumulator to memory address  |
| ADD      | 0x03   | Add memory value to accumulator      |
| SUB      | 0x04   | Subtract memory value from accumulator |
| JMP      | 0x05   | Jump to memory address               |

## Memory Architecture

- 256 bytes of total memory
- 8-bit addresses (0x00-0xFF)
- Memory-mapped I/O (not implemented in this version)

## CPU Registers

- Accumulator (A): 8-bit general purpose register
- Program Counter (PC): 8-bit instruction pointer
- Instruction Register (IR): Stores current instruction

## Limitations

- Assembler doesn't support labels - addresses must be numeric
- No overflow detection for arithmetic operations
- Memory values initialized to 0 by default (set manually in code)
- Limited error checking in assembler

## License

[MIT License](https://raw.githubusercontent.com/mrwindius/AL8-Emulator/refs/heads/main/LICENSE)
