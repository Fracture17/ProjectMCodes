#ifndef PROJECTMCODES_SOEFFECTMODULEIMPL_H
#define PROJECTMCODES_SOEFFECTMODULEIMPL_H

#include "stddef.h"
#include "containers/Vec3f.h"

struct soEffectModuleImpl_vtable {
  char*** objDescriptor;
  void* thisPtrFixup;
  
  void** fn__isNull;
  void** fn____dt;
  void** fn__activate;
  void** fn__deactivate;
  void** fn__begin;
  void** fn__update;
  void** fn__req;
  void** fn__req2D;
  void** fn__req_13;
  void** fn__req_14;
  void** fn__reqAttachGround;
  void** fn__reqFollow;
  void** fn__reqContinual;
  void** fn__removeContinual;
  void** fn__removeAllContinual;
  void** fn__reqTime;
  void** fn__reqTimeFollow;
  void** fn__removeTime;
  void** fn__removeAllTime;
  void** fn__reqEmit;
  void** fn__remove;
  void** fn__removeAll;
  void** fn__kill;
  void** fn__killKind;
  void** fn__killAll;
  void** fn__reqAfterImage;
  void** fn__removeAfterImage;
  void** fn__removeAllAfterImage;
  void** fn__setPos;
  void** fn__setRot;
  void** fn__setScale;
  void** fn__isExistEffect;
  void** fn__setVisible;
  void** fn__setWhole;
  void** fn__reqPause;
  void** fn__fillScreen;
  void** fn__clearScreen;
  void** fn__getDeadEffectRotZ;
  void** fn__reqCommon;
  void** fn__removeCommon;
  void** fn__resetCommon;
  void** fn__isEndCommon;
  void** fn__isExistCommon;
  void** fn__reqScreen;
  void** fn__removeScreen;
  void** fn__setSyncVisibility;
  void** fn__isSyncVisibility;
  void** fn__setSyncScale;
  void** fn__isSyncScale;
  void** fn__setShieldEffect;
  void** fn__ignoreScaleZ;
};

struct soEffectModuleImpl {
  VTABLE_METHOD(soEffectModuleImpl*, void, isNull)
  VTABLE_METHOD(soEffectModuleImpl*, void, __dt)
  VTABLE_METHOD(soEffectModuleImpl*, void, activate)
  VTABLE_METHOD(soEffectModuleImpl*, void, deactivate)
  VTABLE_METHOD(soEffectModuleImpl*, void, begin)
  VTABLE_METHOD(soEffectModuleImpl*, void, update)
  
  void req(float scale, u32 effectId, Vec3f* position, Vec3f* rotation, int loggingSomething, int anmIndex) { return ((void (*)(soEffectModuleImpl* self, float scale, u32 effectId, Vec3f* position, Vec3f* rotation, int loggingSomething, int anmIndex)) this->vtable->fn__req2D)(this, scale, effectId, position, rotation, loggingSomething, anmIndex); }
  void req2D(float scale, u32 effectId, Vec3f* position, Vec3f* rotation, int loggingSomething) { return ((void (*)(soEffectModuleImpl* self, float scale, u32 effectId, Vec3f* position, Vec3f* rotation, int loggingSomething)) this->vtable->fn__req2D)(this, scale, effectId, position, rotation, loggingSomething); }

  void req_13(float scale, u32 effectId, int node, Vec3f* positionOffset, Vec3f* rotation, Vec3f* posRange, Vec3f* rotRange, bool inheritSomething, int loggingSomething) { return ((void (*)(soEffectModuleImpl* self, float scale, u32 effectId, int node, Vec3f* positionOffset, Vec3f* rotation, Vec3f* posRange, Vec3f* rotRange, bool inheritSomething, int loggingSomething)) this->vtable->fn__req_13)(this, scale, effectId, node, positionOffset, rotation, posRange, rotRange, inheritSomething, loggingSomething); }
  void req_14(float scale, u32 effectId, int node, int* collision_unk, Vec3f* position, Vec3f* rotation, Vec3f* posRange, Vec3f* rotRange, bool inheritSomething, int loggingSomething) { return ((void (*)(soEffectModuleImpl* self, float scale, u32 effectId, int node, int* collision_unk, Vec3f* position, Vec3f* rotation, Vec3f* posRange, Vec3f* rotRange, bool inheritSomething, int loggingSomething)) this->vtable->fn__req_14)(this, scale, effectId, node, collision_unk, position, rotation, posRange, rotRange, inheritSomething, loggingSomething); }
  
  // VTABLE_METHOD(soEffectModuleImpl*, void, req_14)
  VTABLE_METHOD(soEffectModuleImpl*, void, reqAttachGround)
  void reqFollow(float scale, u32 effectId, int node, Vec3f* positionOffset, Vec3f* rotation, bool parented, int loggingSomething, int effectIDAdd, int presetAnimIndex) { return ((void (*)(soEffectModuleImpl* self, float scale, u32 effectId, int node, Vec3f* positionOffset, Vec3f* rotation, bool parented, int loggingSomething, int effectIDAdd, int presetAnimIndex)) this->vtable->fn__reqFollow)(this, scale, effectId, node, positionOffset, rotation, parented, loggingSomething, effectIDAdd, presetAnimIndex); }
  // VTABLE_METHOD(soEffectModuleImpl*, void, reqFollow)
  VTABLE_METHOD(soEffectModuleImpl*, void, reqContinual)
  VTABLE_METHOD(soEffectModuleImpl*, void, removeContinual)
  VTABLE_METHOD(soEffectModuleImpl*, void, removeAllContinual)
  VTABLE_METHOD(soEffectModuleImpl*, void, reqTime)
  VTABLE_METHOD(soEffectModuleImpl*, void, reqTimeFollow)
  VTABLE_METHOD(soEffectModuleImpl*, void, removeTime)
  VTABLE_METHOD(soEffectModuleImpl*, void, removeAllTime)
  VTABLE_METHOD(soEffectModuleImpl*, void, reqEmit)
  VTABLE_METHOD(soEffectModuleImpl*, void, remove)
  VTABLE_METHOD(soEffectModuleImpl*, void, removeAll)
  VTABLE_METHOD(soEffectModuleImpl*, void, kill)
  VTABLE_METHOD(soEffectModuleImpl*, void, killKind)
  VTABLE_METHOD(soEffectModuleImpl*, void, killAll)
  VTABLE_METHOD(soEffectModuleImpl*, void, reqAfterImage)
  VTABLE_METHOD(soEffectModuleImpl*, void, removeAfterImage)
  VTABLE_METHOD(soEffectModuleImpl*, void, removeAllAfterImage)
  VTABLE_METHOD(soEffectModuleImpl*, void, setPos)
  VTABLE_METHOD(soEffectModuleImpl*, void, setRot)
  VTABLE_METHOD(soEffectModuleImpl*, void, setScale)
  VTABLE_METHOD(soEffectModuleImpl*, void, isExistEffect)
  VTABLE_METHOD(soEffectModuleImpl*, void, setVisible)
  VTABLE_METHOD(soEffectModuleImpl*, void, setWhole)
  VTABLE_METHOD(soEffectModuleImpl*, void, reqPause)
  VTABLE_METHOD(soEffectModuleImpl*, void, fillScreen)
  VTABLE_METHOD(soEffectModuleImpl*, void, clearScreen)
  VTABLE_METHOD(soEffectModuleImpl*, void, getDeadEffectRotZ)
  VTABLE_METHOD(soEffectModuleImpl*, void, reqCommon)
  VTABLE_METHOD(soEffectModuleImpl*, void, removeCommon)
  VTABLE_METHOD(soEffectModuleImpl*, void, resetCommon)
  VTABLE_METHOD(soEffectModuleImpl*, void, isEndCommon)
  VTABLE_METHOD(soEffectModuleImpl*, void, isExistCommon)
  VTABLE_METHOD(soEffectModuleImpl*, void, reqScreen)
  VTABLE_METHOD(soEffectModuleImpl*, void, removeScreen)
  VTABLE_METHOD(soEffectModuleImpl*, void, setSyncVisibility)
  VTABLE_METHOD(soEffectModuleImpl*, void, isSyncVisibility)
  VTABLE_METHOD(soEffectModuleImpl*, void, setSyncScale)
  VTABLE_METHOD(soEffectModuleImpl*, void, isSyncScale)
  VTABLE_METHOD(soEffectModuleImpl*, void, setShieldEffect)
  VTABLE_METHOD(soEffectModuleImpl*, void, ignoreScaleZ)

  soEffectModuleImpl_vtable* vtable;

};



#endif //PROJECTMCODES_SOEFFECTMODULEIMPL_H
