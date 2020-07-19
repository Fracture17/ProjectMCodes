//
// Created by dareb on 7/18/2020.
//

#ifndef PROJECTMCODES_AIACT_H
#define PROJECTMCODES_AIACT_H

struct aiScriptValues {
};

struct aiAct {
    float variables[26];
    char _spacer[0xC];

    aiScriptValues* scriptValues;

    unsigned short aiScript;

    char _spacer2[0x30];
    unsigned int framesSinceScriptChanged;
};



#endif //PROJECTMCODES_AIACT_H
