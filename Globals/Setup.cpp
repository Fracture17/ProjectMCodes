//
// Created by johno on 11/26/2020.
//

#include "Setup.h"

INJECTION("setup", 0x80017504, R"(
    SAVE_REGS
    li r3, 0
    bl setup
    RESTORE_REGS
    stw r0, 0x100(r23)
)");


INJECTIONS* injections;


//Changing this name requires changing the build system
extern "C" void setup(MEMORY_HEAP_INFO* heapInfo) {
    //Only first run passes non null
    bool firstTime = heapInfo != nullptr;

    if(firstTime) {
        injections = (INJECTIONS*) (heapInfo + 1);

        _INITIALIZE_MEMORY_(heapInfo->heapAddress, heapInfo->heapSize);
    }

    for(int i = 0; i < injections->numInjections; i++) {
        performInjection(injections->injections[i], firstTime);
    }

    auto startups = (STARTUPS*) (injections->injections + injections->numInjections);
    if(firstTime) {
        for(int i = 0; i < startups->numStartups; i++) {
            startups->startupFunctions[i]();
        }
    }
}


u32 makeBranchInstruction(u32 targetAddress, u32 startAddress) {
    int offset = targetAddress - startAddress;
    u32 maskedOffset = ((1 << 26) - 1) & offset;
    u32 branchInstruction = maskedOffset | 0x48000000;
    return branchInstruction;
}

void invalidateCache(u32 address) {
    asm volatile ( R"(
        dcbst 0, %0
        sync
        icbi 0, %0
        isync)"
    :
    : "r" (address)
    );
}

void performInjection(INJECTIONS::_injection& injection, bool shouldInvalidate) {
    auto branchInstruction = makeBranchInstruction(injection.injectedFunctionAddress, injection.injectionAddress);
    *((u32*) injection.injectionAddress) = branchInstruction;
    if(shouldInvalidate) {
        invalidateCache(injection.injectionAddress);
    }

    auto returnBranchInstruction = makeBranchInstruction(injection.injectionAddress + 4, injection.returnBranchAddress);
    *((u32*) injection.returnBranchAddress) = returnBranchInstruction;
    if(shouldInvalidate) {
        invalidateCache(injection.returnBranchAddress);
    }
}