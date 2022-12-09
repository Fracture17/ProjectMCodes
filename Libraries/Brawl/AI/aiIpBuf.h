#ifndef PROJECTMCODES_AIIPBUF_H
#define PROJECTMCODES_AIIPBUF_H

#include "Containers/Vec2f.h"
#include "Brawl/IP/Inputs.h"

struct AiIpBuf {
  Vec2f sticks[0x10];
  struct AiIpBufBtn {
    short _dummy;
    Inputs inputs;
  } buttonBuffer[0x10];
  int currOffset;
};

#endif // PROJECTMCODES_AIIPBUF_H