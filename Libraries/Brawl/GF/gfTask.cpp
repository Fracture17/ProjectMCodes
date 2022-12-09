//
// Created by fudgepop01 on 9/27/2022.
//

#include "gfTask.h"

#define _$_GF_TASK(FN_NAME) \
void gfTask::FN_NAME() { \
    return ((void (*)()) this->vtable->fn_ ## FN_NAME)(); \
}

_$_GF_TASK(processBegin)
_$_GF_TASK(processAnim)
_$_GF_TASK(processUpdate)
_$_GF_TASK(processPreMapCorrection)
_$_GF_TASK(processMapCorrection)
_$_GF_TASK(processFixPosition)
_$_GF_TASK(processPreCollision)
_$_GF_TASK(processCollision)
_$_GF_TASK(processCatch)
_$_GF_TASK(processHit)
_$_GF_TASK(processCamera)
_$_GF_TASK(processFixCamera)
_$_GF_TASK(processEffect)
_$_GF_TASK(processGameProc)
_$_GF_TASK(processEnd)
_$_GF_TASK(renderPre)
_$_GF_TASK(renderOpa)
_$_GF_TASK(renderXlu)
_$_GF_TASK(processDebug)
_$_GF_TASK(renderDebug)
_$_GF_TASK(init)
_$_GF_TASK(__dt)
