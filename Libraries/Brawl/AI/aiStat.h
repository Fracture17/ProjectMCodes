//
// Created by dareb on 8/5/2020.
//

#ifndef PROJECTMCODES_AISTAT_H
#define PROJECTMCODES_AISTAT_H

#include "aiIpBuf.h"
#include "Containers/Vec3f.h"

enum AI_ACT_CAT {
    ACAT_NONE = 0x0,	
    ACAT_IDLE_GROUND = 0x1,	
    ACAT_IDLE_AIR = 0x2,	
    ACAT_DASH_RUN = 0x3,	
    ACAT_TECH_OPTION = 0x4,	
    ACAT_HURT = 0x5,	
    ACAT_IDLE_LEDGE = 0x6,	
    ACAT_LEDGE_OPTION = 0x7,	
    ACAT_LADDER = 0x8,	
    ACAT_STUNNED = 0x9,	
    ACAT_IDLE_GRABBING = 0xa,	
    ACAT_THROWING = 0xb,	
    ACAT_WATER = 0xc,	
    ACAT_WALLCLING = 0xd,	
    ACAT_SPECIAL_FALL = 0xe,	
    ACAT_USING_DRAGOON = 0xf,	
    ACAT_GLIDE_CONTROL = 0x10,	
    ACAT_WARPSTAR_RIDE = 0x11,	
    ACAT_ATTACKING = 0x12,	
    ACAT_SMASH_START = 0x13,	
    ACAT_GRABBING = 0x14,	
    ACAT_SHIELDING = 0x15,	
    ACAT_JUMPSQUAT = 0x16,	
    ACAT_USELESS_CAUGHT = 0x17,	
    ACAT_ROLLING = 0x18
};

struct AiInput;
struct AiStat {
    double calcArrivePosX(double time);
    double calcArrivePosY(double time);    

    // 0x0
    char aiIndex;

    // 0x1
    char unk1;

    // 0x2
    bool weaponMarked;

    // 0x3
    bool enableAway;

    char _spacer1[0x4C - 0x3 - 1];
    // 0x4C
    AiInput* aiInput;
    
    // 0x50
    union FloorInfoUnion {
        unsigned int raw;
        struct FloorInfoBits {
            unsigned unk32 : 1;
            unsigned unk31 : 1;
            unsigned unk30 : 1;
            unsigned isNotTouch_xa4_0 : 1;
            unsigned isNotTouch_x52_0 : 1;
            unsigned isNotTouch_1_0 : 1;
            unsigned unk26 : 1;
            unsigned DamageElec : 1;
            
            unsigned unk24 : 1;
            unsigned DamageElec_2 : 1;
            unsigned unk22 : 1;
            unsigned unk21 : 1;
            unsigned LABit_x18 : 1;
            unsigned isGrabbing : 1;
            unsigned unk18 : 1;
            unsigned Fighter_x138_bit_4 : 1;

            unsigned unk16 : 1;
            unsigned unk15 : 1;
            unsigned unk14 : 1;
            unsigned unk13 : 1;
            unsigned unk12 : 1;
            unsigned unk11 : 1;
            unsigned unk10 : 1;
            unsigned unk9 : 1;

            unsigned unk8 : 1;
            unsigned unk7 : 1;
            unsigned unk6 : 1;
            unsigned unk5 : 1;
            unsigned unk4 : 1;
            unsigned unk3 : 1;
            unsigned offStage : 1;
            unsigned isTouchingGround : 1;
        } infos;
    };

    // 0x54
    union TransitionUnion {
        unsigned int raw;
        struct TransitionBits {
            unsigned unk32 : 1;
            unsigned unk31 : 1;
            unsigned unk30 : 1;
            unsigned unk29 : 1;
            unsigned unk28 : 1;
            unsigned unk27 : 1;
            unsigned unk26 : 1;
            unsigned unk25 : 1;
            
            unsigned unk24 : 1;
            unsigned unk23 : 1;
            unsigned unk22 : 1;
            unsigned unk21 : 1;
            unsigned unk20 : 1;
            unsigned unk19 : 1;
            unsigned unk18 : 1;
            unsigned unk17 : 1;

            unsigned unk16 : 1;
            unsigned unk15 : 1;
            unsigned unk14 : 1;
            unsigned unk13 : 1;
            unsigned unk12 : 1;
            unsigned unk11 : 1;
            unsigned unk10 : 1;
            unsigned unk9 : 1;

            unsigned unk8 : 1;
            unsigned unk7 : 1;
            unsigned unk6 : 1;
            unsigned unk5 : 1;
            unsigned unk4 : 1;
            unsigned unk3 : 1;
            unsigned unk2 : 1;
            unsigned unk1 : 1;
        } transitions;
    };

    // 0x58
    union ChrStatUnion {
        unsigned int raw;
        struct ChrStatBits {
            unsigned unk32 : 1;
            unsigned unk31 : 1;
            unsigned unk30 : 1;
            unsigned unk29 : 1;
            unsigned unk28 : 1;
            unsigned unk27 : 1;
            unsigned unk26 : 1;
            unsigned unk25 : 1;
            
            unsigned unk24 : 1;
            unsigned unk23 : 1;
            unsigned unk22 : 1;
            unsigned unk21 : 1;
            unsigned unk20 : 1;
            unsigned unk19 : 1;
            unsigned unk18 : 1;
            unsigned unk17 : 1;

            unsigned unk16 : 1;
            unsigned unk15 : 1;
            unsigned unk14 : 1;
            unsigned unk13 : 1;
            unsigned unk12 : 1;
            unsigned unk11 : 1;
            unsigned unk10 : 1;
            unsigned unk9 : 1;

            unsigned unk8 : 1;
            unsigned unk7 : 1;
            unsigned unk6 : 1;
            unsigned unk5 : 1;
            unsigned unk4 : 1;
            unsigned unk3 : 1;
            unsigned unk2 : 1;
            unsigned unk1 : 1;
        } chrStats;
    };

    // 0x5C
    Vec3f position;
    // 0x68
    Vec3f scale;
    // 0x74
    Vec3f topNPos;

    // 0x80
    float xVel;
    // 0x84
    float yVel;
    // 0x88
    float xAccel;
    // 0x8C
    float yAccel;

    char _spacer4[0xA0 - 0x8C - 4];
    // 0xA0
    float direction;
    // 0xA4
    float hurtboxSize;

    char _spacer5[0xAA - 0xA4 - 4];
    // 0xAA
    char comboedCounter;

    //0xAB
    char _spacer_aa_ac;

    // 0xAC
    int character;

    // 0xB4
    // int currStatus

    // 0xBC
    // aka currBlk
    // void* currFloorPtr

    char _spacer6[0xC4 - 0xAC - 4];
    // 0xC4
    float damage;
    // 0xC8
    float team;
    // 0xCC
    float teamPoints;
    // 0xD0
    float stockCount;

    // 0xD4
    // 0xD8
    char _spacer7[0xDC - 0xD0 - 4];

    // 0xDC
    float LAFloat3;

    char _spacer8[0xE4 - 0xDC - 4];

    // 0xE4
    AiIpBuf ipBuf;

    char _spacer_fill[0x1a8 - 0xE4 - sizeof(AiIpBuf)];

    char _spacer9[0x1ba - 0x1a8 - 1];
    
    // 0x1BA
    char aiActionCategory;
};

#define _calcArraivePosX_aiStat ((double (*)(double time, AiStat * stat)) 0x80916884)
#define _calcArraivePosY_aiStat ((double (*)(double time, AiStat * stat)) 0x809168c8)
#define _AIACTIONCAT_TABLE ((AiActionCategory[0x116]) 0x80b979cc)

#endif //PROJECTMCODES_AISTAT_H
