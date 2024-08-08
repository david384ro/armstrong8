#include <iostream>
#include <cstdint>
#include <cstring>

#include "ppu/ppu_main.hpp"

#ifndef CPU_MAIN_H
#define CPU_MAIN_H

struct CPU {
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t SP;
    uint16_t PC;
    uint8_t flags;

    uint8_t ram[255]; // 8-bit ZERO PAGE
    uint8_t xram[512]; // 8-bit NEXT PAGE
    uint8_t rom[32767]; // 8-bit ROM memory, will be changed later
    bool halted;

    PPU ppu; // the ppu has 1024 bytes of vram
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