#include <fstream>
#include <iostream>

//oof
#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "include/cpu_run.hpp"

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

    std::string xram_filename = folder + "/xram.bin";
    std::ofstream xram_file(xram_filename, std::ios::binary);
    if (xram_file.is_open()) {
        xram_file.write(reinterpret_cast<const char*>(cpu->xram), sizeof(cpu->xram));
        xram_file.close();
        std::cout << "XRAM data saved to " << xram_filename << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing XRAM" << std::endl;
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
    std::memset(cpu->xram, 0, sizeof(cpu->xram));
    std::memset(cpu->rom, 0, sizeof(cpu->rom));
    cpu->halted = false;
}

/*
    LDA_IMMEDIATE = 0xA9
    LDA_ZERO_PAGE = 0xA5
    STA_ZERO_PAGE = 0x85
    LDA_NEXT_PAGE = 0xA6
    STA_NEXT_PAGE = 0x86
    STA_ZERO_PAGE_X = 0x87
    STA_NEXT_PAGE_X = 0x88
    INX = 0xE8
    BRK = 0x01
    BEQ = 0xF0
    BNE = 0xD0
    BPL = 0x10
    BMI = 0x30
    JMP = 0x4C
    ADD = 0x60
    SUB = 0x61
    MUL = 0x62
    CMP = 0x63
    CMX = 0x64
    CMXHL = 0x65
*/

int main() {
    CPU cpu;
    initialize_cpu(&cpu);

    // sample program smr 3 cai comp faster
    uint8_t program[] = {
        LDA_IMMEDIATE, 0xFF,   // 0x00: Load A with 0xFF
        STA_NEXT_PAGE_X,       // 0x02: Store A at xram[X]
        INX,                   // 0x03: Increment X
        CMXHL, 0x00, 0x20,     // 0x04: Compare X with 0x2000
        BNE, 0x02,             // 0x07: If X != 0x2000, jump back to 0x02
        BRK                    // 0x09: Break
        // :brah:
    };

    load_program(&cpu, program, sizeof(program), 0x00);
    save_rom(&cpu, "rom");
    run(&cpu);

    save_cpu_state(&cpu, "cpu");
    save_ram(&cpu, "ram");

    return 0;
}
