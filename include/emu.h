//ensure contents of header are included only once
#pragma once

#include "common.h"
#include "cpu.h"

//typedef = keyword giving datatype a nickname

//holds data about running emulator
typedef struct {
    bool paused; //tells whether its paused or running
    bool running;
    bool die;
    u64 ticks;// Shows processor ticks
}emu_context;

//main function
int emu_run(int argc, char **argv);

//function to get emulator context in other places 
emu_context *emu_get_context();

//Emulator cycles for ppu and timer
void emu_cycles(int cpu_cycles);
