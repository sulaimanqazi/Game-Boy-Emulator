//ensure contents of header are included only once
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//undefined ints
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

//Checks if bit of integer a at the position N is set or not
#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)
//Sets or clears a at the position of n based on the value of on
#define BIT_SET(a, n, on) { if (on)  a |= (1 << n); else a &= ~(1 << n);}
//checks if a is between b and c,  inclusive
#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

u32 get_ticks();
void delay(u32 ms);

//Terminate emulator for things we didnt implement yet
#define NO_IMPL { fprintf(stderr, "NOT YET IMPLEMENTED\n"); exit(-5);}