//
// Created by dareb on 7/18/2020.
//

#ifndef PROJECTMCODES_AIACT_H
#define PROJECTMCODES_AIACT_H

struct aiScriptValues {
};

struct aiAct {
    // 0x00
    float variables[26];
    char _spacer[0x74 - 0x68];

    // 0x74
    aiScriptValues* scriptValues;

    // 0x78
    unsigned short aiScript;
    char _spacer2[0xAC - 0x78 - 2];

    // 0xAC
    unsigned int framesSinceScriptChanged;
};



#endif //PROJECTMCODES_AIACT_H
