#include <fstream>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define SDL_MAIN_HANDLED
#include "include/SDL2/SDL.h"

#include "include/cpu_run.hpp"
#include "include/ppu/ppu_main.hpp"

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

    std::string vram_filename = folder + "/vram.bin";
    std::ofstream vram_file(vram_filename, std::ios::binary);
    if (vram_file.is_open()) {
        vram_file.write(reinterpret_cast<const char*>(cpu->ppu.vram), sizeof(cpu->ppu.vram));
        vram_file.close();
        std::cout << "VRAM data saved to " << vram_filename << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing VRAM" << std::endl;
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
    std::memset(cpu->ppu.vram, 0, sizeof(cpu->ppu.vram));
    std::memset(cpu->rom, 0, sizeof(cpu->rom));
    cpu->halted = false;
}

int main() {
    CPU cpu;
    initialize_cpu(&cpu);

    uint8_t program[] = {0x12, 0xff, 0x1d, 0x15, 0x07, 0xff, 0x18, 0x02, 0x00};

    load_program(&cpu, program, sizeof(program), 0x00);
    save_rom(&cpu, "rom");
    run(&cpu);

    save_cpu_state(&cpu, "cpu");
    save_ram(&cpu, "ram");

    return 0;
}