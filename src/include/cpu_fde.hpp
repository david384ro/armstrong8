#include "cpu_main.hpp"
#include "cpu_instructions.hpp"
#include "cpu_setflag.hpp"
#include <sstream>
#include <iomanip>

void execute_instruction(CPU* cpu) {
    uint8_t opcode = cpu->rom[cpu->PC];
    cpu->PC++;
    switch (opcode) {
        case LDA_IMMEDIATE: {
            uint8_t value = cpu->rom[cpu->PC];
            cpu->PC++;
            cpu->A = value;
            set_flag(cpu, FLAG_ZERO, cpu->A == 0);
            set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
            break;
        }
        case LDA_ZERO_PAGE: {
            uint8_t addr = cpu->rom[cpu->PC];
            cpu->PC++;
            cpu->A = cpu->ram[addr];
            set_flag(cpu, FLAG_ZERO, cpu->A == 0);
            set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
            break;
        }
        case STA_ZERO_PAGE: {
            uint8_t addr = cpu->rom[cpu->PC];
            cpu->PC++;
            cpu->ram[addr] = cpu->A;
            break;
        }
        case LDA_NEXT_PAGE: {
            uint8_t addr = cpu->rom[cpu->PC];
            cpu->PC++;
            cpu->A = cpu->xram[addr];
            set_flag(cpu, FLAG_ZERO, cpu->A == 0);
            set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
            break;
        }
        case STA_NEXT_PAGE: {
            uint8_t addr = cpu->rom[cpu->PC];
            cpu->PC++;
            cpu->xram[addr] = cpu->A;
            break;
        }
        case STA_NEXT_PAGE_X: {
            cpu->xram[cpu->X] = cpu->A;
            break;
        }
        case STA_ZERO_PAGE_X: {
            cpu->ram[cpu->X] = cpu->A;
            break;
        }
        case INX: {
            cpu->X++;
            set_flag(cpu, FLAG_ZERO, cpu->X == 0);
            set_flag(cpu, FLAG_NEGATIVE, cpu->X & 0x80);
            break;
        }
        case BRK: {
            std::cout << "BRK encountered. Halting CPU." << std::endl;
            cpu->halted = true;
            break;
        }
         case BNE: {
            uint16_t address = cpu->rom[cpu->PC] | (cpu->rom[cpu->PC + 1] << 8);
            cpu->PC += 2;
            if (!get_flag(cpu, FLAG_ZERO)) {
                cpu->PC = address;
            }
            break;
        }
        case BEQ: {
            uint16_t address = cpu->rom[cpu->PC] | (cpu->rom[cpu->PC + 1] << 8);
            cpu->PC += 2;
            if (get_flag(cpu, FLAG_ZERO)) {
                cpu->PC = address;
            }
            break;
        }
        case BPL: {
            uint16_t address = cpu->rom[cpu->PC] | (cpu->rom[cpu->PC + 1] << 8);
            cpu->PC += 2;
            if (!get_flag(cpu, FLAG_NEGATIVE)) {
                cpu->PC = address;
            }
            break;
        }
        case BMI: {
            uint16_t address = cpu->rom[cpu->PC] | (cpu->rom[cpu->PC + 1] << 8);
            cpu->PC += 2;
            if (get_flag(cpu, FLAG_NEGATIVE)) {
                cpu->PC = address;
            }
            break;
        }
        case JMP: {
            uint16_t addr = cpu->rom[cpu->PC] | (cpu->rom[cpu->PC + 1] << 8);
            cpu->PC += 2;
            cpu->PC = addr;
            break;
        }
        case ADD: {
            uint8_t value = cpu->rom[cpu->PC];
            cpu->PC++;
            uint16_t result = cpu->A + value;
            cpu->A = result & 0xFF; // 8-bit result
            set_flag(cpu, FLAG_ZERO, cpu->A == 0);
            set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
            set_flag(cpu, FLAG_CARRY, result > 0xFF);
            break;
        }
        case SUB: {
            uint8_t value = cpu->rom[cpu->PC];
            cpu->PC++;
            uint16_t result = cpu->A - value;
            cpu->A = result & 0xFF; // 8-bit result
            set_flag(cpu, FLAG_ZERO, cpu->A == 0);
            set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
            set_flag(cpu, FLAG_CARRY, result > 0xFF);
            break;
        }
        case MUL: {
            uint8_t value = cpu->rom[cpu->PC];
            cpu->PC++;
            uint16_t result = cpu->A * value;
            cpu->A = result & 0xFF; // 8-bit result
            set_flag(cpu, FLAG_ZERO, cpu->A == 0);
            set_flag(cpu, FLAG_NEGATIVE, cpu->A & 0x80);
            set_flag(cpu, FLAG_CARRY, result > 0xFF);
            break;
        }
        case CMP: {
            uint8_t value = cpu->rom[cpu->PC];
            cpu->PC++;
            uint16_t result = cpu->A - value;
            set_flag(cpu, FLAG_ZERO, (result & 0xFF) == 0);
            set_flag(cpu, FLAG_NEGATIVE, result & 0x80);
            set_flag(cpu, FLAG_CARRY, cpu->A >= value);
            break;
        }
        case CMX: {
            uint8_t value = cpu->rom[cpu->PC];
            cpu->PC++;
            uint16_t result = cpu->X - value;
            set_flag(cpu, FLAG_ZERO, (result & 0xFF) == 0);
            set_flag(cpu, FLAG_NEGATIVE, result & 0x80);
            set_flag(cpu, FLAG_CARRY, cpu->A >= value);
            break;
        }
        case CMXHL: {
            uint8_t low = cpu->rom[cpu->PC];
            uint8_t high = cpu->rom[cpu->PC + 1];
            cpu->PC += 2;
            uint16_t value = (high << 8) | low;
            uint16_t result = cpu->X - value;
            set_flag(cpu, FLAG_ZERO, (result & 0xFFFF) == 0);
            set_flag(cpu, FLAG_NEGATIVE, result & 0x8000);
            set_flag(cpu, FLAG_CARRY, cpu->X >= value);
            break;
        }
        default: {
            std::stringstream ss;
            ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(opcode);
            std::cerr << "Unknown opcode: " << ss.str() << std::endl;
            cpu->halted = true;
            break;
        }
    }
}
