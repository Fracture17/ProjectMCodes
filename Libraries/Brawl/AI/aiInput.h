//
// Created by johno on 7/17/2020.
//

#ifndef PROJECTMCODES_AIINPUT_H
#define PROJECTMCODES_AIINPUT_H

#include "../IP/IpHuman.h"

struct aiInput: IpHuman {
    //won't work until IpHuman is correct size
    char _spacer[0x90 - sizeof(IpHuman)];
    //0x90
    //aiParam
};


#endif //PROJECTMCODES_AIINPUT_H
