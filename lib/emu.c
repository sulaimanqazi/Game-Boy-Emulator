#include "../include/cpu.h"
#include "../include/ppu_sm.h"

#include <stdio.h>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <timer.h>
#include <dma.h>
#include <ppu.h>

#include <pthread.h>
#include <unistd.h>

/* 
  Emu components:

  |Cart|
  |CPU|
  |Address Bus|
  |PPU|
  |Timer|

*/

//Name for emulator context
static emu_context ctx;

//To return emu context
emu_context *emu_get_context() {
    return &ctx;
}


//Using a delay with SDL
//void delay(u32 ms) {
  //  SDL_Delay(ms);
//}

void *cpu_run(void *p) {
    ppu_init();

    timer_init();

    //intialize the CPU
    cpu_init();

    //set context variables
    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    //infinite game loop if its running
    while(ctx.running) {
    
        //if the emulator is paused keep on looping a delay 
        if (ctx.paused) {
            delay(10);
            continue;
        }

        //Do a step of the cpu but if it failes terminate the program
        if (!cpu_step()) {
            printf("CPU Stopped\n");
            return 0;
        }

        //increment ticks and keep on with main loop
        //ctx.ticks++;
    }
    return 0;
}


//Main function
int emu_run(int argc, char **argv) {
    //Check if they passed in a romfile if there is no rom file throw an error
    if (argc < 2) {
        printf("Usage: emu <rom_file>\n");
        return -1;
    }
    //Cart load will load the cartridge, but if that failed throw an error
    if (!cart_load(argv[1])) {
        printf("Failed to load ROM file: %s\n", argv[1]);
        return -2;
    } 
    printf("Cart loaded..\n");

  
    ui_init();
    
    pthread_t t1;

    if(pthread_create(&t1, NULL, cpu_run, NULL)) {
        fprintf(stderr, "FAILED TO START MAIN CPU THREAD");
        return -1;
    }

    u32 prev_frame = 0;

    while(!ctx.die){
        usleep(1000);
        ui_handle_events();

        if(prev_frame != ppu_get_context()->current_frame){
            ui_update();
        }

        prev_frame = ppu_get_context()->current_frame;
    }

    return 0;
}

void emu_cycles(int cpu_cycles){
    
    
    for (int i = 0; i < cpu_cycles; i++) {
        for(int n = 0; n < 4; n++) {
            ctx.ticks++;
            timer_tick();
            ppu_tick();
        }
        dma_tick();
    }
    
    
}