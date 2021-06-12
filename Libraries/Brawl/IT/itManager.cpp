
#include "itManager.h"

#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)
BaseItem* itManager::getItemFromIndex(u32 index) {
    OSReport("here 1.1\n");
    OSReport("itMan addr: %08x\n", this);
    return _getItemFromIndex_itManager(this, index);
}