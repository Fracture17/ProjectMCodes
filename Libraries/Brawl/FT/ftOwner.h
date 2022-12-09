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
    char _spacer[0x110 - 4];
    // 0x110
    int ftKind;
    char _spacer2[0xC40 - 0x110 - 4];

//    char _spacer2[0x9E8 - 0x110 - 4];
//
//    int* _moveset;
//    int currentMove
//
//    char _spacer3[0xC4C - 0x10C - 4];

    // 0xC40
    int currStaleQueueNum;
    char _spacer3[0xC50 - 0xC40 - 4];

    // 0xC50
    StaleMoveEntry staleMoveQueue[9];
};


struct AiInput;
struct FtOwner {
    double getDamage();
    void setDamage(double newValue, int shouldLog);
    bool isCpu();
    bool hasCurry();

    // 0x0
    FtOwnerData* ownerDataPtr;
    // 0x4
    AiInput* aiInputPtr;

    char _spacer[0x4d0 - 0x4 - 0x4];
    // 0x4d0
    float slipMul;

    char _spacer2[0x4e0 - 0x4d0 - 0x4];
    // 0x4e0
    char slipInterval;

    // Seal (Sticker) fields

    char _spacer3[0x83C - 0x4e0 - 0x1];
    // 0x83C
    float slipMulSeal;
};

#define _getDamage_ftOwner ((double (*)(FtOwner * owner)) 0x8081c264)
#define _setDamage_ftOwner ((void (*)(double newValue, FtOwner * owner, int shouldLog)) 0x8081bdcc)
#define _isOperationCpu_ftOwner ((bool (*)(FtOwner * self)) 0x8081bdb0)
#define _getCurry_ftOwner ((bool (*)(FtOwner * self)) 0x8081c874)

#endif //PROJECTMCODES_FTOWNER_H
