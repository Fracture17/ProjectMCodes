//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_SOMODULEACCESSOR_H
#define PROJECTMCODES_SOMODULEACCESSOR_H


#include "StageObject.h"
#include "soControllerModuleImpl.h"
#include "soWorkManageModuleImpl.h"


struct StageObject;

//Contains references to all modules in a StageObject
//The "so" prefix is the base module class
//Modules like ft are derived from the StageObject class
struct soModuleAccessor {
    char _spacer[8];
    //0x8
    StageObject* owner;
    //There should be an soModuleEnummeration class here, but I'm ommiting it for convienence
    char _spacer2[0x18 - 0xC];
    //0xC + 0xC
    //soPostureModuleImpl* postureModule;
    char _spacer3[0x68 - 0x1C + 4];
    //0x5C + 0xC
    soControllerModuleImpl* controllerModule;
    char _spacer4[0x70 - 0x6C];
    //0x64 + 0xC
    soWorkManageModuleImpl* workModule;
    //char _spacer5[0x7C - 0x74];
    //0x70 + 0xC
    //soStatusModuleImpl* statusModule;
};


#endif //PROJECTMCODES_SOMODULEACCESSOR_H
