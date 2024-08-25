#include "cpu_main.hpp"
#include "cpu_instructions.hpp"
#include "cpu_setflag.hpp"
#include <sstream>
#include <iomanip>
#include <vector>
#include <chrono>

// tip: dont be stupid and look for the simplest way.
void execute_instruction(CPU *cpu, SDL_Window *window)
{
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    uint8_t opcode = cpu->rom[cpu->PC];
    cpu->PC++;
    // std::cout << "OP:" << static_cast<int>(opcode) << " PC:" << static_cast<int>(cpu->PC) << std::endl;
    switch (opcode)
    {
    case LDA_IMMEDIATE:
    {
        uint8_t value = cpu->rom[cpu->PC];
        cpu->PC++;
        cpu->A = value;
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        break;
    }
    case LDA_ZERO_PAGE:
    {
        uint8_t addr = cpu->rom[cpu->PC];
        cpu->PC++;
        cpu->A = cpu->ram[addr];
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        break;
    }
    case STA_ZERO_PAGE:
    {
        uint8_t addr = cpu->rom[cpu->PC];
        cpu->PC++;
        cpu->ram[addr] = cpu->A;
        break;
    }
    case STAX_ZERO_PAGE:
    {
        cpu->ram[cpu->X] = cpu->A;
        break;
    }
    case LDA_NEXT_PAGE:
    {
        uint8_t low = cpu->rom[cpu->PC];
        uint8_t high = cpu->rom[cpu->PC + 1];
        cpu->PC += 2;
        uint16_t addr = (high << 8) | low;
        cpu->A = cpu->xram[addr];
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        break;
    }
    case STA_NEXT_PAGE:
    {
        uint8_t low = cpu->rom[cpu->PC];
        uint8_t high = cpu->rom[cpu->PC + 1];
        cpu->PC += 2;
        uint16_t addr = (high << 8) | low;
        cpu->xram[addr] = cpu->A;
        break;
    }
    case STAX_NEXT_PAGE:
    {
        uint16_t address = cpu->X + cpu->Y;
        cpu->xram[address] = cpu->A;
        break;
    }
    // case STA_NEXT_PAGE_X: {
    //     cpu->xram[cpu->X] = cpu->A;
    //     break;
    // }
    // case STA_ZERO_PAGE_X: {
    //     cpu->ram[cpu->X] = cpu->A;
    //     break;
    // }
    case INX:
    {
        cpu->X++;
        set_flag(cpu, FLAG_ZERO, cpu->X == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->X & 0x80);
        break;
    }
    case DEX:
    {
        cpu->X--;
        set_flag(cpu, FLAG_ZERO, cpu->X == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->X & 0x80);
        break;
    }
    case INY:
    {
        cpu->Y++;
        set_flag(cpu, FLAG_ZERO, cpu->Y == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->Y & 0x80);
        break;
    }
    case DEY:
    {
        cpu->Y--;
        set_flag(cpu, FLAG_ZERO, cpu->Y == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->Y & 0x80);
        break;
    }
    case IN1:
    {
        cpu->i3++;
        set_flag(cpu, FLAG_ZERO, cpu->i3 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->i3 & 0x80);
        break;
    }
    case DE1:
    {
        cpu->i3--;
        set_flag(cpu, FLAG_ZERO, cpu->i3 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->i3 & 0x80);
        break;
    }
    case INP:
    {
        // std::cout << "P:" << static_cast<int>(cpu->ppu.I) << std::endl;
        cpu->ppu.I++;
        set_flag(cpu, FLAG_ZERO, cpu->ppu.I == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->ppu.I & 0x8000);
        break;
    }
    case DEP:
    {
        cpu->ppu.I--;
        set_flag(cpu, FLAG_ZERO, cpu->ppu.I == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->ppu.I & 0x8000);
        break;
    }

    case BRK:
    {
        std::cout << "BRK encountered. Halting CPU." << std::endl;
        cpu->halted = true;
        break;
    }
    case JNE:
    {
        uint16_t address = cpu->rom[cpu->PC] /*| (cpu->rom[cpu->PC + 1] << 8)*/;
        // std::cout << address << std::endl;
        cpu->PC++;
        if (!get_flag(cpu, FLAG_ZERO))
        {
            cpu->PC = address;
        }
        break;
    }
    case JEQ:
    {
        uint16_t address = cpu->rom[cpu->PC] /*| (cpu->rom[cpu->PC + 1] << 8)*/;
        cpu->PC++;
        if (get_flag(cpu, FLAG_ZERO))
        {
            cpu->PC = address;
        }
        break;
    }
    case JPL:
    {
        uint16_t address = cpu->rom[cpu->PC] /*| (cpu->rom[cpu->PC + 1] << 8)*/;
        cpu->PC++;
        if (!get_flag(cpu, FLAG_NEGATIVE))
        {
            cpu->PC = address;
        }
        break;
    }
    case JMI:
    {
        uint16_t address = cpu->rom[cpu->PC] /*| (cpu->rom[cpu->PC + 1] << 8)*/;
        cpu->PC++;
        if (get_flag(cpu, FLAG_NEGATIVE))
        {
            cpu->PC = address;
        }
        break;
    }
    case JMP:
    {
        uint16_t addr = cpu->rom[cpu->PC] /*| (cpu->rom[cpu->PC + 1] << 8)*/;
        cpu->PC = addr;
        break;
    }
    case ADD:
    {
        uint8_t value = cpu->rom[cpu->PC];
        cpu->PC++;
        uint16_t result = cpu->A + value;
        cpu->A = result & 0xFF; // 8-bit result
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        set_flag(cpu, FLAG_CARRY, result > 0xFF);
        break;
    }
    case SUB:
    {
        uint8_t value = cpu->rom[cpu->PC];
        cpu->PC++;
        uint16_t result = cpu->A - value;
        cpu->A = result & 0xFF; // 8-bit result
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        set_flag(cpu, FLAG_CARRY, result > 0xFF);
        break;
    }
    case AND:
    {
        uint8_t value = cpu->rom[cpu->PC];
        cpu->PC++;
        cpu->A = cpu->A & value;
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        set_flag(cpu, FLAG_CARRY, false);
        break;
    }
    case OR:
    {
        uint8_t value = cpu->rom[cpu->PC];
        cpu->PC++;
        cpu->A = cpu->A | value;
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        set_flag(cpu, FLAG_CARRY, false);
        break;
    }
    case XOR:
    {
        uint8_t value = cpu->rom[cpu->PC];
        cpu->PC++;
        cpu->A = cpu->A ^ value;
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        set_flag(cpu, FLAG_CARRY, false);
        break;
    }
    case NOT:
    {
        cpu->A = ~cpu->A;
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        set_flag(cpu, FLAG_CARRY, false);
        break;
    }
    case SHL:
    {
        uint8_t value = cpu->rom[cpu->PC];
        cpu->PC++;
        uint16_t result = cpu->A << value;
        cpu->A = result & 0xFF; // 8-bit result
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        set_flag(cpu, FLAG_CARRY, (result & 0x100) != 0); // Check if bit 8 is set
        break;
    }
    case SHR:
    {
        uint8_t value = cpu->rom[cpu->PC];
        cpu->PC++;
        uint16_t result = cpu->A >> value;
        cpu->A = result & 0xFF; // 8-bit result
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        set_flag(cpu, FLAG_CARRY, (cpu->A & 0x01) != 0); // Check if the least significant bit is set
        break;
    }
    case MUL:
    {
        uint8_t value = cpu->rom[cpu->PC];
        cpu->PC++;
        uint16_t result = cpu->A * value;
        cpu->A = result & 0xFF; // 8-bit result
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        set_flag(cpu, FLAG_CARRY, result > 0xFF);
        break;
    }
    case CMP:
    {
        uint8_t value = cpu->rom[cpu->PC];
        cpu->PC++;
        uint16_t result = cpu->A - value;
        set_flag(cpu, FLAG_ZERO, result == 0);
        set_flag(cpu, FLAG_NEGATIVE, result & 0x80);
        set_flag(cpu, FLAG_CARRY, cpu->A >= value);
        break;
    }
    case CMX:
    {
        // std::cout << "x:" << static_cast<int>(cpu->X) << std::endl;
        uint8_t value = cpu->rom[cpu->PC];
        cpu->PC++;
        uint16_t result = cpu->X - value;
        set_flag(cpu, FLAG_ZERO, result == 0);
        set_flag(cpu, FLAG_NEGATIVE, result & 0x80);
        set_flag(cpu, FLAG_CARRY, cpu->X >= value);
        break;
    }
    case CMY:
    {
        // std::cout << "y:" << static_cast<int>(cpu->Y) << std::endl;
        uint8_t value = cpu->rom[cpu->PC];
        cpu->PC++;
        uint16_t result = cpu->Y - value;
        set_flag(cpu, FLAG_ZERO, result == 0);
        set_flag(cpu, FLAG_NEGATIVE, result & 0x80);
        set_flag(cpu, FLAG_CARRY, cpu->Y >= value);
        break;
    }
    case CMPHL:
    {
        uint8_t low = cpu->rom[cpu->PC];
        uint8_t high = cpu->rom[cpu->PC + 1];
        uint16_t value = (high << 8) | low;
        uint16_t result = cpu->ppu.I - value;
        cpu->PC += 2;
        set_flag(cpu, FLAG_ZERO, result == 0);
        set_flag(cpu, FLAG_NEGATIVE, (result & 0x8000) != 0);
        set_flag(cpu, FLAG_CARRY, cpu->ppu.I >= value);
        break;
    }
    case TAX:
    {
        cpu->X = cpu->A;
        set_flag(cpu, FLAG_ZERO, cpu->X == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->X & 0x80);
        break;
    }
    case TAY:
    {
        cpu->Y = cpu->A;
        set_flag(cpu, FLAG_ZERO, cpu->Y == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->Y & 0x80);
        break;
    }
    case TXA:
    {
        cpu->A = cpu->X;
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        break;
    }
    case TYA:
    {
        cpu->A = cpu->Y;
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        break;
    }
    case TYX:
    {
        cpu->X = cpu->Y;
        set_flag(cpu, FLAG_ZERO, cpu->X == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->X & 0x80);
        break;
    }
    case TXY:
    {
        cpu->Y = cpu->X;
        set_flag(cpu, FLAG_ZERO, cpu->Y == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->Y & 0x80);
        break;
    }
    case TIA:
    {
        cpu->i3 = cpu->A;
        set_flag(cpu, FLAG_ZERO, cpu->i3 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->i3 & 0x80);
        break;
    }
    case TAI:
    {
        cpu->A = cpu->i3;
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        break;
    }
    case TIX:
    {
        cpu->i3 = cpu->X;
        set_flag(cpu, FLAG_ZERO, cpu->i3 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->i3 & 0x80);
        break;
    }
    case TIY:
    {
        cpu->X = cpu->i3;
        set_flag(cpu, FLAG_ZERO, cpu->X == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->X & 0x80);
        break;
    }
    case TYI:
    {
        cpu->Y = cpu->i3;
        set_flag(cpu, FLAG_ZERO, cpu->Y == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->Y & 0x80);
        break;
    }
    case TXI:
    {
        cpu->i3 = cpu->X;
        set_flag(cpu, FLAG_ZERO, cpu->i3 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->i3 & 0x80);
        break;
    }
    case TAT1:
    {
        cpu->t1 = cpu->A;
        set_flag(cpu, FLAG_ZERO, cpu->t1 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->t1 & 0x80);
        break;
    }
    case TT1A:
    {
        cpu->A = cpu->t1;
        set_flag(cpu, FLAG_ZERO, cpu->A == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
        break;
    }
    case TXT1:
    {
        cpu->t1 = cpu->X;
        set_flag(cpu, FLAG_ZERO, cpu->t1 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->t1 & 0x80);
        break;
    }
    case TT1X:
    {
        cpu->X = cpu->t1;
        set_flag(cpu, FLAG_ZERO, cpu->X == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->X & 0x80);
        break;
    }
    case TYT1:
    {
        cpu->t1 = cpu->Y;
        set_flag(cpu, FLAG_ZERO, cpu->t1 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->t1 & 0x80);
        break;
    }
    case TT1Y:
    {
        cpu->Y = cpu->t1;
        set_flag(cpu, FLAG_ZERO, cpu->Y == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->Y & 0x80);
        break;
    }
    case TIT1:
    {
        cpu->t1 = cpu->i3;
        set_flag(cpu, FLAG_ZERO, cpu->t1 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->t1 & 0x80);
        break;
    }
    case TT1I:
    {
        cpu->i3 = cpu->t1;
        set_flag(cpu, FLAG_ZERO, cpu->i3 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->i3 & 0x80);
        break;
    }
    case TT1T2:
    {
        cpu->t2 = cpu->t1;
        set_flag(cpu, FLAG_ZERO, cpu->t2 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->t2 & 0x80);
        break;
    }
    case TT2T1:
    {
        cpu->t1 = cpu->t2;
        set_flag(cpu, FLAG_ZERO, cpu->t1 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->t1 & 0x80);
        break;
    }

    case TPT3:
    {
        cpu->r4 = cpu->ppu.I;
        set_flag(cpu, FLAG_ZERO, cpu->r4 == 0);
        set_flag(cpu, FLAG_NEGATIVE, (cpu->r4 & 0x8000) != 0);
        break;
    }
    case TT3P:
    {
        cpu->ppu.I = cpu->r4;
        set_flag(cpu, FLAG_ZERO, cpu->ppu.I == 0);
        set_flag(cpu, FLAG_NEGATIVE, (cpu->ppu.I & 0x8000) != 0);
        break;
    }

    case STA_VRAM:
    {
        uint8_t low = cpu->rom[cpu->PC];
        uint8_t high = cpu->rom[cpu->PC + 1];
        cpu->PC += 2;
        uint16_t addr = (high << 8) | low;
        cpu->ppu.write(addr, cpu->A);
        break;
    }
    case LDIO:
    {
        uint8_t low = cpu->rom[cpu->PC];
        uint8_t high = cpu->rom[cpu->PC + 1];
        cpu->PC += 2;
        uint16_t addr = (high << 8) | low;
        cpu->io = cpu->input[addr];
        set_flag(cpu, FLAG_ZERO, cpu->io == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->io & 0x80);
        break;
    }
    case LDIOP:
    {
        cpu->io = cpu->input[cpu->ppu.I];
        break;
    }
    case STIO_VRAM:
    {
        uint8_t low = cpu->rom[cpu->PC];
        uint8_t high = cpu->rom[cpu->PC + 1];
        cpu->PC += 2;
        uint16_t addr = (high << 8) | low;
        cpu->ppu.write(addr, cpu->io);
        break;
    }
    case STIOX_VRAM:
    {
        uint16_t address = cpu->X + cpu->Y;
        cpu->ppu.write(address, cpu->io);
        break;
    }
    case STAX_VRAM:
    {
        uint16_t address = cpu->X + cpu->Y;
        cpu->ppu.write(address, cpu->A);
        break;
    }
    case LDA_VRAM:
    {
        uint8_t low = cpu->rom[cpu->PC];
        uint8_t high = cpu->rom[cpu->PC + 1];
        cpu->PC += 2;
        uint16_t addr = (high << 8) | low;
        cpu->A = cpu->ppu.read(addr);
        break;
    }
    case STAP_VRAM:
    {
        cpu->ppu.write(cpu->ppu.I, cpu->A);
        break;
    }
    case STIOP_VRAM:
    {
        cpu->ppu.write(cpu->ppu.I, cpu->io);
        break;
    }
    case REFRESH:
    {
        int H = 144;
        int W = 172;

        const uint8_t hexcolors[] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 40, 0, 0, 60, 0, 0, 80, 0, 0, 100, 0, 0, 120, 0, 0, 140, 0, 0, 160, 0, 0, 180, 0, 0, 200, 0, 0, 220, 0, 0, 240, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 0, 0, 20, 0, 0, 40, 0, 0, 60, 0, 0, 80, 0, 0, 100, 0, 0, 120, 0, 0, 140, 0, 0, 160, 0, 0, 180, 0, 0, 200, 0, 0, 220, 0, 0, 240, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 0, 0, 0, 20, 20, 0, 40, 40, 0, 60, 60, 0, 80, 80, 0, 100, 100, 0, 120, 120, 0, 140, 140, 0, 160, 160, 0, 180, 180, 0, 200, 200, 0, 220, 220, 0, 240, 240, 0, 255, 255, 0, 255, 255, 0, 255, 255, 0, 0,
            0, 20, 0, 0, 40, 0, 0, 60, 0, 0, 80, 0, 0, 100, 0, 0, 120, 0, 0, 140, 0, 0, 160, 0, 0, 180, 0, 0, 200, 0, 0, 220, 0, 0, 240, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 0, 0, 0, 20, 0, 20, 40, 0, 40, 60, 0, 60, 80, 0, 80, 100, 0, 100, 120, 0, 120, 140, 0, 140, 160, 0, 160, 180, 0, 180, 200, 0, 200, 220, 0, 220, 240, 0, 240, 255, 0, 255, 255, 0, 255, 255, 0, 255, 0, 0, 0, 20, 10, 0, 40, 20, 0, 60, 30, 0, 80, 40, 0, 100, 50, 0, 120, 60, 0, 140, 70, 0, 160, 80,
            0, 180, 90, 0, 200, 100, 0, 220, 110, 0, 240, 120, 0, 255, 130, 0, 255, 140, 0, 255, 150, 0, 0, 0, 0, 20, 20, 20, 40, 40, 40, 60, 60, 60, 80, 80, 80, 100, 100, 100, 120, 120, 120, 140, 140, 140, 160, 160, 160, 180, 180, 180, 200, 200, 200, 220, 220, 220, 240, 240, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 10, 10, 10, 20, 20, 20, 30, 30, 30, 40, 40, 40, 50, 50, 50, 60, 60, 60, 70, 70, 70, 80, 80, 80, 90, 90, 90, 100, 100, 100, 110, 110, 110, 120, 120, 120, 130, 130, 130, 140, 140, 140, 150, 150, 150, 0, 0, 0, 10, 10, 30, 20, 20, 60, 30, 30, 90, 40, 40, 120, 50, 50, 150, 60, 60, 180, 70, 70, 210, 80, 80, 240, 90, 90, 255, 100, 100, 255, 110, 110, 255, 120, 120, 255, 130, 130, 255, 140, 140, 255, 150, 150, 255, 0, 0, 0, 10, 30, 10, 20, 60, 20, 30, 90, 30, 40, 120, 40, 50, 150, 50, 60, 180, 60, 70, 210, 70, 80, 240, 80, 90, 255, 90, 100, 255, 100, 110, 255, 110, 120, 255, 120, 130, 255, 130, 140, 255, 140, 150, 255, 150, 0, 0, 0, 10, 30, 30, 20, 60, 60, 30, 90, 90, 40, 120, 120, 50, 150, 150, 60, 180, 180, 70, 210, 210, 80, 240, 240, 90, 255, 255, 100, 255, 255, 110, 255, 255, 120, 255, 255, 130, 255, 255, 140, 255, 255, 150, 255, 255, 0, 0, 0, 30, 10, 10, 60, 20, 20, 90, 30, 30, 120, 40, 40, 150, 50, 50, 180, 60, 60, 210, 70, 70, 240, 80, 80, 255, 90, 90, 255, 100, 100, 255, 110, 110, 255, 120, 120, 255, 130, 130, 255, 140, 140, 255, 150, 150, 0, 0, 0, 30, 10, 30, 60, 20, 60, 90, 30, 90, 120, 40, 120, 150, 50, 150, 180, 60, 180, 210, 70, 210, 240, 80, 240, 255, 90, 255, 255, 100, 255, 255, 110, 255, 255, 120, 255, 255, 130, 255, 255, 140, 255, 255, 150, 255, 0, 0, 0, 30, 30, 10, 60, 60, 20, 90, 90, 30, 120, 120, 40, 150, 150, 50, 180, 180, 60, 210, 210, 70, 240, 240, 80, 255, 255, 90, 255, 255, 100, 255, 255, 110, 255, 255, 120, 255, 255, 130, 255, 255, 140, 255, 255, 150, 0, 0, 0, 30, 30, 30, 60, 60, 60, 90, 90, 90, 120, 120, 120, 150, 150, 150, 180, 180, 180, 210, 210, 210, 240, 240, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

        std::vector<uint32_t> palette(256);
        size_t num_colors = sizeof(hexcolors) / 3;
        for (size_t i = 0; i < num_colors; ++i)
        {
            uint32_t color = (hexcolors[i * 3] << 16) | (hexcolors[i * 3 + 1] << 8) | hexcolors[i * 3 + 2];
            palette[i] = (color & 0x00FFFFFF) | (255 << 24);
        }

        // for (size_t i = 0; i < palette.size(); ++i)
        //{
        //     uint32_t color = palette[i];
        //     uint8_t red = (color >> 16) & 0xFF;
        //     uint8_t green = (color >> 8) & 0xFF;
        //     uint8_t blue = color & 0xFF;
        //     uint8_t alpha = (color >> 24) & 0xFF;

        //    // Invert the color
        //    red = 255 - red;
        //    green = 255 - green;
        //    blue = 255 - blue;

        //    palette[i] = (alpha << 24) | (red << 16) | (green << 8) | blue;
        //}

        if (SDL_MUSTLOCK(surface))
        {
            SDL_LockSurface(surface);
        }

        uint32_t *dst = (uint32_t *)surface->pixels;
        uint8_t *src = cpu->ppu.vram;
        int pitch = surface->pitch / sizeof(uint32_t);

        for (int y = 0; y < H; ++y)
        {
            uint32_t *dstRow = dst + y * pitch;
            uint8_t *srcRow = src + y * W;
            for (int x = 0; x < W; ++x)
            {
                dstRow[x] = palette[srcRow[x]];
            }
        }

        if (SDL_MUSTLOCK(surface))
        {
            SDL_UnlockSurface(surface);
        }

        SDL_UpdateWindowSurface(window);
        break;
    }

    case FILL:
    {
        uint8_t value = cpu->rom[cpu->PC];
        cpu->ppu.fill_vram(value);
        cpu->PC++;
        break;
    }
    case NOP:
    {
        break;
    }
    case GPRN:
    {
        static uint64_t seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        seed = (seed ^ (seed >> 33)) * 0xBF58476D1CE4E5B9 + 0x94D049BB133111EB;
        cpu->A = seed & 0xFF;
        break;
    }
    // case CMXHL: {
    //     uint8_t low = cpu->rom[cpu->PC];
    //     uint8_t high = cpu->rom[cpu->PC + 1];
    //     cpu->PC += 2;
    //     uint16_t value = (high << 8) | low;
    //     uint16_t result = cpu->X - value;
    //     set_flag(cpu, FLAG_ZERO, (result & 0xFFFF) == 0);
    //     set_flag(cpu, FLAG_NEGATIVE, result & 0x8000);
    //     set_flag(cpu, FLAG_CARRY, cpu->X >= value);
    //     break;
    // }
    default:
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(opcode);
        std::cerr << "Unknown opcode: " << ss.str() << std::endl;
        cpu->halted = true;
        break;
    }
    }
}
