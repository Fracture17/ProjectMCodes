//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_SOKINETICMODULEGENERICIMPL_H
#define PROJECTMCODES_SOKINETICMODULEGENERICIMPL_H

#include "stddef.h"
#include "Containers/Vec3f.h"
#include "Containers/Vec2f.h"
#include "Containers/ArrayVector.h"

struct soKineticEnergyNormal_vtable {
    char*** objDescriptor;
    void* thisPtrFixup;
    
    // soNullable::isNull
    void** fn__isNull;
    // soKineticEnergyNormal::updateEnergy;
    void** fn__updateEnergy;
    // soKineticEnergyNormal::getSpeed;
    void** fn__getSpeed;
    // soKineticEnergy::getSpeed3f;
    void** fn__getSpeed3f;
    // soKineticEnergyNormal::getRotation;
    void** fn__getRotation;
    // soKineticEnergyNormal::resetEnergy;
    void** fn__resetEnergy;
    // soKineticEnergyNormal::clearSpeed;
    void** fn__clearSpeed;
    // soKineticEnergyNormal::clearRotSpeed;
    void** fn__clearRotSpeed;
    // soKineticEnergyNormal::mulSpeed;
    void** fn__mulSpeed;
    // soKineticEnergyNormal::mulAccel;
    void** fn__mulAccel;
    // soKineticEnergyNormal::reflectSpeed;
    void** fn__reflectSpeed;
    // soKineticEnergyNormal::reflectAccel;
    void** fn__reflectAccel;
    // soKineticEnergyNormal::onConsiderGroundFriction;
    void** fn__onConsiderGroundFriction;
    // soKineticEnergyNormal::offConsiderGroundFriction;
    void** fn__offConsiderGroundFriction;
    // soKineticEnergyNormal::__dt;
    void** fn____dt;
    // soKineticEnergyNormal::init;
    void** fn__init;
};

struct soKineticEnergyNormal {
    VTABLE_METHOD(void, isNull)
    VTABLE_METHOD(void, updateEnergy)
    VTABLE_METHOD(xyDouble, getSpeed)
    VTABLE_METHOD(void, getSpeed3f)
    VTABLE_METHOD(void, getRotation)
    VTABLE_METHOD(void, resetEnergy)
    VTABLE_METHOD(void, clearSpeed)
    VTABLE_METHOD(void, clearRotSpeed)
    VTABLE_METHOD_1ARG(void, mulSpeed, Vec2f*, multiplier)
    VTABLE_METHOD(void, mulAccel)
    VTABLE_METHOD(void, reflectSpeed)
    VTABLE_METHOD(void, reflectAccel)
    VTABLE_METHOD(void, onConsiderGroundFriction)
    VTABLE_METHOD(void, offConsiderGroundFriction)
    VTABLE_METHOD(void, __dt)
    VTABLE_METHOD(void, init)

    soKineticEnergyNormal_vtable* vtable;

    char _spacer[0x8 - 4];

    // 0x8
    float xVel;

    // 0xC
    float yVel;
};

struct soKineticEnergyVector {
    char _spacer[0x10];
    ArrayVector<soKineticEnergyNormal*> energies;
};

// responsible for action/subaction motion data
struct ftKineticEnergyMotion : public soKineticEnergyNormal { };

struct soKineticModuleImpl_vtable {
    char** objDescriptor;
    void* thisPtrFixup;

    // soNullable::isNull
    void** fn__isNull;
    // soKineticModuleImpl::activate
    void** fn__activate;
    // soKineticModuleImpl::deactivate
    void** fn__deactivate;
    // soKineticModuleImpl::addEnergy
    void** fn__addEnergy;
    // soKineticModuleImpl::eraseEnergy
    void** fn__eraseEnergy;
    // soKineticModuleImpl::eraseEnergyAll
    void** fn__eraseEnergyAll;
    // soKineticModuleImpl::getEnergy
    void** fn__getEnergy;
    // soKineticModuleImpl::isContain
    void** fn__isContain;
    // soKineticModuleImpl::updateEnergy
    void** fn__updateEnergy;
    // soKineticModuleImpl::clearSpeedAll
    void** fn__clearSpeedAll;
    // soKineticModuleImpl::suspendEnergyAll
    void** fn__suspendEnergyAll;
    // soKineticModuleImpl::resumeEnergyAll
    void** fn__resumeEnergyAll;
    // soKineticModuleImpl::unableEnergyAll
    void** fn__unableEnergyAll;
    // soKineticModuleImpl::getSumSpeed
    void** fn__getSumSpeed;
    // soKineticModuleImpl::getSumSpeed3f
    void** fn__getSumSpeed3f;
    // soKineticModuleImpl::getSumRotation
    void** fn__getSumRotation;
    // soKineticModuleImpl::setConsiderGroundFriction
    void** fn__setConsiderGroundFriction;
    // soKineticModuleImpl::mulSpeed
    void** fn__mulSpeed;
    // soKineticModuleImpl::mulAccel
    void** fn__mulAccel;
    // soKineticModuleImpl::reflectSpeed
    void** fn__reflectSpeed;
    // soKineticModuleImpl::reflectAccel
    void** fn__reflectAccel;
    // soKineticModuleImpl::changeKinetic
    void** fn__changeKinetic;
    // soKineticModuleImpl::getKineticType
    void** fn__getKineticType;
};

enum KineticEnergyType {
    KIN_Motion = 0x0,
    KIN_Gravity,
    KIN_Controller,
    KIN_Stop,
    KIN_Damage,
    KIN_WindNormal,
    KIN_GroundMovement,
    KIN_Jostle
};

struct soModuleAccessor;
struct soKineticModuleImpl {
    VTABLE_METHOD(void, isNull)
    VTABLE_METHOD(void, activate)
    VTABLE_METHOD(void, deactivate)
    VTABLE_METHOD(void, addEnergy)
    VTABLE_METHOD_1ARG(void, eraseEnergy, int, index)
    VTABLE_METHOD(void, eraseEnergyAll)
    VTABLE_METHOD_1ARG(soKineticEnergyNormal*, getEnergy, int, index)
    VTABLE_METHOD_1ARG(void, isContain, soKineticEnergyNormal_vtable**, kineticEnergyInstance)
    VTABLE_METHOD(void, updateEnergy)
    VTABLE_METHOD(void, clearSpeedAll)
    VTABLE_METHOD(void, suspendEnergyAll)
    VTABLE_METHOD(void, resumeEnergyAll)
    VTABLE_METHOD(void, unableEnergyAll)
    // 1 args
    VTABLE_METHOD(void, getSumSpeed)
    // 2 args
    VTABLE_METHOD(void, getSumSpeed3f)
    // 1 args
    VTABLE_METHOD(void, getSumRotation)
    // 2 args
    VTABLE_METHOD(void, setConsiderGroundFriction)
    // 2 args (vec2f, whichEnergies)
    VTABLE_METHOD_2ARG(void, mulSpeed, Vec2f, muls, int, energies)
    // 2 args (vec2f, whichEnergies)
    VTABLE_METHOD(void, mulAccel)
    // 2 args (vec2f, whichEnergies)
    VTABLE_METHOD(void, reflectSpeed)
    VTABLE_METHOD(void, reflectAccel)
    VTABLE_METHOD(void, changeKinetic)
    VTABLE_METHOD(void, getKineticType)

    soKineticModuleImpl_vtable* vtable;

    char _spacer[0x14 - 4];

    // 0x14 - might be soKineticModuleImpl
    soKineticEnergyVector* kineticEnergyVector;
    // 0x18
    char kineticType;

    char _spacer2[0x1C - 0x18 - 1];
    // 0x1C
    soModuleAccessor* moduleAccessor;

    char _spacer3[0xf4 - 0x1C - 4];

    // 0xf4
    ftKineticEnergyMotion energyMotion;
    char _spacer_motion[0x144 - 0xf4 - sizeof(ftKineticEnergyMotion)];
};

static_assert(offsetof(soKineticModuleImpl,energyMotion) == 0xf4, "energyMotion is not at 0xF4");
#define _changeKinetic_soKineticModuleGenericImpl ((void (*)(soKineticModuleGenericImpl* self, int kind, soModuleAccessor* accessor)) 0x807c1890)
#define _clearSpeed_soKineticModuleGenericImpl ((void (*)(soKineticModuleGenericImpl* self, int* clearType)) 0x8079227c)
#define _addSpeed_soKineticModuleGenericImpl ((void (*)(soKineticModuleGenericImpl* self, Vec3f* setOpts, soModuleAccessor* accessor)) 0x807c1b24)


#endif //PROJECTMCODES_SOKINETICMODULEGENERICIMPL_H
