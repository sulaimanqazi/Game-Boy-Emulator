#pragma once

#include <common.h>

//Stack is used with instructions like RET or JP which has instructions for push and pop


void stack_push(u8 data);
void stack_push16(u16 data);

u8 stack_pop();
u16 stack_pop16();

