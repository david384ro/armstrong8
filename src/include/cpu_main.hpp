#include <iostream>
#include <cstdint>
#include <cstring>

#include "ppu/ppu_main.hpp"

#ifndef CPU_MAIN_H
#define CPU_MAIN_H

struct CPU
{
    uint8_t A;     // accumulator
    uint8_t X;     // increment register 1
    uint8_t Y;     // increment register 2
    uint8_t i3;    // increment register 3
    uint8_t t1;    // TMP register 1, you can transfer data to and from it
    uint8_t t2;    // TMP register 2, you can transfer data to and from it
    uint16_t r4;   // TMP register 3, use it to transfer from the increment register on the PPU
    uint8_t SP;    // stack pointer (no implementation currently)
    uint16_t PC;   // program counter
    uint8_t flags; // flag register
    // max registers: 16, current used: 10

    uint8_t ram[255];   // 8-bit ZERO PAGE
    uint8_t xram[512];  // 8-bit NEXT PAGE/extended
    uint8_t rom[32767]; // 8-bit ROM memory, will be changed later
    bool halted;

    PPU ppu; // the ppu has 16kb of vram, 128x128 screen
};

#define FLAG_CARRY 0x01
#define FLAG_ZERO 0x02
#define FLAG_INTERRUPT 0x04
#define FLAG_DECIMAL 0x08
#define FLAG_BREAK 0x10
#define FLAG_UNUSED 0x20
#define FLAG_OVERFLOW 0x40
#define FLAG_NEGATIVE 0x80

#endif // CPU_MAIN_H