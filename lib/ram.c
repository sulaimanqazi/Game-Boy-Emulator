#include "../include/ram.h"
#include "../include/common.h"

typedef struct {
    u8 wram[0x2000]; //working ram is 2000 bytes (C000-DFFF)
    u8 hram[0x80];  //high ram is 80 bytes (FF80 -FFFE)
} ram_context;

static ram_context ctx;

u8 wram_read(u16 address) {
    address -= 0xC000; //subtract address by 0xC000 because it starts there

    if(address >= 0x2000){ //see if the address is too big and print error
        printf("INVALID WRAM ADDR %08X\n", address + 0xC000);
        exit(-1);
    }

    return ctx.wram[address]; //return ctx.wram at the index of address
}

void wram_write(u16 address, u8 value){
    address -= 0xC000; //subtract address by 0xC000 because it starts there

    ctx.wram[address] = value; //set value to ctx.wram at the index of address
}



u8 hram_read(u16 address) {
    address -= 0xFF80; //subtract address by 0xFF80 because it starts there



    return ctx.hram[address]; //return ctx.hram at the index of address
}

void hram_write(u16 address, u8 value){
    address -= 0xFF80; //subtract address by 0xFF80 because it starts there

    ctx.hram[address] = value; //set value to ctx.hram at the index of address
}