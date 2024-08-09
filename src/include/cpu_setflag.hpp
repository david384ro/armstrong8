#include "cpu_main.hpp"

void set_flag(CPU *cpu, uint8_t flag, bool value)
{
    if (value)
    {
        cpu->flags |= flag;
    }
    else
    {
        cpu->flags &= ~flag;
    }
}

bool get_flag(CPU *cpu, uint8_t flag)
{
    return (cpu->flags & flag) != 0;
}