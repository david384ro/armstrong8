#include "cpu_fde.hpp"
#include "cpu_loadp.hpp"

void run(CPU* cpu) {
    while (!cpu->halted) {
        execute_instruction(cpu);
    }
}