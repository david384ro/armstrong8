#include "cpu_main.hpp"
#include "cpu_instructions.hpp"
#include "cpu_setflag.hpp"
#include <sstream>
#include <iomanip>

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
        cpu->ppu.I++;
        set_flag(cpu, FLAG_ZERO, cpu->ppu.I == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->ppu.I & 0x80);
        break;
    }
    case DEP:
    {
        cpu->ppu.I--;
        set_flag(cpu, FLAG_ZERO, cpu->ppu.I == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->ppu.I & 0x80);
        break;
    }

    case BRK:
    {
        std::cout << "BRK encountered. Halting CPU." << std::endl;
        cpu->halted = true;
        break;
    }
    case BNE:
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
    case BEQ:
    {
        uint16_t address = cpu->rom[cpu->PC] /*| (cpu->rom[cpu->PC + 1] << 8)*/;
        cpu->PC++;
        if (get_flag(cpu, FLAG_ZERO))
        {
            cpu->PC = address;
        }
        break;
    }
    case BPL:
    {
        uint16_t address = cpu->rom[cpu->PC] /*| (cpu->rom[cpu->PC + 1] << 8)*/;
        cpu->PC++;
        if (!get_flag(cpu, FLAG_NEGATIVE))
        {
            cpu->PC = address;
        }
        break;
    }
    case BMI:
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
        set_flag(cpu, FLAG_ZERO, (result & 0xFF) == 0);
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
        set_flag(cpu, FLAG_ZERO, (result & 0xFF) == 0);
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
        set_flag(cpu, FLAG_ZERO, (result & 0xFF) == 0);
        set_flag(cpu, FLAG_NEGATIVE, result & 0x80);
        set_flag(cpu, FLAG_CARRY, cpu->Y >= value);
        break;
    }
    case CMPHL:
    {
        uint8_t low = cpu->rom[cpu->PC];
        uint8_t high = cpu->rom[cpu->PC + 1];
        uint16_t value = (high << 8) | low;
        cpu->PC += 2;
        uint16_t result = cpu->ppu.I - value;
        set_flag(cpu, FLAG_ZERO, (result & 0xFF) == 0);
        set_flag(cpu, FLAG_NEGATIVE, result & 0x80);
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
        set_flag(cpu, FLAG_ZERO, cpu->t1 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->t1 & 0x80);
        break;
    }
    case TT3P:
    {
        cpu->ppu.I = cpu->r4;
        set_flag(cpu, FLAG_ZERO, cpu->t1 == 0);
        set_flag(cpu, FLAG_NEGATIVE, cpu->t1 & 0x80);
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
        uint16_t address = cpu->ppu.I;
        cpu->ppu.write(address, cpu->A);
        break;
    }
    case REFRESH:
    {
        int W = 128;
        int H = 128;
        if (SDL_MUSTLOCK(surface))
        {
            SDL_LockSurface(surface);
        }
        uint32_t *dst = (uint32_t *)surface->pixels;
        uint8_t *src = cpu->ppu.vram;
        for (int y = 0; y < H; ++y)
        {
            for (int x = 0; x < W; ++x)
            {
                uint8_t color = src[y * W + x];
                uint32_t rgbaColor = (color << 16) | (color << 8) | color | (255 << 24);
                dst[y * (surface->pitch / sizeof(uint32_t)) + x] = rgbaColor;
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
