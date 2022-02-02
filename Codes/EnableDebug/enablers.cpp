#include "Assembly.h"
#include "Memory.h"

INJECTION("forceVisMemPool-0", 0x80025dc8, "cmpwi r3, 69");
INJECTION("forceVisMemPool-1", 0x80025e94, "cmpwi r3, 69");

INJECTION("physics_something-0", 0x800818a8, "cmplr r0, r0");

