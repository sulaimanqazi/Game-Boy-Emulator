//ensure contents of header are included only once
#pragma once

#include "common.h"

// see

typedef struct rom_header{
    u8 entry[4]; //0100-0103 (256-259) Entry point
    /*After displaying logo, boot ROM jumps to address 0100
    Which should jump to main program in cartridge
    Commercial games fill this 4 byte area with a nop
    instruction followed by a jp 0150
     */

    u8 logo[0x30]; //0104-0133 (260-307) Nintendo logo
    /* Area contains bitmap image displayed when game boy
     is powered on. Must match this hexadecimal dump
     or else boot ROM won't allow the game to run

     CE ED 66 66 CC 0D 00 0B 03 73 00 83 00 0C 00 0D
     00 08 11 1F 88 89 00 0E DC CC 6E E6 DD DD D9 99
     BB BB 67 63 6E 0E EC CC DD DC 99 9F BB B9 33 3E
     */

    char title[16]; //0134-0143 (308-323) Title
    /*
     Bytes contain title of game in upper ASCII
     If title is less than 16 characters,
     the remaining bytes should be padded with $00 s
     Parts of this area have a different meaning
     on later cartridges, reducing title size
     to 15 (0134-1432) or 11 (0134-013E)
     */
    u16 new_lic_code; //0144-0145 (324-325) New Licensee Code
    /*
     Area contains a two character ASCII licensee code
     indicating the game's publisher.
     Only used if old licensee is exactly $33 (all games after
     SGB released)
     Otherwise use the old code
     */
    u8 sgb_flag;//0146(326) SGB flag
    /*
     Byte specifies if the game supports SGB functions
     SGB will ignore command packets if this byte
     is set to a value other than $03, (which is usually $00)
     */
    u8 type; // 0147(327) Cartridge type
    /*
     Byte indicates what kind of hardware is present on the cartridge
     Most notably its mapper
     For example it could be:
     ROM ONLY or
     MBC1 or RAM or BATTERY or MMM01 or TIMER or MBC3
     or MBC5 or RUMBLE or SENSOR or POCKET CAMERA and so on
     */
    u8 rom_size; //0148(328) ROM size
    /*
     byte indiccating how much ROM is present on cartridge
     in most cases ROM is solved by the formula
     32KiB*( 1 << <value> )

     it goes from 32KiB to 1.5MiB
     */
    u8 ram_size; // 0149(329) RAM size
    /*
     Byte indicating how much RAM is present on the cartridge
     If cartridge type does not include RAM in its name (u8 type)
     This should be set to 0.
     This includes MBC2 since its 512x4 bits of memory are built
     directly into the mapper

     */
    u8 dest_code;// 014A(330) Destination Code
    /*
     Byte specifies whether this version of the game is
     intended to be sold in Japan or elsewhere.
     Only two values defined
     $00 - Japan / possible overseas
     $01 - Overseas only
     */
    u8 lic_code; //014B(331) - Old licensee Code
    /*
     Byte used in pre SGB cartridges to specify publisher
     Value $33 indicates that the new licensee codes must
     be considered instead.
     SGB ignores command packets unless this value is $33
     */
    u8 version; //014C(332) Mask ROM version number
    /*
     Specifies version number of the game
     usually $00
     */
    u8 checksum; //014D(333) Header checksum
    /*
     byte contains 8 bit checksum computed from
     cartridge header bytes $0134-$014C

     boot ROM verifies this checksum. If the byte at $014D
     does not match the lower 8 bits of checksum,
     the boot ROM will lock up and the program in the cartridge
     wont run.

     checksum is computer by running a for loop from
     0134-014C and setting a checksum to all that is inside
     */
    u16 global_checksum; //014E-014F (334-335) Global Checksum
    /*
     Bytes contain 16bit checksum computed as sum of all bytes
     of cartridge rom except these two
     checksum is not verified

     */
};

//load the cartridge
bool cart_load(char* cart);

//cart.c
u8 cart_read(u16 address);
void cart_write(u16 address, u8 value);


bool cart_need_save();
void cart_battery_load();
void cart_battery_save();