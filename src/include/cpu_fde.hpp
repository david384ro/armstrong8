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
        case BEQ: {
            int8_t offset = cpu->rom[cpu->PC];
            cpu->PC++;
            if (get_flag(cpu, FLAG_ZERO)) {
                cpu->PC += offset;
            }
            break;
        }
        case BNE: {
            int8_t offset = cpu->rom[cpu->PC];
            cpu->PC++;
            if (!get_flag(cpu, FLAG_ZERO)) {
                cpu->PC += offset;
            }
            break;
        }
        case BPL: {
            int8_t offset = cpu->rom[cpu->PC];
            cpu->PC++;
            if (!get_flag(cpu, FLAG_NEGATIVE)) {
                cpu->PC += offset;
            }
            break;
        }
        case BMI: {
            int8_t offset = cpu->rom[cpu->PC];
            cpu->PC++;
            if (get_flag(cpu, FLAG_NEGATIVE)) {
                cpu->PC += offset;
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
        default: {
            std::stringstream ss;
            ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(opcode);
            std::cerr << "Unknown opcode: " << ss.str() << std::endl;
            cpu->halted = true;
            break;
        }
    }
}
