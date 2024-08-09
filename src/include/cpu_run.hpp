#include "cpu_fde.hpp"
#include "cpu_loadp.hpp"

void handle_event(SDL_Event* event, CPU* cpu) {
    switch (event->type) {
        case SDL_QUIT:
            cpu->halted = true;
            break;
        case SDL_KEYDOWN:
            break;
    }
}

void run(CPU* cpu, SDL_Window* window) {
    SDL_Event event;

    while (true) {
        while (SDL_PollEvent(&event)) {
            handle_event(&event, cpu);
            if (event.type == SDL_QUIT) {
                return;
            }
        }

        if (!cpu->halted) {
            execute_instruction(cpu, window);
        } else {
            SDL_Delay(10);
        }
    }
}