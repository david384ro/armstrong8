#include "cpu_main.hpp"

void load_program(CPU *cpu, const uint8_t *program, uint16_t size, uint16_t start_addr)
{
    std::memcpy(&cpu->rom[start_addr], program, size);
    cpu->PC = start_addr;
}

void load_program_from_file(CPU *cpu, const std::string &filename, uint16_t start_addr)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file for reading program data: " << filename << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size < 0 || static_cast<uint16_t>(size) + start_addr > sizeof(cpu->rom))
    {
        std::cerr << "Error: Program file size exceeds available ROM space." << std::endl;
        file.close();
        return;
    }

    file.read(reinterpret_cast<char *>(&cpu->rom[start_addr]), size);
    file.close();

    cpu->PC = start_addr;

    std::cout << "Program loaded from " << filename << " starting at address 0x"
              << std::hex << start_addr << std::endl;
}