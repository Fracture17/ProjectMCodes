#ifndef PROJECTMCODES_SODAMAGEMODULEIMPL_H
#define PROJECTMCODES_SODAMAGEMODULEIMPL_H

#include "stddef.h"

enum soKind : int {
  StageObject_Fighter = 0x0,
  StageObject_Enemy = 0x1,
  StageObject_Weapon = 0x2,
  StageObject_Yakumono = 0x3,
  StageObject_Item = 0x4,
  StageObject_Invalid = 0x5
};

struct soAttackerInfo {
  char _spacer[0x4];
  int directAttackerTaskId;
  soKind directAttackerSoKind;
  int directAttackerSoSubKind;
};

struct soDamageModuleImpl_vtable {
    char*** objDescriptor;
    void* thisPtrFixup;
    
    void** fn__soNullable$$isNull;
    void** fn____dt;
    void** fn__activate;
    void** fn__deactivate;
    void** fn__sleep;
    void** fn__update;
    void** fn__initDamage;
    void** fn__initInfo;
    void** fn__setInfo;
    void** fn__storeDamage;
    void** fn__check;
    void** fn__getDamageLr;
    void** fn__setForceDamage;
    void** fn__setForceDamage1;
    void** fn__setForceDamage2;
    void** fn__addDamage;
    void** fn__getDamage;
    void** fn__getReaction;
    void** fn__getPowerMax;
    void** fn__checkNoReaction;
    void** fn__setNoReactionModeStatus;
    void** fn__setNoReactionMode2nd;
    void** fn__resetNoReactionModeStatus;
    void** fn__setNoReactionMode;
    void** fn__resetNoReactionMode;
    void** fn__setNoReactionModeAlways;
    void** fn__setNoReactionModePerfect;
    void** fn__isNoReactionModePerfect;
    void** fn__toTurnDamage;
    void** fn__leaveTurnDamage;
    void** fn__getDamageLog;
    void** fn__clearDamageLog;
    void** fn__getAttackerInfo;
    void** fn__getAttackerInfoDirect;
    void** fn__restoreAttackerInfo;
    void** fn__clearAttackerInfo;
    void** fn__getEffector;
    void** fn__getTransactor;
    void** fn__isReaction;
    void** fn__isCatchCut;
    void** fn__getCaptureDamage;
    void** fn__isCaptureCut;
    void** fn__isCaptureCut1;
    void** fn__setDamageMul;
    void** fn__setReactionMul;
    void** fn__setReactionMul2nd;
    void** fn__setDamageLock;
    void** fn__isDamageLock;
    void** fn__heal;
    void** fn__updateAttackerInfo;
};

struct soDamageModuleImpl {
  // VTABLE_METHOD(void, soNullable$$isNull)
  VTABLE_METHOD(soDamageModuleImpl*, void, __dt)
  VTABLE_METHOD(soDamageModuleImpl*, void, activate)
  VTABLE_METHOD(soDamageModuleImpl*, void, deactivate)
  VTABLE_METHOD(soDamageModuleImpl*, void, sleep)
  VTABLE_METHOD(soDamageModuleImpl*, void, update)
  VTABLE_METHOD(soDamageModuleImpl*, void, initDamage)
  VTABLE_METHOD(soDamageModuleImpl*, void, initInfo)
  VTABLE_METHOD(soDamageModuleImpl*, void, setInfo)
  VTABLE_METHOD(soDamageModuleImpl*, void, storeDamage)
  VTABLE_METHOD(soDamageModuleImpl*, void, check)
  VTABLE_METHOD(soDamageModuleImpl*, void, getDamageLr)
  VTABLE_METHOD(soDamageModuleImpl*, void, setForceDamage)
  VTABLE_METHOD(soDamageModuleImpl*, void, setForceDamage1)
  VTABLE_METHOD(soDamageModuleImpl*, void, setForceDamage2)
  VTABLE_METHOD(soDamageModuleImpl*, void, addDamage)
  VTABLE_METHOD(soDamageModuleImpl*, void, getDamage)
  VTABLE_METHOD(soDamageModuleImpl*, void, getReaction)
  VTABLE_METHOD(soDamageModuleImpl*, void, getPowerMax)
  VTABLE_METHOD(soDamageModuleImpl*, void, checkNoReaction)
  VTABLE_METHOD(soDamageModuleImpl*, void, setNoReactionModeStatus)
  VTABLE_METHOD(soDamageModuleImpl*, void, setNoReactionMode2nd)
  VTABLE_METHOD(soDamageModuleImpl*, void, resetNoReactionModeStatus)
  VTABLE_METHOD_1ARG(soDamageModuleImpl*, void, setNoReactionMode, int, mode)
  VTABLE_METHOD(soDamageModuleImpl*, void, resetNoReactionMode)
  VTABLE_METHOD(soDamageModuleImpl*, void, setNoReactionModeAlways)
  VTABLE_METHOD(soDamageModuleImpl*, void, setNoReactionModePerfect)
  VTABLE_METHOD(soDamageModuleImpl*, void, isNoReactionModePerfect)
  VTABLE_METHOD(soDamageModuleImpl*, void, toTurnDamage)
  VTABLE_METHOD(soDamageModuleImpl*, void, leaveTurnDamage)
  VTABLE_METHOD(soDamageModuleImpl*, void, getDamageLog)
  VTABLE_METHOD(soDamageModuleImpl*, void, clearDamageLog)
  VTABLE_METHOD(soDamageModuleImpl*, void, getAttackerInfo)
  VTABLE_METHOD(soDamageModuleImpl*, soAttackerInfo*, getAttackerInfoDirect)
  VTABLE_METHOD(soDamageModuleImpl*, void, restoreAttackerInfo)
  VTABLE_METHOD(soDamageModuleImpl*, void, clearAttackerInfo)
  VTABLE_METHOD(soDamageModuleImpl*, void, getEffector)
  VTABLE_METHOD(soDamageModuleImpl*, void, getTransactor)
  VTABLE_METHOD(soDamageModuleImpl*, void, isReaction)
  VTABLE_METHOD(soDamageModuleImpl*, void, isCatchCut)
  VTABLE_METHOD(soDamageModuleImpl*, void, getCaptureDamage)
  VTABLE_METHOD(soDamageModuleImpl*, void, isCaptureCut)
  VTABLE_METHOD(soDamageModuleImpl*, void, isCaptureCut1)
  VTABLE_METHOD(soDamageModuleImpl*, void, setDamageMul)
  VTABLE_METHOD(soDamageModuleImpl*, void, setReactionMul)
  VTABLE_METHOD(soDamageModuleImpl*, void, setReactionMul2nd)
  VTABLE_METHOD(soDamageModuleImpl*, void, setDamageLock)
  VTABLE_METHOD(soDamageModuleImpl*, void, isDamageLock)
  VTABLE_METHOD(soDamageModuleImpl*, void, heal)
  VTABLE_METHOD(soDamageModuleImpl*, void, updateAttackerInfo)

  soDamageModuleImpl_vtable* vtable;

  char _spacer[0x64];
  soAttackerInfo attackerInfo;

  char _spacer_attackerInfo[0x28 - sizeof(attackerInfo)];

};

struct soDamageModuleActor {
  char _spacer_damagesArr[0xac];
  soDamageModuleImpl damageModuleImpl;
};



#endif //PROJECTMCODES_SODAMAGEMODULEIMPL_H
