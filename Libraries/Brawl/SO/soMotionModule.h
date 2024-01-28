//
// Created by johno on 7/15/2020.
//

#ifndef PROJECTMCODES_SOMOTIONMODULE_H
#define PROJECTMCODES_SOMOTIONMODULE_H

struct CHR0 {
    char* getString() {
        auto thisINT = reinterpret_cast<unsigned int>(this);
        return (char*) (thisINT + this->stringOffset);
    }

    char _spacer[0x14];
    unsigned int stringOffset;
    char _spacer2[0x1C - 0x14 - 4];
    unsigned short animLength;
    
};

struct AnmObjChrRes {
    int* vtable;
    char _spacer[0x2C - 4];
    // 0x2C
    CHR0* CHR0Ptr;
};

struct soAnimChr {
    AnmObjChrRes* resPtr;
    // 0x4
    float animFrame;
    char _spacer[0x10 - 0x4 - 4];
    // 0x10
    float frameSpeedModifier;
};

// struct soTransitionModuleImpl_vtable {
//     char*** objDescriptor;
//     void* thisPtrFixup;

//     void** fn__checkEstablish;
//     void** fn__enableTerm;
//     void** fn__unableTerm;
//     void** fn__enableTermAll;
//     void** fn__unableTermAll;
//     void** fn__enableTermGroup;
//     void** fn__unableTermGroup;
//     void** fn__isEnableTermGroup;
//     void** fn__addTerm;
//     void** fn__addGeneralTerm;
//     void** fn__addGeneralTermLastTerm;
//     void** fn__clearTransitionTermAll;
//     void** fn__notifyEventAnimCmd;
//     void** fn__getLastTransitionInfo;
//     void** fn____dt;
// };

// struct soTransitionModuleImpl {
//     VTABLE_METHOD(void, init)
//     VTABLE_METHOD(void, checkEstablish)
//     VTABLE_METHOD(void, enableTerm)
//     VTABLE_METHOD(void, unableTerm)
//     VTABLE_METHOD(void, enableTermAll)
//     VTABLE_METHOD(void, unableTermAll)
//     VTABLE_METHOD(void, enableTermGroup)
//     VTABLE_METHOD(void, unableTermGroup)
//     VTABLE_METHOD(void, isEnableTermGroup)
//     VTABLE_METHOD(void, addTerm)
//     VTABLE_METHOD(void, addGeneralTerm)
//     VTABLE_METHOD(void, addGeneralTermLastTerm)
//     VTABLE_METHOD(void, clearTransitionTermAll)
//     VTABLE_METHOD(void, notifyEventAnimCmd)
//     VTABLE_METHOD(void, getLastTransitionInfo)
//     VTABLE_METHOD(void, __dt)

//     soTransitionModuleImpl_vtable* vtable;
// };

struct soMotionChangeParam {
    int kind;
    float frame;
    float rate;
    int SPACER;
};

struct soMotionModule {
    float getFrame();
    float getEndFrame();
    void changeMotionRequest(soMotionChangeParam* params);

    char _spacer[0x2C];
    // 0x2C
    float direction;

    char _spacer2[0x3C - 0x2C - 4];
    // 0x3C
    soAnimChr mainAnimationData;

    char _spacer3[0x58 - 0x3C - sizeof(soAnimChr)];
    // 0x58
    int subAction;

    char _spacer4[0xf0 - 0x58 - 4];

    // 0xf0
    // soTransitionModuleImpl* transitionModuleImpl;
    void* transitionModuleImpl;

    char _spacer5[0x168 - 0xf0 - 4];
    // 0x168
    class soModuleAccessor* accesser;
};


#define _getFrame_soMotionModule ((float (*)(soMotionModule* self)) 0x8071f474)
#define _changeMotionRequest_soMotionModule ((void (*)(soMotionModule* self, soMotionChangeParam* params)) 0x80726cf8)
#define _getEndFrame_soMotionModule ((float (*)(soMotionModule* self)) 0x8071f714)

#endif //PROJECTMCODES_SOMOTIONMODULE_H
