//
// Created by johno on 7/5/2020.
//



#include "Setup.h"


asm(R"(
SETUP_INFO:
    {fileNames}
    {injections}
    {startupFunctionAddress}
)")

#define NUM_FILES ({numFiles})
#define NUM_INJECTIONS ({numInjections})




struct Injection {
    unsigned int injectionAddress;
    unsigned int injectedFunctionAddress;
    unsigned int returnBranchAddress;
};

struct CodeFile {
    char name[4];
    unsigned int address;
};


struct SetupInfo {
    CodeFile fileNames[NUM_FILES];
    Injection injections[NUM_INJECTIONS];
    (void (startupFunction*)());

};









asm(R"(
    .section .text.saveRegs
	.globl saveRegs
	.type saveRegs, @function
saveRegs:
	stw 0, 4(1)
	mfctr 0
	stw 0, -8(1)

	stwu 1, -248(1)
	stmw 2, 8(1)

	blr

	.size saveRegs, .-saveRegs
)");

asm(R"(
    .section .text.restoreRegs
	.globl restoreRegs
	.type restoreRegs, @function
restoreRegs:
	lmw 2, 8(1)
	addi 1, 1, 248

	lwz 0, -8(1)
	mtctr 0
	lwz 0, 4(1)

	blr

	.size restoreRegs, .-restoreRegs
)");


#define FAFOpen ((FILE* (*)(const char* filepath, const char* mode)) 0x803ebeb8)
#define FAFRead ((size_t (*)(const void* destination, size_t size, size_t count, FILE* stream)) 0x803ebee4)
#define FAFClose ((int (*)(FILE* file)) 0x803ebe8c)

struct FILE {};

void readFile(const char* name, void* destination) {
    auto file = FAFOpen(name, "r");
    FAFRead(destination, 0x8000, 1, file);
    FAFClose(file);
}

void readCodeFiles(SetupInfo& info) {
    for(auto& file: info.fileNames) {
        readFile(file.name, file.address);
    }
}



unsigned int makeBranchInstruction(unsigned int targetAddress, unsigned int startAddress) {
    int offset = targetAddress - startAddress;
    unsigned int maskedOffset = (2<<26 - 1) & offset;
    unsigned int branchInstruction = maskedOffset | 0x48000000;
    return branchInstruction;
}

void invalidateCache(unsigned int address) {
    asm volatile ( R"(
        dcbst 0, %0
        sync
        icbi 0, %0
        isync)"
    :
    : "r" (address)
    );
}


void makeInjection(Injection& injection) {
    auto branchInstruction = makeBranchInstruction(injection.injectedFunctionAddress, injection.injectionAddress);
    *((unsigned int*) injection.injectionAddress) = branchInstruction;
    if(shouldInvalidate) {
        invalidateCache(injection.injectionAddress);
    }

    auto returnBranchInstruction = makeBranchInstruction(injection.injectionAddress + 4, injection.returnBranchAddress);
    *((unsigned int*) injection.returnBranchAddress) = returnBranchInstruction;
    if(shouldInvalidate) {
        invalidateCache(injection.returnBranchAddress);
    }
}


void makeInjectionBranches(InitializerFile& file, bool shouldInvalidate) {
    for(auto& injection: file.injections) {
        makeInjection(injection);
    }
}

extern "C" void setup() {
    SetupInfo* info;
    asm(R"(
        lis %0, SETUP_INFO@ha
        ori %0, SETUP_INFO@l)"
        : "=r" (info)
        );
    readCodeFiles(*info);
    makeInjectionBranches(*info, true);
    info->startupFunction();
}


extern "C" void inject() {
    makeInjectionBranches(*INITIALIZER_FILE, false);
}