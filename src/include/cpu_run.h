#include "cpu_fde.h"
#include "cpu_loadp.h"

void run(CPU* cpu) {
    while (!cpu->halted) {
        execute_instruction(cpu);
    }
}