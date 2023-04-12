//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_SOSTATUSMODULEIMPL_H
#define PROJECTMCODES_SOSTATUSMODULEIMPL_H


#include "../../../Globals/stddef.h"
#include "soModuleAccessor.h"
#include "Containers/ArrayVector.h"

struct soModuleAccessor;

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
    void* objDescriptor;
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

struct soStatusModuleImpl {
    void changeStatusForce(u32 action, soModuleAccessor* accesser);
    void changeStatusRequest(u32 action, soModuleAccessor* accesser);
    void enableTransitionTermAll();

    char _spacer[0x06];

    // 0x06
    u16 previousAction;

    char _spacer2[0x30 - 0x06 - 2];

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
