//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_STAGEOBJECT_H
#define PROJECTMCODES_STAGEOBJECT_H

#include "Brawl/GF/gfTask.h"

struct AiInput;
struct soModuleAccessor;
struct StageObject : public gfTask {
    AiInput* getInput();

    //a bunch of arguments
    inline static auto ct = (StageObject* (*)()) 0x8070e274;

    char _spacer_so[0x60 - sizeof(gfTask)];
    //0x60
    soModuleAccessor* modules;
    
};

#define _updatePosture_StageObject ((void (*)(StageObject* self, bool interpolate)) 0x8070ef84)
#define _updateRoughPos_StageObject ((void (*)(StageObject* self)) 0x807110b0)
#define _updateNodeSRT_StageObject ((void (*)(StageObject* self)) 0x8070f340)

#define _getInput_StageObject ((AiInput* (*)(StageObject* self)) 0x8070fcc8)

#endif //PROJECTMCODES_STAGEOBJECT_Hrec
