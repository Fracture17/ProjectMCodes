//
// Created by dareb on 8/5/2020.
//

#ifndef PROJECTMCODES_AISTAT_H
#define PROJECTMCODES_AISTAT_H

#include "aiIpBuf.h"
#include "Containers/Vec3f.h"
#include "Brawl/FT/Fighter.h"

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
struct FtInput;
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

    char _spacer1[0x48 - 0x3 - 1];
    // 0x48
    FtInput* ftInput;
    // 0x4C
    AiInput* aiInput;
    
    // 0x50
    union FloorInfoUnion {
        unsigned int raw;
        struct FloorInfoBits {
            bool isOnStage : 1;
            bool unk31 : 1;
            bool unk30 : 1;
            bool isNotTouch_xa4_0 : 1;
            bool isNotTouch_x52_0 : 1;
            bool isNotTouch_1_0 : 1;
            bool unk26 : 1;
            bool DamageElec : 1;
            
            bool unk24 : 1;
            bool DamageElec_2 : 1;
            bool unk22 : 1;
            bool unk21 : 1;
            bool LABit_x18 : 1;
            bool isGrabbing : 1;
            bool unk18 : 1;
            bool Fighter_x138_bit_4 : 1;

            bool unk16 : 1;
            bool unk15 : 1;
            bool unk14 : 1;
            bool unk13 : 1;
            bool unk12 : 1;
            bool unk11 : 1;
            bool unk10 : 1;
            bool unk9 : 1;

            bool unk8 : 1;
            bool unk7 : 1;
            bool unk6 : 1;
            bool unk5 : 1;
            bool unk4 : 1;
            bool unk3 : 1;
            bool offStage : 1;
            bool isTouchingGround : 1;
        } infos;
    } floorInfoBitfield;

    // 0x54
    union TransitionUnion {
        unsigned int raw;
        struct TransitionBits {
            bool unk32 : 1;
            bool unk31 : 1;
            bool unk30 : 1;
            bool unk29 : 1;
            bool unk28 : 1;
            bool unk27 : 1;
            bool unk26 : 1;
            bool unk25 : 1;
            
            bool unk24 : 1;
            bool unk23 : 1;
            bool unk22 : 1;
            bool unk21 : 1;
            bool unk20 : 1;
            bool unk19 : 1;
            bool unk18 : 1;
            bool unk17 : 1;

            bool stat_air_free_1 : 1;
            bool stat_air_free_2 : 1;
            bool unk14 : 1;
            bool unk13 : 1;
            bool stat_air_free_3 : 1;
            bool unk11 : 1;
            bool unk10 : 1;
            bool unk9 : 1;

            bool stat_gr_free_1 : 1;
            bool unk7 : 1;
            bool unk6 : 1;
            bool stat_gr_free_2 : 1;
            bool stat_gr_free_3 : 1;
            bool unk3 : 1;
            bool unk2 : 1;
            bool stat_gr_free_4 : 1;
        } transitions;
    } transitionBitfield;

    // 0x58
    union ChrStatUnion {
        unsigned int raw;
        struct ChrStatBits {
            bool unk32 : 1;
            bool unk31 : 1;
            bool unk30 : 1;
            bool unk29 : 1;
            bool unk28 : 1;
            bool unk27 : 1;
            bool unk26 : 1;
            bool unk25 : 1;
            
            bool unk24 : 1;
            bool unk23 : 1;
            bool unk22 : 1;
            bool unk21 : 1;
            bool unk20 : 1;
            bool unk19 : 1;
            bool unk18 : 1;
            bool unk17 : 1;

            bool unk16 : 1;
            bool unk15 : 1;
            bool unk14 : 1;
            bool unk13 : 1;
            bool unk12 : 1;
            bool unk11 : 1;
            bool unk10 : 1;
            bool unk9 : 1;

            bool unk8 : 1;
            bool unk7 : 1;
            bool unk6 : 1;
            bool unk5 : 1;
            bool unk4 : 1;
            bool unk3 : 1;
            bool unk2 : 1;
            bool unk1 : 1;
        } chrStats;
    } chrStatBitfield;

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

    // 0xA8
    char _spacer_a8;
    char _spacer_a9;

    // 0xAA
    char comboedCounter;

    //0xAB
    char _spacer_aa_ac;

    // 0xAC
    CHAR_ID character;

    // 0xB0
    CHAR_ID kirbyHatCharacter;

    // 0xB4
    int currAction;

    // 0xBC
    // aka currBlk
    // void* currFloorPtr

    char _spacer6[0xC4 - 0xB4 - 4];
    // 0xC4
    float damage;
    // 0xC8
    float team;
    // 0xCC
    float teamPoints;
    // 0xD0
    float stockCount;

    // 0xD4
    char playerNumber;
    // 0xD8
    char _spacer7[0xDC - 0xD4 - 1];

    // 0xDC
    float LAFloat3;

    // 0xE0
    StageObject* stageObject;

    // 0xE4
    AiIpBuf ipBuf;

    char _spacer_fill[0x1BA - 0xE4 - sizeof(AiIpBuf)];

    // char _spacer9[0x1ba - 0x1a8 - 1];
    
    // 0x1BA
    char aiActionCategory;
    // 0x1BB
    char respawnInvincLeft;
    // 0x1BC
    u8 attackAnimFrame;
    // 0x1BD
    u8 attackAnimFrameAgain;
};

static_assert(offsetof(AiStat, character) == 0xAC, "improper character offset!!");

#define _calcArraivePosX_aiStat ((double (*)(double time, AiStat * stat)) 0x80916884)
#define _calcArraivePosY_aiStat ((double (*)(double time, AiStat * stat)) 0x809168c8)
#define _AIACTIONCAT_TABLE ((AiActionCategory[0x116]) 0x80b979cc)

#endif //PROJECTMCODES_AISTAT_H
