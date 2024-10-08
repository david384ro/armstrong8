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

void create_directory(const std::string &folder)
{
#ifdef _WIN32
    if (_mkdir(folder.c_str()) != 0 && errno != EEXIST)
    {
        std::cerr << "Error: Unable to create directory" << std::endl;
    }
#else
    if (mkdir(folder.c_str(), 0755) != 0 && errno != EEXIST)
    {
        std::cerr << "Error: Unable to create directory" << std::endl;
    }
#endif
}

void save_cpu_state(const CPU *cpu, const std::string &folder)
{
    create_directory(folder);

    std::string filename = folder + "/cpu_state.bin";
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<const char *>(&cpu->A), sizeof(cpu->A));
        file.write(reinterpret_cast<const char *>(&cpu->X), sizeof(cpu->X));
        file.write(reinterpret_cast<const char *>(&cpu->Y), sizeof(cpu->Y));
        file.write(reinterpret_cast<const char *>(&cpu->i3), sizeof(cpu->i3));
        file.write(reinterpret_cast<const char *>(&cpu->t1), sizeof(cpu->t1));
        file.write(reinterpret_cast<const char *>(&cpu->t2), sizeof(cpu->t2));
        file.write(reinterpret_cast<const char *>(&cpu->r4), sizeof(cpu->r4));
        file.write(reinterpret_cast<const char *>(&cpu->SP), sizeof(cpu->SP));
        file.write(reinterpret_cast<const char *>(&cpu->PC), sizeof(cpu->PC));
        file.write(reinterpret_cast<const char *>(&cpu->flags), sizeof(cpu->flags));
        file.write(reinterpret_cast<const char *>(&cpu->ppu.I), sizeof(cpu->ppu.I));
        file.close();
        std::cout << "CPU state saved to " << filename << std::endl;
    }
    else
    {
        std::cerr << "Error: Unable to open file for writing" << std::endl;
    }
}

void save_rom(const CPU *cpu, const std::string &folder)
{
    create_directory(folder);

    std::string filename = folder + "/rom.bin";
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<const char *>(cpu->rom), sizeof(cpu->rom));
        file.close();
        std::cout << "ROM data saved to " << filename << std::endl;
    }
    else
    {
        std::cerr << "Error: Unable to open file for writing" << std::endl;
    }
}

void save_ram(const CPU *cpu, const std::string &folder)
{
    create_directory(folder);

    std::string filename = folder + "/ram.bin";
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<const char *>(cpu->ram), sizeof(cpu->ram));
        file.close();
        std::cout << "RAM data saved to " << filename << std::endl;
    }
    else
    {
        std::cerr << "Error: Unable to open file for writing" << std::endl;
    }

    std::string xram_filename = folder + "/xram.bin";
    std::ofstream xram_file(xram_filename, std::ios::binary);
    if (xram_file.is_open())
    {
        xram_file.write(reinterpret_cast<const char *>(cpu->xram), sizeof(cpu->xram));
        xram_file.close();
        std::cout << "XRAM data saved to " << xram_filename << std::endl;
    }
    else
    {
        std::cerr << "Error: Unable to open file for writing XRAM" << std::endl;
    }

    std::string vram_filename = folder + "/vram.bin";
    std::ofstream vram_file(vram_filename, std::ios::binary);
    if (vram_file.is_open())
    {
        vram_file.write(reinterpret_cast<const char *>(cpu->ppu.vram), sizeof(cpu->ppu.vram));
        vram_file.close();
        std::cout << "VRAM data saved to " << vram_filename << std::endl;
    }
    else
    {
        std::cerr << "Error: Unable to open file for writing VRAM" << std::endl;
    }
}

void save_input(const CPU *cpu, const std::string &folder)
{
    create_directory(folder);

    std::string filename = folder + "/input.bin";
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<const char *>(cpu->input), sizeof(cpu->input));
        file.close();
        std::cout << "Input data saved to " << filename << std::endl;
    }
    else
    {
        std::cerr << "Error: Unable to open file for writing input data" << std::endl;
    }
}

void load_input(CPU *cpu, const std::string &folder, const std::string &filename)
{
    std::string filepath = folder + "/" + filename;
    std::ifstream file(filepath, std::ios::binary);

    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        if (static_cast<unsigned int>(size) > sizeof(cpu->input))
        {
            std::cerr << "Error: Input file size exceeds the input buffer size." << std::endl;
            file.close();
            return;
        }

        file.read(reinterpret_cast<char *>(cpu->input), size);
        file.close();

        std::cout << "Input data loaded from " << filepath << std::endl;
    }
    else
    {
        std::cerr << "Error: Unable to open file for reading input data" << std::endl;
    }
}

void initialize_cpu(CPU *cpu)
{
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    cpu->i3 = 0;
    cpu->t1 = 0;
    cpu->t2 = 0;
    cpu->r4 = 0;
    cpu->SP = 0xFF;
    cpu->PC = 0x00;
    cpu->flags = 0;
    cpu->io = 0;
    cpu->ppu.I = 0;
    std::memset(cpu->ram, 0, sizeof(cpu->ram));
    std::memset(cpu->xram, 0, sizeof(cpu->xram));
    std::memset(cpu->ppu.vram, 0, sizeof(cpu->ppu.vram));
    std::memset(cpu->rom, 0, sizeof(cpu->rom));
    std::memset(cpu->input, 0, sizeof(cpu->input));
    cpu->halted = false;
}

#define H 144
#define W 172

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("armstrong 8 display adapter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, 0);
    if (!window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    CPU cpu;
    initialize_cpu(&cpu);

    // uint8_t program[] = {0x44, 0x4a, 0x37, 0x0f, 0xbf, 0x60, 0x3a, 0x00, 0x41, 0x00};

    // load_program(&cpu, program, sizeof(program), 0x00);
    load_program_from_file(&cpu, "rom/program.bin", 0x00);
    load_input(&cpu, "input", "image1.bin");
    save_rom(&cpu, "rom");
    run(&cpu, window);

    save_input(&cpu, "input");
    save_cpu_state(&cpu, "cpu");
    save_ram(&cpu, "ram");

    return 0;
}