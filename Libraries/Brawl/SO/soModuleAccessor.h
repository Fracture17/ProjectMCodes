//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_SOMODULEACCESSOR_H
#define PROJECTMCODES_SOMODULEACCESSOR_H

#include "Brawl/GF/gfTask.h"
#include "Brawl/SO/StageObject.h"
#include "soControllerModuleImpl.h"
#include "soDamageModuleImpl.h"
#include "soWorkManageModuleImpl.h"
#include "soPostureModuleImpl.h"
#include "soMotionModule.h"
#include "soStatusModuleImpl.h"
#include "soEffectModuleImpl.h"
#include "soCollisionAttackModuleImpl.h"
#include "soCollisionHitModuleImpl.h"
#include "soKineticModuleGenericImpl.h"
#include "soGroundModuleImpl.h"
#include "soGenerateArticleManageModuleImpl.h"
#include "soItemManageModuleImpl.h"
#include "../FT/ftParamCustomizeModuleImpl.h"
#include "../FT/ftStopModuleImpl.h"
#include "soColorBlendModule.h"
#include "soAnimCmdModuleImpl.h"
#include "soResourceModuleImpl.h"
#include "soTeamModuleImpl.h"


struct StageObject;
struct soGenerateArticleManageModuleImpl;
struct soStatusModuleImpl;

//Contains references to all modules in a StageObject
//The "so" prefix is the base module class
//Modules like ft are derived from the StageObject class
struct soModuleAccessor {
    char _spacer[8];
    //0x8
    StageObject* owningObject;
    //There should be an soModuleEnummeration class here, but I'm just going to list the modules directly for now
    //0x0 + 0xC = 0xC
    soResourceModuleImpl* resourceModule;
    char _spacer2[0x14 - 0xC - 4];

    //0x8 + 0xC = 0x14
    soMotionModule* motionModule;

    //0xC + 0xC = 0x18
    soPostureModuleImpl* postureModule;

    //0x10 + 0xC = 0x1C
    soGroundModuleImpl* groundModule;

    char _spacer3[0x24 - 0x1C - 4];
    //0x18 + 0xC = 0x24
    soTeamModuleImpl* teamModule;
    //0x1C + 0xC = 0x28
    soCollisionAttackModuleImpl* collisionAttackModule;
    //0x20 + 0xC = 0x2C
    soCollisionHitModuleImpl* collisionHitModule;

    //0x38 + 0xC = 0x44
    char _spacer4[0x44 - 0x2C - 4];
    soDamageModuleImpl* damageModuleImpl;

    char _spacer5[0x50 - 0x44 - 4];
    // 0x44 + 0xC = 0x50
    ftStopModuleImpl* ftStopModule;

    char _spacer6[0x68 - 0x50 - 4];
    //0x5C + 0xC = 0x68
    soControllerModuleImpl* controllerModule;
    char _spacer7[0x70 - 0x68 - 4];
    //0x64 + 0xC = 0x70
    soWorkManageModuleImpl* workModule;
    char _spacer8[0x78 - 0x70 - 4];
    // 0x6C + 0xC = 0x78
    soAnimCmdModuleImpl* animCmdModule;
    char _spacer9[0x7C - 0x78 - 4];
    //0x70 + 0xC = 0x7C
    soStatusModuleImpl* statusModule;
    char _spacer10[0x88 - 0x7C - 4];
    //0x7C + 0xC = 0x88
    soKineticModuleImpl* kineticModule;
    char _spacer11[0x90 - 0x88 - 4];
    //0x84 + 0xC = 0x90
    soGenerateArticleManageModuleImpl* generateArticleManageModule;
    //0x88 + 0xC = 0x94
    soEffectModuleImpl* effectModule;
    char _spacer12[0xB4 - 0x94 - 4];
    //0xA8 + 0xC = 0xB4
    soItemManageModuleImpl* itemManageModule;
    char _spacer13[0xB8 - 0xB4 - 4];
    //0xAC + 0xC = 0xB8
    soColorBlendModule* colorBlendModule;
    char _spacer14[0xD0 - 0xB8 - 4];
    //0xC4 + 0xC = 0xD0
    ftParamCustomizeModuleImpl* paramCustomizeModule;
    char _spacer15[4];
    // 0xCC + 0xC = 0xD8
    soModuleAccessor * moduleEnumeration;
};


#endif //PROJECTMCODES_SOMODULEACCESSOR_H
