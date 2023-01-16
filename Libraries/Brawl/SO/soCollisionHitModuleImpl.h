//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_SOCOLLISIONHITMODULEIMPL_H
#define PROJECTMCODES_SOCOLLISIONHITMODULEIMPL_H

#include "Containers/ArrayVector.h"

enum CollisionAttackHitState : int {
    NORMAL,
    INVINCIBLE,
    INTANGIBLE_FLASH,
    INTANGIBLE,
    INTANGIBLE_RAPID_FLASH
};

struct soCollisionHitGroup {
    char _spacer[0x20];

    // 0x20
    CollisionAttackHitState hitState;
    // 0x24
    int remainingIntangibility;
    // 0x28
    int remainingInvincibility;
    
    char _spacer2[0x31 - 0x28 - 4];

    // 0x31
    bool forceIntangibility;
    // 0x32
    bool forceInvincibility;

    char unk1;
    char unk2;
};

struct soCollisionHitModuleImpl {
    char _spacer[0x40];

    // 0x40
    float TopNX;

    // 0x44
    float TopNY;


    char _spacer2[0x58 - 0x44 - 4];

    // 0x58
    ArrayVector<soCollisionHitGroup>* mainHitGroup; 
};


#endif //PROJECTMCODES_SOCOLLISIONHITMODULEIMPL_H
