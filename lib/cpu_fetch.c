
#include "../include/cpu.h"
#include "../include/bus.h"
#include "../include/emu.h"

extern cpu_context ctx;
void fetch_data(){
    ctx.mem_dest = 0;
    ctx.dest_is_mem = false;


    if(ctx.cur_inst == NULL){ //if its IN_NONE
        return;

    }

    //what to do based on current instructions addressing mode
    switch(ctx.cur_inst-> mode){
        case AM_IMP: return; //Implied= nothing needs to be read so just return

        case AM_R: //Register
            //Read data from register
            ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_1);
            return;
        case AM_R_R: //Register
            //Read data from register
            ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_2);
            return;
        case AM_R_D8: //Take 8 bit value and transfer it into a register
            ctx.fetched_data = bus_read(ctx.regs.pc); //Fetch data from program counter
            emu_cycles(1); //increment emulator cycle
            ctx.regs.pc++; //increment program counter
            return;
        case AM_R_D16: //Takes a register and a 16 bit number
        case AM_D16:{
                //we can only read 8 bits at a time so break it up
                //lower 8 bits
                u16 lo = bus_read(ctx.regs.pc);
                emu_cycles(1);
                //read 8 higher bits
                u16 hi = bus_read(ctx.regs.pc+1);
                emu_cycles(1);

                ctx.fetched_data = lo | (hi<<8); //fetched data = lower 8 bits OR'd with high byte shifted (to make it work) 

                ctx.regs.pc += 2;

                return;
            }
        case AM_MR_R: //Loading a register into a memory region, //Grab second register_2); //Grab second register
                ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_2);
                ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1); //Destination is first register
                ctx.dest_is_mem = true; //Let the program know that the destination is a memory location

                if(ctx.cur_inst->reg_1 == RT_C){ //If register 1 is the same as register type C, set the most significant bit of the memory destination to FF 
                    ctx.mem_dest |= 0xFF00;
                }

                return;

        case AM_R_MR:{ //Load memory region to register

                u16 addr = cpu_read_reg(ctx.cur_inst->reg_2); //Grab second register

                if(ctx.cur_inst->reg_2 == RT_C){ //If register 2 is the same as register type C, set the most significant bit of the address to FF 
                    addr |= 0xFF00;
                }

                ctx.fetched_data = bus_read(addr);
                emu_cycles(1);
                
                }return;
        case AM_R_HLI: //Loading address of hl register and increment hl register
                ctx.fetched_data = bus_read(cpu_read_reg(ctx.cur_inst->reg_2)); //Read the second register which holds HL 
                emu_cycles(1);
                cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1); //increment hl register
                return;

        case AM_R_HLD: //Loading address of hl register and decrement hl register
                ctx.fetched_data = bus_read(cpu_read_reg(ctx.cur_inst->reg_2)); //Read the second register which holds HL 
                emu_cycles(1);
                cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1); //decrement hl register
                return;
        
        case AM_HLI_R: //Removing register value into the hl address and increment it
                ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_2); //Read current instruction register 2 
                ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1); //Set memory destination to register 1
                ctx.dest_is_mem = true; //Let the program know that the destination is a memory location
                cpu_set_reg(RT_HL,cpu_read_reg(RT_HL)+1); //increment hl register
                return;
        
        case AM_HLD_R: //Removing register value into the hl address and decrement it
                ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_2); //Read current instruction register 2 
                ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1); //Set memory destination to register 1
                ctx.dest_is_mem = true; //Let the program know that the destination is a memory location
                cpu_set_reg(RT_HL,cpu_read_reg(RT_HL) - 1); //decrement hl register
                return;
        
        case AM_R_A8: //Moving from a8 to a register
                ctx.fetched_data = bus_read(ctx.regs.pc); //fetched data will be read from the program counter
                emu_cycles(1);
                ctx.regs.pc++;
                return;
        
        case AM_A8_R: //Moving from a register into A8
                ctx.mem_dest = bus_read(ctx.regs.pc) | 0xFF00; //memory destination is program counter with msb as FF 
                ctx.dest_is_mem = true; //Let the program know that the destination is a memory location
                emu_cycles(1);
                ctx.regs.pc++;
                return;

        case AM_HL_SPR: //Load stack pointer into HL incremented by r8
                ctx.fetched_data = bus_read(ctx.regs.pc); //fetched data will be read from the program counter
                emu_cycles(1);
                ctx.regs.pc++;
                return;

        case AM_D8:
                ctx.fetched_data = bus_read(ctx.regs.pc); //fetched data will be read from the program counter
                emu_cycles(1);
                ctx.regs.pc++;
                return;

        case AM_A16_R:
        case AM_D16_R: { //Move register into a 16 bit address
                //we can only read 8 bits at a time so break it up
                //lower 8 bits of address at program counter
                u16 lo = bus_read(ctx.regs.pc);
                emu_cycles(1);
                //read 8 higher bits of address at program counter
                u16 hi = bus_read(ctx.regs.pc+1);
                emu_cycles(1);

                ctx.mem_dest = lo | (hi<<8); // memory destination = lower 8 bits OR'd with high byte shifted (to make it work) 
                ctx.dest_is_mem = true; //Let the program know that the destination is a memory location

                ctx.regs.pc += 2;
                ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_2); //Grab second register
                
                
        } return;

        case AM_MR_D8: //Loading a d8 into a memory address of a register
                ctx.fetched_data = bus_read(ctx.regs.pc); //read value from program counter
                emu_cycles(1);
                ctx.regs.pc++;
                ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1); //set memory destination to value of register 1
                ctx.dest_is_mem = true; //Let the program know that the destination is a memory location
                return;

        case AM_MR: //Set memory region
                ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1); //set memory destination to value of register 1
                ctx.dest_is_mem = true; //Let the program know that the destination is a memory location
                ctx.fetched_data = bus_read(cpu_read_reg(ctx.cur_inst->reg_1)); //read data of first register in case its needed
                emu_cycles(1);
                return;
        

        case AM_R_A16: {
                //we can only read 8 bits at a time so break it up
                //lower 8 bits
                u16 lo = bus_read(ctx.regs.pc);
                emu_cycles(1);
                //read 8 higher bits
                u16 hi = bus_read(ctx.regs.pc+1);
                emu_cycles(1);

                u16 addr = lo | (hi<<8); //fetched data = lower 8 bits OR'd with high byte shifted (to make it work) 

                ctx.regs.pc += 2;
                ctx.fetched_data = bus_read(addr); //Fetched data bus_= 8 bit addr
                emu_cycles(1);
                return;
        } 


        


        /* MIGHT DELETE
        case AM_R_D16: 
            {
                //we can only read 8 bits at a time so break it up
                //lower 8 bits
                u16 lo = bus_read(ctx.regs.pc);
                emu_cycles(1);
                //read 8 higher bits
                u16 hi = bus_read(ctx.regs.pc+1);
                emu_cycles(1);


                ctx.fetched_data = lo | (hi<<8); //fetched data = lower 8 bits OR'd with high byte shifted (to make it work) 

                ctx.regs.pc += 2;


            }
*/

        default: //if we have to deal with an addressing mode thats an error/ i didnt implement 
            printf("unknown addressing mode %d (%02X)\n", ctx.cur_inst-> mode, ctx.cur_opcode);
            exit(-7);
            return;
    }
}