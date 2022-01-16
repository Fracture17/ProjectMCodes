//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_SOKINETICMODULEGENERICIMPL_H
#define PROJECTMCODES_SOKINETICMODULEGENERICIMPL_H

#include "Containers/Vec3f.h"

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
    char kineticType;
};

#define _changeKinetic_soKineticModuleGenericImpl ((void (*)(soKineticModuleGenericImpl* self, int kind, soModuleAccessor* accessor)) 0x807c1890)
#define _clearSpeed_soKineticModuleGenericImpl ((void (*)(soKineticModuleGenericImpl* self, int* clearType)) 0x8079227c)
#define _addSpeed_soKineticModuleGenericImpl ((void (*)(soKineticModuleGenericImpl* self, Vec3f* setOpts, soModuleAccessor* accessor)) 0x807c1b24)


#endif //PROJECTMCODES_SOKINETICMODULEGENERICIMPL_H
