cppText = """
#include "Setup.h"


struct Injection {
    unsigned int injectionAddress;
    unsigned int injectedFunctionAddress;
    unsigned int returnBranchAddress;
};

struct CodeFile {
    char name[12];
    unsigned int address;
};

struct SetupInfo {
    CodeFile fileNames[NUM_FILES];
    Injection injections[NUM_INJECTIONS];
    void (*startupFunction)();

};

asm(R"(
	.macro SAVE_REGS
    	stw 0, -4(1)
    	mflr 0
    	bl saveRegs
    .endm
    	.macro RESTORE_REGS
    	bl restoreRegs
    	mtlr 0
    	lwz 0, -4(1)
    .endm
)");

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
#define FAFRead ((int (*)(const void* destination, int size, int count, FILE* stream)) 0x803ebee4)
#define FAFClose ((int (*)(FILE* file)) 0x803ebe8c)

struct FILE {};


const char READ[] = "r";

void readFile(const char* name, void* destination) {
    auto file = FAFOpen(name, READ);
    FAFRead(destination, 1, 0x10000, file);
    FAFClose(file);
}

void readCodeFiles(SetupInfo& info) {
    for(auto& file: info.fileNames) {
        readFile(file.name, (void*)file.address);
    }
}

unsigned int makeBranchInstruction(unsigned int targetAddress, unsigned int startAddress) {
    int offset = targetAddress - startAddress;
    unsigned int maskedOffset = ((1 << 26) - 1) & offset;
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

void makeInjection(Injection& injection, bool shouldInvalidate) {
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

void makeInjectionBranches(SetupInfo& file, bool shouldInvalidate) {
    for(auto& injection: file.injections) {
        makeInjection(injection, shouldInvalidate);
    }
}

extern "C" void setup() {
    SetupInfo* info;
    asm(R"(
        lis %0, SETUP_INFO@ha
        ori %0, %0, SETUP_INFO@l)"
        : "=r" (info)
        );
    readCodeFiles(*info);
    makeInjectionBranches(*info, true);
    info->startupFunction();
}



asm(R"(
INJECT:
    SAVE_REGS
    bl inject
    RESTORE_REGS
    lwz 3, 0x10(3)
INJECT_END:
    nop
)");

extern "C" void inject() {
    SetupInfo* info;
    asm(R"(
        lis %0, SETUP_INFO@ha
        ori %0, %0, SETUP_INFO@l)"
        : "=r" (info)
        );
    makeInjectionBranches(*info, false);
}
"""


headerFormat = """
asm(R"(
.int setup

SETUP_INFO:
    #file names
    {fileNames}
    #injections
    {injections}
    .int 0x80023d54
    .int INJECT
    .int INJECT_END
    #startup function address
    {startupFunctionAddress}
)");

#define NUM_FILES ({numFiles})
#define NUM_INJECTIONS ({numInjections} + 1)
"""


fileNameFormat = """
1:
    .ascii "{name}"
    .zero (12-(.-1b))
    .int {address}
"""


injectionFormat = """
    .int {injectionAddress}
    .int {injectedFunctionAddress}
    .int {returnBranchAddress}
"""





def makeSetupFile(segments, injections, startupFunctionAddress, setupCPPPath, setupHeaderPath):
    with open(setupCPPPath, "w") as file:
        file.write(cppText)

    print(startupFunctionAddress)

    with open(setupHeaderPath, "w") as file:
        fileNamesText = []
        for segment in segments:
            fileNamesText.append(fileNameFormat.format(name=segment.filename, address=segment.startAddress))
        fileNamesText = ''.join(fileNamesText)

        injectionsText = []
        for injection in injections:
            injectionsText.append(injectionFormat.format(injectionAddress=injection[0], injectedFunctionAddress=injection[1], returnBranchAddress=injection[2]))
        injectionsText = ''.join(injectionsText)

        startupFunctionText = f".int {startupFunctionAddress}\n"

        headerText = headerFormat.format(fileNames=fileNamesText, injections=injectionsText, startupFunctionAddress=startupFunctionText, numFiles=len(segments), numInjections=len(injections))

        file.write(headerText)