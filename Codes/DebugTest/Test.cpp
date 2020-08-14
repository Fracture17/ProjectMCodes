//
// Created by johno on 7/31/2020.
//

#include "Wii/EXI.h"
#include "Memory.h"
#include "Assembly.h"
#include "stddef.h"


BASIC_INJECT("TEST", 0x80017470, "lwz r0, 0xF8(r22)");


void* readDest = (void*) 0x91331c00;
int shouldRead = 0;

extern "C" void TEST() {
    if(shouldRead == 1) {
        readEXI(readDest, 0x1000, slotB, device0, EXI_32MHz);
    }
}