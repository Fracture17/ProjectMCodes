//
// Created by johno on 7/24/2020.
//

#ifndef PROJECTMCODES_GFFILEIOREQUEST_H
#define PROJECTMCODES_GFFILEIOREQUEST_H


#include "stddef.h"

struct gfFileIORequest {
    //0
    char* filePath;

    char _spacer[0x8 - 4];

    //0x8
    u32 size;

    char _spacer2[0x18 - 8 - 4];

    //0x18
    //bitflags
    //only most significant 20 seem to be used
    //0x00001000 is isReady flag
    //if 0x20000000 is set, and 0x40000000 is not set, will call setSyncCommand after setting isReady flag
    u32 params;

    //0x1C
    //CRC16
    u16 hash;
};

#define IS_READY_PARAM_GF_FILE_IO_REQUEST (0x1000u)

#endif //PROJECTMCODES_GFFILEIOREQUEST_H
