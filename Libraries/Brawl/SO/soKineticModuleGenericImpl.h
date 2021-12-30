//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_SOKINETICMODULEGENERICIMPL_H
#define PROJECTMCODES_SOKINETICMODULEGENERICIMPL_H

struct valueContainerThing {
    char _spacer[0x8];

    // 0x8
    float kbXVel;

    // 0xC
    float kbYVel;
};

struct soKineticModuleImplIntermediateThing {
    char _spacer[0x4C];

    valueContainerThing* valContThing;
};

struct soKineticModuleGenericImpl {
    char _spacer[0x14];

    // 0x14 - might be soKineticModuleImpl
    soKineticModuleImplIntermediateThing* intermediateThing;
};


#endif //PROJECTMCODES_SOKINETICMODULEGENERICIMPL_H
