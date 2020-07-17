import re
import sys
import os


startupFunctionsTemplate = """
.globl __STARTUP_FUNCTIONS__
	.type __STARTUP_FUNCTIONS__, @function
__STARTUP_FUNCTIONS__:
    SAVE_REGS
    bl initMemory
    b _INITIALIZER_LIST_
_POST_INITIALIZER_LIST_:
    {functionCalls}
    RESTORE_REGS
    blr
    nop
    .size __STARTUP_FUNCTIONS__, .-__STARTUP_FUNCTIONS__
"""


initializerListTemplate = """
asm(R"(
.globl _INITIALIZER_LIST_
    .type _INITIALIZER_LIST_, @function
_INITIALIZER_LIST_:
    {initializerPlaceholders}
    b _POST_INITIALIZER_LIST_
    .size _INITIALIZER_LIST_, .-_INITIALIZER_LIST_
)");

asm(R"({startupFunctions})");
"""


injectionsFileTemplate = """
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef long long s64;
typedef float f32;
typedef double f64;

typedef unsigned int size_t;

asm(R"(
.set sp, 1;  .set rtoc, 2;
.set r0,0;   .set r1,1;   .set r2,2; .set r3,3;   .set r4,4
.set r5,5;   .set r6,6;   .set r7,7;   .set r8,8;   .set r9,9
.set r10,10; .set r11,11; .set r12,12; .set r13,13; .set r14,14
.set r15,15; .set r16,16; .set r17,17; .set r18,18; .set r19,19
.set r20,20; .set r21,21; .set r22,22; .set r23,23; .set r24,24
.set r25,25; .set r26,26; .set r27,27; .set r28,28; .set r29,29
.set r30,30; .set r31,31;
.set f0,0;   .set f1,1;   .set f2,2; .set f3,3;   .set f4,4
.set f5,5;   .set f6,6;   .set f7,7;   .set f8,8;   .set f9,9
.set f10,10; .set f11,11; .set f12,12; .set f13,13; .set f14,14
.set f15,15; .set f16,16; .set f17,17; .set f18,18; .set f19,19
.set f20,20; .set f21,21; .set f22,22; .set f23,23; .set f24,24
.set f25,25; .set f26,26; .set f27,27; .set f28,28; .set f29,29
.set f30,30; .set f31,31;
)");

asm(R"(.macro SAVE_REGS
	stw r0, -4(sp)
	mflr r0
	bl saveRegs
.endm)");

asm(R"(.macro RESTORE_REGS
	bl restoreRegs
	mtlr r0
	lwz r0, -4(sp)
.endm)");


//injection list must come before any functions, since getInjections.py checks the first function for the list
asm(".globl _INJECTION_LIST_");
asm("_INJECTION_LIST_:");
asm(".int {numInjections}");
asm(R"({injections})");
asm(".size _INJECTION_LIST_, .-_INJECTION_LIST_");

//must come immediatly after injection list
{initializerList}

asm(".globl _start");
asm("_start:");
    asm("bl _INJECTION_LIST_");
    //asm("bl _INITIALIZER_LIST_");
    
"""


def makeInjectionsFile(nmDirectory: str, injectionsPath, isInitializer: bool):
    injections, startups, numInitializers = getAllInjections(nmDirectory)

    #if not isInitializer:
        #removes startup injection
        #injections = injections[:-1]

    numInjections = len(injections)
    injectionInfo = [(branch, injectionAddress) for branch, injectionAddress, *_ in injections]
    injectionInfo = '\n'.join(f"b {branch}\n.int {injectionAddress}"
                              for branch, injectionAddress in injectionInfo)

    startupCalls = '\n'.join('bl ' + func for func in startups)
    startupFunctions = startupFunctionsTemplate.format(functionCalls=startupCalls)

    #placeholder for actual function calls
    #can't know address until it actually is linked
    initializerPlaceholders = '\n'.join([".int 0"] * numInitializers)

    if isInitializer:
        initializerList = initializerListTemplate.format(initializerPlaceholders=initializerPlaceholders,
                                                         startupFunctions=startupFunctions)
    else:
        initializerList = ''

    x = makeInjectionCode(numInjections, injectionInfo, initializerList)
    writeCodeToFile(x, injectionsPath)


def getAllInjections(nmDirectory: str):
    allInjections = []
    allStartups = []
    numInitializers = 0
    for nmFile in os.listdir(nmDirectory):
        with open(f"{nmDirectory}/{nmFile}", 'r', encoding='utf-8') as file:
            text = file.read()

            injections = getInjections(text)
            allInjections.extend(injections)

            startups = getStartups(text)
            allStartups.extend(startups)

            print(getInitializers(text))
            numInitializers += len(getInitializers(text))

    #allInjections.append(('__STARTUP_FUNCTIONS__', '0x8002d50c'))

    return allInjections, allStartups, numInitializers


def getInjections(text):
    return re.findall(r'([a-zA-Z0-9_]*_INJECTION_((0x)?[0-9a-fA-F]{8}))', text)


def getStartups(text):
    return re.findall(r'[a-zA-Z0-9_]*_STARTUP_', text)

def getInitializers(text):
    return re.findall(r"_GLOBAL__sub_I_*", text)


def makeInjectionCode(numInjections, injections, initializerList):
    return injectionsFileTemplate.format(numInjections=numInjections,
                                         injections=injections,
                                         initializerList=initializerList)


def writeCodeToFile(code, filePath):
    with open(filePath, 'w+') as file:
        file.write(code)


