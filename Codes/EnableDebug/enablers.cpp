#include "Assembly.h"
#include "Memory.h"

INJECTION("forceVisMemPool_0", 0x80025dc8, "cmpwi r3, 69");
INJECTION("forceVisMemPool_1", 0x80025e94, "cmpwi r3, 69");

INJECTION("physics_something_0", 0x800818a8, "cmpw r0, r0");