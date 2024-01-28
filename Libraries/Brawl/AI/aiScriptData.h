//
// Created by dareb on 7/18/2020.
//

#ifndef PROJECTMCODES_AIACT_H
#define PROJECTMCODES_AIACT_H

struct AICEPac;
struct AiInput;
struct AiStat;
// p1 @ 8128bbc0
// p1 curr_goal @ 8128bc14

// p2 @ 812ddbc0
// p2 act id @ 812ddc38
// p2 currInst @ 812ddc28 

// 926d58c0
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
    AiStat* aiStatPtr;

    // 0x78
    unsigned short aiScript;
    // 0x7A
    unsigned short nextScript;
    // 0x7C
    unsigned short intermediateCurrentAiScript;
    // 0x7E
    unsigned short intermediateNextAiScript;
    char _spacer3[0x94 - 0x7E - 2];

    // 0x94
    unsigned int timeToWait; 
    char _spacer4[0xAC - 0x94 - 4];
    // 0xAC
    unsigned int framesSinceScriptChanged;
};



#endif //PROJECTMCODES_AIACT_H
