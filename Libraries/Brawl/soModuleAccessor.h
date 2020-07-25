//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_SOMODULEACCESSOR_H
#define PROJECTMCODES_SOMODULEACCESSOR_H


#include "StageObject.h"
#include "soControllerModuleImpl.h"
#include "soWorkManageModuleImpl.h"
#include "soPostureModuleImpl.h"
#include "soMotionModule.h"


struct StageObject;

//Contains references to all modules in a StageObject
//The "so" prefix is the base module class
//Modules like ft are derived from the StageObject class
struct soModuleAccessor {
    char _spacer[8];
    //0x8
    StageObject* owner;
    //There should be an soModuleEnummeration class here, but I'm just going to list the modules directly for now
    char _spacer2[0x14 - 0x8 - 4];

    //0x8 + 0xC = 0x14
    soMotionModule* motionModule;

    //0xC + 0xC = 0x18
    soPostureModuleImpl* postureModule;
    char _spacer3[0x68 - 0x18 - 4];
    //0x5C + 0xC = 0x64
    soControllerModuleImpl* controllerModule;
    char _spacer4[0x70 - 0x6C];
    //0x64 + 0xC = 0x70
    soWorkManageModuleImpl* workModule;
    //char _spacer5[0x7C - 0x74];
    //0x70 + 0xC = 0x7C
    //soStatusModuleImpl* statusModule;
};


#endif //PROJECTMCODES_SOMODULEACCESSOR_H
