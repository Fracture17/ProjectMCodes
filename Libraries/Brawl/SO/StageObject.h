//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_STAGEOBJECT_H
#define PROJECTMCODES_STAGEOBJECT_H

#include "Brawl/GF/gfTask.h"

struct soModuleAccessor;
struct StageObject : public gfTask {
    //a bunch of arguments
    inline static auto ct = (StageObject* (*)()) 0x8070e274;

    char _spacer_so[0x60 - sizeof(gfTask)];
    //0x60
    soModuleAccessor* modules;
    
};

#define _updatePosture_StageObject ((void (*)(StageObject* self, bool interpolate)) 0x8070ef84)

#endif //PROJECTMCODES_STAGEOBJECT_Hrec
