#include <iostream>
#include <cstdint>
#include <cstring>

#ifndef CPU_MAIN_H
#define CPU_MAIN_H

struct CPU {
    uint8_t A;      // Accumulator
    uint8_t X;      // X Register
    uint8_t Y;      // Y Register
    uint8_t SP;     // Stack Pointer
    uint16_t PC;     // Program Counter
    uint8_t flags;  // Processor Status

    uint8_t ram[255]; // 8-bit RAM memory, will be changed later
    uint8_t xram[511]; // 8-bit EXTENDED RAM memory, will be changed later
    uint8_t rom[32767]; // 8-bit ROM memory, will be changed later
    bool halted;
};

#define FLAG_CARRY     0x01
#define FLAG_ZERO      0x02
#define FLAG_INTERRUPT 0x04
#define FLAG_DECIMAL   0x08
#define FLAG_BREAK     0x10
#define FLAG_UNUSED    0x20
#define FLAG_OVERFLOW  0x40
#define FLAG_NEGATIVE  0x80

#endif // CPU_MAIN_H