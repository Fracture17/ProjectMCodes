//
// Created by dareb on 7/18/2020.
//

#ifndef PROJECTMCODES_AIACT_H
#define PROJECTMCODES_AIACT_H

struct AICEPac;
struct AiInput;
struct AiStat;
// p2 @ 812ddbc0
// p2 act id @ 812ddc38
// p2 currInst @ 812ddc28
struct AiScriptData {
    // 0x00
    float variables[24];
    // 0x60
    AICEPac* AIScriptPac;
    
    // 0x64
    int* constPtr;

    // 0x68
    int* currentInstruction;

    char _spacer2[0x70 - 0x68 - 4];

    // 0x70
    AiInput* aiInputPtr;

    // 0x74
    AiStat* scriptValues;

    // 0x78
    unsigned short aiScript;
    // 0x7A
    unsigned short nextScript;
    // 0x7C
    unsigned short intermediateCurrentAiScript;
    // 0x7E
    unsigned short intermediateNextAiScript;
    char _spacer3[0xAC - 0x7E - 2];

    // 0xAC
    unsigned int framesSinceScriptChanged;
};



#endif //PROJECTMCODES_AIACT_H
