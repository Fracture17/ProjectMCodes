//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_SOSTATUSMODULEIMPL_H
#define PROJECTMCODES_SOSTATUSMODULEIMPL_H


#include "../../../Globals/stddef.h"
#include "soModuleAccessor.h"
#include "Containers/ArrayVector.h"

struct soModuleAccessor;

struct soTransitionModuleEntity_vtable {
    char** typeName;
    void* thisPtrFixup;
    void** fn__checkEstablish;
    void** fn__enableTerm;
    void** fn__unableTerm;
    void** fn__enableTermAll;
    void** fn__unableTermAll;
    void** fn__enableTermGroup;
    void** fn__unableTermGroup;
    void** fn__isEnableTermGroup;
    void** fn__addTerm;
    void** fn__addGeneralTerm;
    void** fn__addGeneralTermLastTerm;
    void** fn__clearTransitionTermAll;
    void** fn__notifyEventAnimCmd;
    void** fn__getLastTransitionInfo;
    void** fn____dt;
};

struct soTransitionModuleEntity {
    VTABLE_METHOD(soTransitionModuleEntity*, void, checkEstablish)
    VTABLE_METHOD(soTransitionModuleEntity*, void, enableTerm)
    VTABLE_METHOD(soTransitionModuleEntity*, void, unableTerm)
    VTABLE_METHOD(soTransitionModuleEntity*, void, enableTermAll)
    VTABLE_METHOD(soTransitionModuleEntity*, void, unableTermAll)
    VTABLE_METHOD(soTransitionModuleEntity*, void, enableTermGroup)
    VTABLE_METHOD(soTransitionModuleEntity*, void, unableTermGroup)
    VTABLE_METHOD(soTransitionModuleEntity*, void, isEnableTermGroup)
    VTABLE_METHOD(soTransitionModuleEntity*, void, addTerm)
    VTABLE_METHOD(soTransitionModuleEntity*, void, addGeneralTerm)
    VTABLE_METHOD(soTransitionModuleEntity*, void, addGeneralTermLastTerm)
    VTABLE_METHOD(soTransitionModuleEntity*, void, clearTransitionTermAll)
    VTABLE_METHOD(soTransitionModuleEntity*, void, notifyEventAnimCmd)
    VTABLE_METHOD(soTransitionModuleEntity*, void, getLastTransitionInfo)
    VTABLE_METHOD(soTransitionModuleEntity*, void, __dt)

    soTransitionModuleEntity_vtable* vtable;

    char _spacer[0xC - 0x0 - 4];
    // 0xC
    int lastTransitionInfo1;
    int lastTransitionInfo2;
    int lastTransitionInfo3;
};

/**
 * soStatusUniqProcesses are essentially vtables that contain functions
 * which direct how a status (known by modders as "actions") are performed.
 * 
 * In most cases, these just use standard PSA as usual. Each of the functions within
 * are effectively dummied out, meaning nothing happens. However, others, such as 
 * "ftDonkeyStatusUniqProcessItemLift," use C++ code to override some of them.
 * 
 * This allows for complex interactions rather than relying on pure PSA to get
 * things done.
 * 
 * This is often seen for final smashes, or as previously mentioned, DK's "item lift"
 * (which corresponds to action 0x97, used when the character picks up a heavy item).
 */
struct soStatusUniqProcess {
    char*** objDescriptor;
    void* thisPtrFixup;

    void** __dt;

    void** initStatus;
    void** exitStatus;
    void** execStatus;
    void** execStop;
    void** execMapCorrection;
    void** execFixPosCounter;
    void** execFixPos;
    void** execFixCamera;
    void** checkDamage;
    void** checkAttack;
    void** onChangeLr;
    void** leaveStop;
    void** checkTransitionPrecede;
};

struct soStatusModuleImpl_vtable {
    char*** objDescriptor;
    void* thisPtrFixup;

    void** fn__soNullable$$isNull;
    void** fn__activate;
    void** fn__deactivate;
    void** fn__changeStatusRequest;
    void** fn__processFixPosition;
    void** fn__begin;
    void** fn__execStatus;
    void** fn__execStop;
    void** fn__execMapCorrection;
    void** fn__execFixPosCounter;
    void** fn__execFixPos;
    void** fn__execFixCamera;
    void** fn__checkDamage;
    void** fn__checkAttack;
    void** fn__onChangeLr;
    void** fn__leaveStop;
    void** fn__getStatusKind;
    void** fn__isCollisionAttackOccer;
    void** fn__isCollisionAttackTarget;
    void** fn__startWatchChange;
    void** fn__isChanged;
    void** fn__checkTransition;
    void** fn__enableTransitionTermGroup;
    void** fn__unableTransitionTermGroup;
    void** fn__isEnableTransitionTermGroup;
    void** fn__enableTransitionTermAll;
    void** fn__clearTransitionTermAll;
    void** fn__getLastStatusTransitionInfo;
    void** fn__addRangeUniqProc;
    void** fn__getStatusName;
    void** fn__getStatusName1;
    void** fn__getStatusGroundCorrect;
    void** fn__getPrevStatusKind;
    void** fn__connectStatusDataList;
    void** fn__changeStatusForce;
    void** fn__unableTransitionTerm;
    void** fn__setUniqProc;
    void** fn____dt;
    void** fn__changeStatus;
};

struct soStatusModuleImpl {
    void changeStatusForce(u32 action, soModuleAccessor* accesser);
    void changeStatusRequest(u32 action, soModuleAccessor* accesser);
    void enableTransitionTermAll();

    // VTABLE_METHOD(soStatusModuleImpl*, void, soNullable$$isNull)
    VTABLE_METHOD(soStatusModuleImpl*, void, activate)
    VTABLE_METHOD(soStatusModuleImpl*, void, deactivate)
    // VTABLE_METHOD(soStatusModuleImpl*, void, changeStatusRequest)
    VTABLE_METHOD(soStatusModuleImpl*, void, processFixPosition)
    VTABLE_METHOD(soStatusModuleImpl*, void, begin)
    VTABLE_METHOD(soStatusModuleImpl*, void, execStatus)
    VTABLE_METHOD_1ARG(soStatusModuleImpl*, void, execStop, struct soModuleAccessor*, accesser)
    VTABLE_METHOD(soStatusModuleImpl*, void, execMapCorrection)
    VTABLE_METHOD(soStatusModuleImpl*, void, execFixPosCounter)
    VTABLE_METHOD(soStatusModuleImpl*, void, execFixPos)
    VTABLE_METHOD(soStatusModuleImpl*, void, execFixCamera)
    VTABLE_METHOD(soStatusModuleImpl*, void, checkDamage)
    VTABLE_METHOD(soStatusModuleImpl*, void, checkAttack)
    VTABLE_METHOD(soStatusModuleImpl*, void, onChangeLr)
    VTABLE_METHOD(soStatusModuleImpl*, void, leaveStop)
    VTABLE_METHOD(soStatusModuleImpl*, void, getStatusKind)
    VTABLE_METHOD(soStatusModuleImpl*, void, isCollisionAttackOccer)
    VTABLE_METHOD(soStatusModuleImpl*, void, isCollisionAttackTarget)
    VTABLE_METHOD(soStatusModuleImpl*, void, startWatchChange)
    VTABLE_METHOD(soStatusModuleImpl*, void, isChanged)
    VTABLE_METHOD(soStatusModuleImpl*, void, checkTransition)
    VTABLE_METHOD_1ARG(soStatusModuleImpl*, void, enableTransitionTermGroup, int, groupID)
    VTABLE_METHOD_1ARG(soStatusModuleImpl*, void, unableTransitionTermGroup, int, groupID)
    VTABLE_METHOD(soStatusModuleImpl*, void, isEnableTransitionTermGroup)
    // VTABLE_METHOD(soStatusModuleImpl*, void, enableTransitionTermAll)
    VTABLE_METHOD(soStatusModuleImpl*, void, clearTransitionTermAll)
    VTABLE_METHOD(soStatusModuleImpl*, void, getLastStatusTransitionInfo)
    VTABLE_METHOD(soStatusModuleImpl*, void, addRangeUniqProc)
    VTABLE_METHOD(soStatusModuleImpl*, char*, getStatusName)
    VTABLE_METHOD_1ARG(soStatusModuleImpl*, char*, getStatusName1, int, statusID)
    VTABLE_METHOD(soStatusModuleImpl*, void, getStatusGroundCorrect)
    VTABLE_METHOD(soStatusModuleImpl*, void, getPrevStatusKind)
    VTABLE_METHOD(soStatusModuleImpl*, void, connectStatusDataList)
    // VTABLE_METHOD(soStatusModuleImpl*, void, changeStatusForce)
    VTABLE_METHOD(soStatusModuleImpl*, void, unableTransitionTerm)
    VTABLE_METHOD(soStatusModuleImpl*, void, setUniqProc)
    VTABLE_METHOD(soStatusModuleImpl*, void, __dt)
    VTABLE_METHOD(soStatusModuleImpl*, void, changeStatus)

    soStatusModuleImpl_vtable* vtable;

    char _spacer[0x02];

    // 0x06
    u16 previousAction;

    char _spacer2[0x2C - 0x06 - 2];
    // 0x2C
    soTransitionModuleEntity* transitionModuleEntity;

    // 0x30
    ArrayVector<soStatusUniqProcess>* uniqProcessArrayVec;

    // 0x34
    u32 action;
    // 0x38
    u32 desiredAction;

    char _spacer3[0x78 - 0x34 - 4];
    
    // 0x7F
    bool attackHasConnected;
};

#define _changeStatusForce_soStatusModuleImpl ((void (*)(soStatusModuleImpl* self, u32 actionID, soModuleAccessor* accesser)) 0x8077f8b0)
#define _changeStatusRequest_soStatusModuleImpl ((void (*)(soStatusModuleImpl* self, u32 actionID, soModuleAccessor* accesser)) 0x8077f5a8)
#define _enableTransitionTermAll_soStatusModuleImpl ((void (*)(soStatusModuleImpl* self)) 0x807807b4)


#endif //PROJECTMCODES_SOSTATUSMODULEIMPL_H
