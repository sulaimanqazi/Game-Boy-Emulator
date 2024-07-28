
//read data from cartridge
//do all reading and writing from the bus

#include "../include/bus.h"
#include "../include/cart.h"
#include "../include/ram.h"
#include "../include/cpu.h"
#include "../include/io.h"
#include "../include/ppu.h"

#include "../include/dma.h"

/* 03 issue is NOT CPU FETCH, CPU PROC, CPU UTIL, BUS
gbemu/gbemu ../roms/tetris.gb
gbemu/gbemu ../roms/01-special.gb
gbemu/gbemu ../roms/mem_timing.gb
gbemu/gbemu ../roms/cpu_instrs.gb
gbemu/gbemu ../roms/dmg-acid2.gb

*/

/*
memory map for each section of the memory
0x0000 - 0x3FFF : ROM Bank 0
0x4000 - 0x7FFF : ROM Bank 1 - Switchable
0x8000 - 0x97FF : CHR RAM
0x9800 - 0x9BFF : BG Map 1
0x9C00 - 0x9FFF : BG Map 2
0xA000 - 0xBFFF : Cartridge RAM
0xC000 - 0xCFFF : RAM Bank 0
0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only
0xE000 - 0xFDFF : Reserved - Echo RAM
0xFE00 - 0xFE9F : Object Attribute Memory
0xFEA0 - 0xFEFF : Reserved - Unusable
0xFF00 - 0xFF7F : I/O Registers
0xFF80 - 0xFFFE : Zero Page
*/

u8 bus_read(u16 address) {
    if (address < 0x8000) {
        //ROM Data (Rom bank 0-1) from cartridge
        return cart_read(address);
    } else if (address < 0xA000){
        //Char/Map Data (CHR RAM and BG Map 1-2)
        return ppu_vram_read(address);
    } else if (address < 0xC000) {
        //Cartridge RAM / EXT-RAM
        return cart_read(address);
    }else if (address < 0xE000) {
        //WRAM (Working RAM) RAM bank 0-8,
        return wram_read(address);
    }else if (address < 0xFE00) {
        //Reserved echo RAM
        return 0;
    } else if (address < 0xFEA0) {
        //Object Attribute Memory
        if(dma_transferring()){
            return 0xFF;
        }
        return ppu_oam_read(address);
    } else if (address < 0xFF00) {
        //Reserved - Unusable
        return 0;
    } else if (address < 0xFF80) {
        //IO Registers
        return io_read(address);


    } else if (address == 0xFFFF) {
        //CPU ENABLE REGISTER
         return cpu_get_ie_register(); //read the value of the interrupt enable register
    }
    //NO_IMPL

    return hram_read(address); //between FF80 and FFFF, its the zero page / high ram
}

void bus_write(u16 address, u8 value) {
    if (address < 0x8000) {
        //ROM Data (Rom bank 0-1) from cartridge
        cart_write(address, value);
    } else if (address < 0xA000){
        //Char/Map Data (CHR RAM and BG Map 1-2)
        ppu_vram_write(address, value);
    } else if (address < 0xC000){
        //Cartridge RAM / EXT-RAM
        cart_write(address, value);
        
    } else if (address < 0xE000) {
        //WRAM (Working RAM) RAM bank 0-8,
        wram_write(address, value);
    } else if (address < 0xFE00) {
        //reserved echo ram
    
    }
     else if (address < 0xFEA0) {
        //Object Attribute Memory
        if(dma_transferring()){
            return;
        }

        ppu_oam_write(address,value);

    }else if (address < 0xFF00) {
        //Unusable reserved
    } else if (address < 0xFF80) {
        //IO REGISTERS
        io_write(address, value);
    } else if (address == 0xFFFF) {
        //CPU SET ENABLE REGISTER

        cpu_set_ie_register(value); //set the value of the interrupt enable register
    } else{ 
        hram_write(address, value);//between FF80 and FFFF, its the zero page / high ram
    }


}

u16 bus_read16(u16 address) {
    //Read lo byte address
    u16 lo = bus_read(address);
    //read address+1
    u16 hi = bus_read(address + 1);

    //Or them together
    return lo | (hi << 8);
}

void bus_write16(u16 address, u16 value) {
    //set address+1 to MSB
    bus_write(address + 1, (value >> 8) & 0xFF);
    //set address to lower byte
    bus_write(address, value & 0xFF);
}