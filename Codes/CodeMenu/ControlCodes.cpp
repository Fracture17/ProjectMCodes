

#include "Assembly.h"
#include "Memory.h"

int percent;

INJECTION("FixPercentSelector_1", 0x800e0ce4, R"(
  bl fixPercentSelectorImpl_1
)");

// constexpr const u8 fixedPercentFlag = 0x69;

extern "C" {
  void fixPercentSelectorImpl() {
    register s32 *fixedPercentFlag asm("r11");

    
    asm(R"(
        lfd f1, 0x00(%0)
        mr 12, %1
    )"
    :
    : "r" (&fixedPercentFlag), "r" (fn_shouldReturnResult));
  }
}