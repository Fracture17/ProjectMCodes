//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_FTPARAMCUSTOMIZEMODULEIMPL_H
#define PROJECTMCODES_FTPARAMCUSTOMIZEMODULEIMPL_H

#include "stddef.h"

struct ftParam {
    // 0
    float walkInitVel;
    // 1
    float walkAccel;
    // 2
    float walkMaxVel;
    // 3
    float groundFriction;
    // 4
    float dashInitVel;
    // 5
    float dashRunAccA;
    // 6
    float dashRunAccB;
    // 7
    float dashRunTermVel;
    // 8
    float runAccel_maybe;
    // 9
    float groundedMaxXVel;
    // a
    u32 dashCancelFrameWindow;
    // b
    float guardOnMaxMomentum;
    // c
    u32 jumpSquatFrames;
    // d
    float jumpXInitVel;
    // e
    float jumpYInitVel;
    // f
    float jumpXVelGroundMult;
    // 10
    float jumpXInitTermVel;
    // 11
    float jumpYInitVelShort;
    // 12
    float airJumpYMult;
    // 13
    float airJumpAdditionalHVel;
    // 14
    float footstoolInitVel;
    // 15
    float footstoolInitVelShort;
    // 16
    float footstoolCancelWindow;
    // 17
    float meteorCancelDelay;
    // 18
    u32 numJumps;
    // 19
    float gravity;
    // 1a
    float maxFallSpeed;
    // 1b
    float airFrictionY;
    // 1c
    float airYTermVel;
    // 1d
    float airMobilityA;
    // 1e
    float airMobilityB;
    // 1f
    float airXTermVel;
    // 20
    float airFrictionX;
    // 21
    float fastFallSpeed;
    // 22
    float airXTermVelHard;
    // 23
    u32 glideFrameWindow;
    // 24
    float unk_0x90;
    // 25
    float jab2Window;
    // 26
    float jab3Window;
    // 27
    float ftilt2Window;
    // 28
    float ftilt3Window;
    // 29
    float fsmash2Window;
    // 2a
    float flipDirFrame;
    // 2b
    float unk_0xac;
    // 2c
    float weight;
    // 2d
    float size;
    // 2e
    float resultsScreenSize;
    // 2f
    float unk_0xbc;
    // 30
    float unk_0xc0;
    // 31
    float shieldSize;
    // 32
    float shieldBreakVel;
    // 33
    float shieldStrength;
    // 34
    float unk_0xd0;
    // 35
    float respawnPlatformSize;
    // 36
    float unk_0xd8;
    // 37
    float unk_0xdc;
    // 38
    float unk_0xe0;
    // 39
    float edgeJumpXVel;
    // 3a
    u32 edgeJumpYVel;
    // 3b
    u32 unk_0xec;
    // 3c
    float unk_0xf0;
    // 3d
    float unk_0xf4;
    // 3e
    float unk_0xf8;
    // 3f
    float unk_0xfc;
    // 40
    float unk_0x100;
    // 41
    float itemThrowStrength;
    // 42
    float unk_0x108;
    // 43
    float unk_0x10c;
    // 44
    float unk_0x110;
    // 45
    float projectileItemMoveSpeed;
    // 46
    float projectileItemMoveSpeedDashF;
    // 47
    float projectileItemMoveSpeedDashB;
    // 48
    float unk_0x120;
    // 49
    float lightLandingLag;
    // 4a
    float normalLandingLag;
    // 4b
    float nairLandingLag;
    // 4c
    float fairLandingLag;
    // 4d
    float bairLandingLag;
    // 4e
    float uairLandingLag;
    // 4f
    float dairLandingLag;
    // 50
    float termVelHardFrames;
    // 51
    float hipNBone;
    // 52
    float tagHeightValue;
    // 53
    float unk_0x14c;
    // 54
    float walljumpXVel;
    // 55
    u32 walljumpYVel;
    // 56
    u32 unk_0x158;
    // 57
    float lhandNBone;
    // 58
    float rhandNBone;
    // 59
    float unk_0x164;
    // 5a
    float waterYAcc;
    // 5b
    float unk_0x16c;
    // 5c
    float unk_0x170;
    // 5d
    u32 unk_0x174;
    // 5e
    float unk_0x178;
    // 5f
    float unk_0x17c;
    // 60
    u32 spitStarSize;
    // 61
    u32 spitStarDamage;
    // 62
    float eggSize;
    // 63
    float unk_0x18c;
    // 64
    float unk_0x190;
    // 65
    float unk_0x194;
    // 66
    float unk_0x198;
    // 67
    float unk_0x19c;
    // 68
    float unk_0x1a0;
    // 69
    float unk_0x1a4;
    // 6a
    u32 hipNBone2;
    // 6b
    float unk_0x1ac;
    // 6c
    float unk_0x1b0;
    // 6d
    float unk_0x1b4;
    // 6e
    float unk_0x1b8;
    // 6f
    float xRotNBone;
    // 70
    float unk_0x1c0;
    // 71
    float unk_0x1c4;
    // 72
    float unk_0x1c8;
    // 73
    u32 unk_0x1cc;
    // 74
    float unk_0x1d0;
    // 75
    float cameraInitialYOffset;
    // 76
    float cameraSizeFront;
    // 77
    float cameraSizeBack;
    // 78
    u32 cameraSizeTop;
    // 79
    float cameraSizeBottom;
    // 7a
    float unk_0x1e8;
    // 7b
    float zoomCameraSizeFront;
    // 7c
    float zoomCameraSizeBack;
    // 7d
    float zoomCameraSizeTop;

    char _spacer[0xa2*4 - 0x7d*4 - 4];
    // a2
    float unk_0x288;
    // a3
    u32 unk_0x28c;
    // a4
    u32 unk_0x290;
    // a5
    u32 unk_0x294;
    // a6
    float unk_0x298;
    // a7
    u32 unk_0x29c;

    char _spacer2[0xb0*4 - 0xa7*4 - 4];

    // b0
    float unk_b0;
    // b1
    float unk_b1;
    // b2
    float unk_b2;
    // b3
    float unk_b3;
    // b4
    float unk_b4;
    // b5
    float unk_b5;
    // b6
    float unk_b6;
    // b7
    float unk_b7;
    // b8
    float unk_b8;
};

struct ftParamEtc {

};

struct ftParamCustomizeModuleImpl {
    // start
    char _spacer[0x18];
    // 0x18
    float walkInitVel;
    // 0x1c
    float walkAccel;
    // 0x20
    float walkMaxVel;
    // 0x24
    float groundFriction;
    // 0x28
    float dashInitVel;
    // 0x2c
    float dashRunAccA;
    // 0x30
    float dashRunAccB;
    // 0x34
    float dashRunTermVel;
    // 0x38
    float runAccel_maybe;
    // 0x3c
    float groundedMaxXVel;
    // 0x40
    float guardOnMaxMomentum;
    // 0x44
    float jumpXInitVel;
    // 0x48
    float jumpYInitVel;
    // 0x4c
    float jumpXVelGroundMult;
    // 0x50
    float jumpXInitTermVel;
    // 0x54
    float jumpYInitVelShort;
    // 0x58
    float airJumpYMult;
    // 0x5c
    float airJumpAdditionalHVel;
    // 0x60
    float footstoolInitVel;
    // 0x64
    float footstoolInitVelShort;
    // 0x68
    float footstoolCancelWindow;
    // 0x6c
    float meteorCancelDelay;
    // 0x70
    float gravity;
    // 0x74
    float maxFallSpeed;
    // 0x78
    float airFrictionY;
    // 0x7c
    float airYTermVel;
    // 0x80
    float airMobilityA;
    // 0x84
    float airMobilityB;
    // 0x88
    float airXTermVel;
    // 0x8c
    float airFrictionX;
    // 0x90
    float fastFallSpeed;
    // 0x94
    float airXTermVelHard;
    // 0x98
    float unk_0x90;
    // 0x9c
    float jab2Window;
    // 0xa0
    float jab3Window;
    // 0xa4
    float ftilt2Window;
    // 0xa8
    float ftilt3Window;
    // 0xac
    float fsmash2Window;
    // 0xb0
    float flipDirFrame;
    // 0xb4
    float unk_0xac;
    // 0xb8
    float weight;
    // 0xbc
    float size;
    // 0xc0
    float resultsScreenSize;
    // 0xc4
    float unk_0xbc;
    // 0xc8
    float unk_0xc0;
    // 0xcc
    float shieldSize;
    // 0xd0
    float shieldBreakVel;
    // 0xd4
    float shieldStrength;
    // 0xd8
    float unk_0xd0;
    // 0xdc
    float respawnPlatformSize;
    // 0xe0
    float unk_0xd8;
    // 0xe4
    float unk_0xdc;
    // 0xe8
    float unk_0xe0;
    // 0xec
    float edgeJumpXVel;
    // 0xf0
    float unk_0xf0;
    // 0xf4
    float unk_0xf4;
    // 0xf8
    float unk_0xf8;
    // 0xfc
    float unk_0xfc;
    // 0x100
    float unk_0x100;
    // 0x104
    float itemThrowStrength;
    // 0x108
    float unk_0x108;
    // 0x10c
    float unk_0x10c;
    // 0x110
    float unk_0x110;
    // 0x114
    float projectileItemMoveSpeed;
    // 0x118
    float projectileItemMoveSpeedDashF;
    // 0x11c
    float projectileItemMoveSpeedDashB;
    // 0x120
    float unk_0x120;
    // 0x124
    float lightLandingLag;
    // 0x128
    float normalLandingLag;
    // 0x12c
    float nairLandingLag;
    // 0x130
    float fairLandingLag;
    // 0x134
    float bairLandingLag;
    // 0x138
    float uairLandingLag;
    // 0x13c
    float dairLandingLag;
    // 0x140
    float termVelHardFrames;
    // 0x144
    float hipNBone;
    // 0x148
    float tagHeightValue;
    // 0x14c
    float unk_0x14c;
    // 0x150
    float walljumpXVel;
    // 0x154
    float lhandNBone;
    // 0x158
    float rhandNBone;
    // 0x15c
    float unk_0x164;
    // 0x160
    float waterYAcc;
    // 0x164
    float unk_0x16c;
    // 0x168
    float unk_0x170;
    // 0x16c
    float unk_0x178;
    // 0x170
    float unk_0x17c;
    // 0x174
    float eggSize;
    // 0x178
    float unk_0x18c;
    // 0x17c
    float unk_0x190;
    // 0x180
    float unk_0x194;
    // 0x184
    float unk_0x198;
    // 0x188
    float unk_0x19c;
    // 0x18c
    float unk_0x1a0;
    // 0x190
    float unk_0x1a4;
    // 0x194
    float unk_0x1ac;
    // 0x198
    float unk_0x1b0;
    // 0x19c
    float unk_0x1b4;
    // 0x1a0
    float unk_0x1b8;
    // 0x1a4
    float xRotNBone;
    // 0x1a8
    float unk_0x1c0;
    // 0x1ac
    float unk_0x1c4;
    // 0x1b0
    float unk_0x1c8;
    // 0x1b4
    float unk_0x1d0;
    // 0x1b8
    float cameraInitialYOffset;
    // 0x1bc
    float cameraSizeFront;
    // 0x1c0
    float cameraSizeBack;
    // 0x1c4
    float cameraSizeBottom;
    // 0x1c8
    float unk_0x1e8;
    // 0x1cc
    float zoomCameraSizeFront;
    // 0x1d0
    float zoomCameraSizeBack;
    // 0x1d4
    float zoomCameraSizeTop;
    // 0x1d8
    float unk_0x288;
    // 0x1dc
    float unk_0x298;
    // 0x1e0
    char _spacer2[0x3a8 - 0x1e0];
    // 0x3a8
    u32 dashCancelFrameWindow;
    // 0x3ac
    u32 jumpSquatFrames;
    // 0x3b0
    u32 numJumps;
    // 0x3b4
    u32 glideFrameWindow;
    // 0x3b8
    u32 edgeJumpYVel;
    // 0x3bc
    u32 unk_0xec;
    // 0x3c0
    u32 walljumpYVel;
    // 0x3c4
    u32 unk_0x158;
    // 0x3c8
    u32 unk_0x174;
    // 0x3cc
    u32 spitStarSize;
    // 0x3d0
    u32 spitStarDamage;
    // 0x3d4
    u32 hipNBone2;
    // 0x3d8
    u32 unk_0x1cc;
    // 0x3dc
    u32 cameraSizeTop;
    // 0x3e0
    u32 unk_0x28c;
    // 0x3e4
    u32 unk_0x290;
    // 0x3e8
    u32 unk_0x294;
    // 0x3ec
    u32 unk_0x29c;
    char _spacer3[0x690 - 0x3ec - 4];
    // 0x690
    float unk_690;

};































































































































































































































































#endif //PROJECTMCODES_FTPARAMCUSTOMIZEMODULEIMPL_H
