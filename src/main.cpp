#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "include/cpu_run.h"

void create_directory(const std::string& folder) {
    #ifdef _WIN32
    if (_mkdir(folder.c_str()) != 0 && errno != EEXIST) {
        std::cerr << "Error: Unable to create directory" << std::endl;
    }
    #else
    if (mkdir(folder.c_str(), 0755) != 0 && errno != EEXIST) {
        std::cerr << "Error: Unable to create directory" << std::endl;
    }
    #endif
}

void save_cpu_state(const CPU* cpu, const std::string& folder) {
    create_directory(folder);

    std::string filename = folder + "/cpu_state.bin";
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&cpu->A), sizeof(cpu->A));
        file.write(reinterpret_cast<const char*>(&cpu->X), sizeof(cpu->X));
        file.write(reinterpret_cast<const char*>(&cpu->Y), sizeof(cpu->Y));
        file.write(reinterpret_cast<const char*>(&cpu->SP), sizeof(cpu->SP));
        file.write(reinterpret_cast<const char*>(&cpu->PC), sizeof(cpu->PC));
        file.write(reinterpret_cast<const char*>(&cpu->flags), sizeof(cpu->flags));
        file.close();
        std::cout << "CPU state saved to " << filename << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing" << std::endl;
    }
}

void save_rom(const CPU* cpu, const std::string& folder) {
    create_directory(folder);

    std::string filename = folder + "/rom.bin";
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(cpu->rom), sizeof(cpu->rom));
        file.close();
        std::cout << "ROM data saved to " << filename << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing" << std::endl;
    }
}

void save_ram(const CPU* cpu, const std::string& folder) {
    create_directory(folder);

    std::string filename = folder + "/ram.bin";
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(cpu->ram), sizeof(cpu->ram));
        file.close();
        std::cout << "RAM data saved to " << filename << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing" << std::endl;
    }
}

void initialize_cpu(CPU* cpu) {
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    cpu->SP = 0xFF;
    cpu->PC = 0x00;
    cpu->flags = 0;
    std::memset(cpu->ram, 0, sizeof(cpu->ram));
    std::memset(cpu->rom, 0, sizeof(cpu->rom));
    cpu->halted = false;
}

/*
    LDA_IMMEDIATE = 0xA9
    LDA_ZERO_PAGE = 0xA5
    STA_ZERO_PAGE = 0x85
    INX = 0xE8
    BRK = 0x00
    BEQ = 0xF0
    BNE = 0xD0
    BPL = 0x10
    BMI = 0x30
    JMP = 0x4C
    ADD = 0x60
    SUB = 0x61
    MUL = 0x62
    CMP = 0x63

    these are the instructions
    ADD works by taking the value of A and adding it with the next value AFTER ADD (so basically it does A + VALUE), which is true in most of these cases.
*/

int main() {
    CPU cpu;
    initialize_cpu(&cpu);

    // sample program
    uint8_t program[] = {
        LDA_IMMEDIATE, 0x05,
        INX,
        ADD, 0x0A,
        SUB, 0x03,
        BEQ, 0x10,

        STA_ZERO_PAGE, 0x00,
        BRK,
    };

    load_program(&cpu, program, sizeof(program), 0x00);
    run(&cpu);

    save_cpu_state(&cpu, "cpu");
    save_rom(&cpu, "rom");
    save_ram(&cpu, "ram");

    return 0;
}
