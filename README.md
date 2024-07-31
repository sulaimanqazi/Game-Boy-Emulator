# Game-Boy-Emulator
### Written in C and SDL2


![ezgif-1-f915983c0f](https://github.com/user-attachments/assets/4c8b1b3b-d28a-49da-a8d7-aa0eba2e9790)

(GIF attached shows me destroying Kano in Mortal Kombat 3)




I wrote the Game Boy Emulator to gain a better understanding of computer architecture and enhance my programming skills.

This was built by implementing all components from the Gameboy as code.

These parts include the CPU, the RAM, the memory, the IO, the PPU, and the UI. 

Unlike my Chip-8 intrepeter, the CPU is more complex. Here I implemented the Sharp LR35902. It uses several opcodes to complete the functions of a CPU. For example, one opcode I implemented (0x01) LD BC, u16 sets the register pair BC to a specific 16-bit value found in the location of the program counter. Since most opcodes function in similar ways I just implemented multiple methods within each opcode.


