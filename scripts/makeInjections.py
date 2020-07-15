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
#include "Assembly.h"

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


