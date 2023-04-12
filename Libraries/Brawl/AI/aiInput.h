//
// Created by johno on 7/17/2020.
//

#ifndef PROJECTMCODES_AIINPUT_H
#define PROJECTMCODES_AIINPUT_H

#include "Brawl/IP/IpHuman.h"
#include "Brawl/AI/aiParam.h"

struct AiInput;
struct AiScriptData;
struct FtEntry;

// p2 @ 812ddd00
// size: 0x274
struct AiInput {
    IpHuman inputs;

    char _spacer3[0x2C - sizeof(IpHuman)];

    // 0x2C
    int fighterId;
    // 0x30
    int charId;
    // 0x34
    AiInput* childAi;

    // 0x38
    void* pacData;
    
    // 0x3C
    union VariousAIInfoUnion {
        unsigned short raw;
        struct VariousAIInfos {
            bool isHumanPlayer : 1;
            bool unk15 : 1;
            bool unk14 : 1;
            bool unk13 : 1;
            bool unk12 : 1;
            bool unk11 : 1;
            bool unk10 : 1;
            bool unk9 : 1;

            // change_md => 0
            bool isChildAi : 1;
            bool unk7 : 1; // pacRegist/[aiInput] if this is on then unk5 and unk6 get toggled off, otherwise this gets toggled on
            // change_md => 0
            bool unk6 : 1; // pacRegist/[aiInput]
            bool unk5 : 1; // pacRegist/[aiInput]
            bool unk4 : 1;
            // change_md => 0
            bool unk3 : 1;
            // change_md => 0
            bool unk2 : 1;
            bool unk1 : 1;
        } infos;
    } infoUnion;

    // contains ARC data
    char _spacer4[0x40 - 0x3C - 2];

    // 0x40
    char swingChkByte1;
    // 0x41
    char swingChkByte2;
    // 0x42
    char cpuIdx;
    char _spacer5[0x44 - 0x42 - 1];

    // 0x44
    AiScriptData* aiActPtr;

    // 0x48
    int aiMd;
    char _spacer6[0x58 - 0x48 - 4];

    // 0x58
    short aiThing;
    short unk_5A;
    short unk_5C;
    // 0x5E
    short recoveryTimer;
    char _spacer7[0x84 - 0x5E - 2];

    // 0x84
    int aiItemTarget;

    char _spacer8[0x90 - 0x84 - 4];
    // 0x90
    AiParam aiParam;

    char _spacer9[0x110 - 0x90 - sizeof(AiParam)];
    // 0x110
    char aiTarget;
    char _spacer10[0x144 - 0x110 - 1];

    // 0x144
    FtEntry* ftEntryPtr;
};

static_assert(sizeof(AiInput) == 0x148);

#endif //PROJECTMCODES_AIINPUT_H
