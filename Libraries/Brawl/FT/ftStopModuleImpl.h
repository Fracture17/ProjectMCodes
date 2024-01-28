//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_FTSTOPMODULEIMPL_H
#define PROJECTMCODES_FTSTOPMODULEIMPL_H

#include "stddef.h"

struct ftStopModuleImpl_vtable {
    char*** objDescriptor;
    void* thisPtrFixup;
    
    void** fn__setSpecialStop;
    void** fn____dt;
    void** fn__activate;
    void** fn__end;
    void** fn__setHitStop;
    void** fn__cancelHitStop;
    void** fn__setOtherStop;
    void** fn__cancelOtherStop;
    void** fn__setLinkStop;
    void** fn__isStop;
    void** fn__isSpecialStop;
    void** fn__isHit;
    void** fn__isDamage;
    void** fn__update;
    void** fn__getHitStopFrame;
    void** fn__getHitStopRealFrame;
    void** fn__setHitStopFrame;
    void** fn__setHitStopFrameFix;
};

struct ftStopModuleImpl {
    VTABLE_METHOD(ftStopModuleImpl*, void, setSpecialStop)
    VTABLE_METHOD(ftStopModuleImpl*, void, __dt)
    VTABLE_METHOD(ftStopModuleImpl*, void, activate)
    VTABLE_METHOD(ftStopModuleImpl*, void, end)
    VTABLE_METHOD(ftStopModuleImpl*, void, setHitStop)
    VTABLE_METHOD(ftStopModuleImpl*, void, cancelHitStop)
    VTABLE_METHOD(ftStopModuleImpl*, void, setOtherStop)
    VTABLE_METHOD(ftStopModuleImpl*, void, cancelOtherStop)
    VTABLE_METHOD(ftStopModuleImpl*, void, setLinkStop)
    VTABLE_METHOD(ftStopModuleImpl*, void, isStop)
    VTABLE_METHOD(ftStopModuleImpl*, void, isSpecialStop)
    VTABLE_METHOD(ftStopModuleImpl*, void, isHit)
    VTABLE_METHOD(ftStopModuleImpl*, void, isDamage)
    VTABLE_METHOD(ftStopModuleImpl*, void, update)
    VTABLE_METHOD(ftStopModuleImpl*, int, getHitStopFrame)
    VTABLE_METHOD(ftStopModuleImpl*, int, getHitStopRealFrame)
    VTABLE_METHOD_2ARG(ftStopModuleImpl*, void, setHitStopFrame, int, targetFrame, int, unkZero)
    VTABLE_METHOD_1ARG(ftStopModuleImpl*, void, setHitStopFrameFix, int, targetFrame)

    ftStopModuleImpl_vtable* vtable;
    char _spacer[0xC];

    // 0x10
    int hitlagMaybe;
    
    // 0x14
    int hitStopFrameFix;
};


#endif //PROJECTMCODES_FTSTOPMODULEIMPL_H
