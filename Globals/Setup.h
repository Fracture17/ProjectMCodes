//
// Created by johno on 11/26/2020.
//

#ifndef PROJECTMCODES_SETUP_H
#define PROJECTMCODES_SETUP_H

#include "Assembly.h"
#include "Memory.h"
#include "stddef.h"

//These structs are puposly inaccurate
//The arrays are not actually empty, they are created after compilation
//This is done because the true size is not known until then
//The location of each struct is immediatly after the previous, from top to bottom

struct MEMORY_HEAP_INFO {
    void* heapAddress;
    u32 heapSize;
};

struct INJECTIONS {
    struct _injection {
        u32 injectionAddress;
        u32 injectedFunctionAddress;
        u32 returnBranchAddress;
    };

    int numInjections;
    _injection injections[];
};

struct STARTUPS {
    typedef void (*startupFunction)();

    int numStartups;
    startupFunction startupFunctions[];
};





void performInjection(INJECTIONS::_injection& injection, bool shouldInvalidate);

#endif //PROJECTMCODES_SETUP_H
