//
// Created by dareb on 7/25/2020.
//

#ifndef PROJECTMCODES_FTOWNER_H
#define PROJECTMCODES_FTOWNER_H

struct StaleMoveEntry {
    unsigned int num;
    unsigned int attack;
    unsigned int didConnect;
};

struct FtOwnerData {
    // 0x0
    int team;

    char _spacer1[0x1c - 4];

    // 0x1C
    union {
        unsigned char bits;
        struct {
            //tap jump bit is set forever if tap jump is set
            unsigned b07 : 1;
            unsigned b06 : 1;
            unsigned hasCurry : 1;
            unsigned b04 : 1;
            unsigned b03 : 1;
            unsigned b02 : 1;
            unsigned b01 : 1;
            unsigned b00 : 1;
        } flags;
    } flags_1c;

    char _spacer2[0x110 - 0x1C - 4];
    // 0x110
    int ftKind;
    char _spacer3[0xC40 - 0x110 - 4];

//    char _spacer3[0x9E8 - 0x110 - 4];
//
//    int* _moveset;
//    int currentMove
//
//    char _spacer4[0xC4C - 0x10C - 4];

    // 0xC40
    int currStaleQueueNum;
    char _spacer4[0xC50 - 0xC40 - 4];

    // 0xC50
    StaleMoveEntry staleMoveQueue[9];
};


struct AiInput;
struct FtOwner {
    double getDamage();
    void setDamage(double newValue, int shouldLog);
    bool isCpu();
    bool hasCurry();
    void setCurry(bool curryState);

    // 0x0
    FtOwnerData* ownerDataPtr;
    // 0x4
    AiInput* aiInputPtr;

    char _spacer[0x4a0 - 0x4 - 0x4];
    // 0x4a0
    float scale;

    char _spacer2[0x4d0 - 0x4a0 - 0x4];
    // 0x4d0
    float slipMul;

    char _spacer3[0x4e0 - 0x4d0 - 0x4];
    // 0x4e0
    char slipInterval;

    // Seal (Sticker) fields

    char _spacer4[0x83C - 0x4e0 - 0x1];
    // 0x83C
    float slipMulSeal;
};

#define _getDamage_ftOwner ((double (*)(FtOwner * owner)) 0x8081c264)
#define _setDamage_ftOwner ((void (*)(double newValue, FtOwner * owner, int shouldLog)) 0x8081bdcc)
#define _isOperationCpu_ftOwner ((bool (*)(FtOwner * self)) 0x8081bdb0)
#define _getCurry_ftOwner ((bool (*)(FtOwner * self)) 0x8081c874)
#define _setCurry_ftOwner ((bool (*)(FtOwner * self, bool curryState)) 0x8081c860)

#endif //PROJECTMCODES_FTOWNER_H
