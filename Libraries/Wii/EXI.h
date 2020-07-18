//
// Created by johno on 7/17/2020.
//

#ifndef PROJECTMCODES_EXI_H
#define PROJECTMCODES_EXI_H

#include "stddef.h"


enum EXIChannel {
    slotA = 0,
    slotB = 1,
    serialPort2 = 2
};

typedef s32 (*EXICallback)(EXIChannel chn, s32 dev);

//

bool attachEXIDevice(EXIChannel channel, EXICallback callback=nullptr);


#define _EXIAttach ((bool (*)(EXIChannel channel, EXICallback callback)))
#define _EXILock (())

#endif //PROJECTMCODES_EXI_H
