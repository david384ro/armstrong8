#include "cpu_main.hpp"

void load_program(CPU *cpu, const uint8_t *program, uint16_t size, uint16_t start_addr)
{
    std::memcpy(&cpu->rom[start_addr], program, size);
    cpu->PC = start_addr;
}