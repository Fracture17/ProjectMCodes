//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_SOGROUNDMODULEIMPL_H
#define PROJECTMCODES_SOGROUNDMODULEIMPL_H

#include "stddef.h"
#include <Containers/Vec3f.h>
#include <Containers/Vec2f.h>
#include <Containers/ArrayVector.h>

struct grCollShape_vtable {
    char*** objDescriptor;
    void* thisPtrFixup;
    
    void** fn__init;
    void** fn__getType;
    void** fn__getAABBox;
    void** fn__setDownPos;
    void** fn__setLeftPos;
    void** fn__setRightPos;
    void** fn__setCenterPos;
    void** fn__setTopPos;
    void** fn__getCenterPos;
    void** fn__getDownPos;
    void** fn__getLeftPos;
    void** fn__getRightPos;
    void** fn__getTopPos;
    void** fn__move;
    void** fn__set;
    void** fn__getHeight;
    void** fn__getWidth;
    void** fn__test;
    void** fn__updateAABBox;
    void** fn__setLR;
    void** fn__setUD;
    void** fn__setLR1;
    void** fn__modifyDownY;
};

struct grCollShape {
    VTABLE_METHOD(grCollShape*, void, init)
    VTABLE_METHOD(grCollShape*, void, getType)
    VTABLE_METHOD(grCollShape*, void, getAABBox)
    VTABLE_METHOD_1ARG(grCollShape*, void, setDownPos, Vec2f*, newPos)
    VTABLE_METHOD(grCollShape*, void, setLeftPos)
    VTABLE_METHOD(grCollShape*, void, setRightPos)
    VTABLE_METHOD(grCollShape*, void, setCenterPos)
    VTABLE_METHOD(grCollShape*, void, setTopPos)
    VTABLE_METHOD(grCollShape*, void, getCenterPos)
    VTABLE_METHOD(grCollShape*, void, getDownPos)
    VTABLE_METHOD(grCollShape*, void, getLeftPos)
    VTABLE_METHOD(grCollShape*, void, getRightPos)
    VTABLE_METHOD(grCollShape*, void, getTopPos)
    VTABLE_METHOD(grCollShape*, void, move)
    VTABLE_METHOD(grCollShape*, void, set)
    VTABLE_METHOD(grCollShape*, void, getHeight)
    VTABLE_METHOD(grCollShape*, void, getWidth)
    VTABLE_METHOD(grCollShape*, void, test)
    VTABLE_METHOD(grCollShape*, void, updateAABBox)
    VTABLE_METHOD(grCollShape*, void, setLR)
    VTABLE_METHOD(grCollShape*, void, setUD)
    VTABLE_METHOD(grCollShape*, void, setLR1)
    VTABLE_METHOD(grCollShape*, void, modifyDownY)
    
    grCollShape_vtable* vtable;
};

struct grCollStatus {
    char _spacer[0x60];
    // 0x60
    grCollShape* currCollShape;

    char _spacer2[0x6E - 0x60 - 4];

    // 0x6E
    char targetLineCategory;

    char _spacer3[0x75 - 0x6E - 1];
    // 0x75
    char touchFlags;
};



struct soGroundShapeImpl {
    STATIC_METHOD(0x8073922c, soGroundShapeImpl, void, __ct)
    STATIC_METHOD(0x8073929c, soGroundShapeImpl, void, __dt)
    STATIC_METHOD(0x8073930c, soGroundShapeImpl, void, clean)
    STATIC_METHOD(0x807393b4, soGroundShapeImpl, void, setup)
    STATIC_METHOD(0x80739484, soGroundShapeImpl, void, activate)
    STATIC_METHOD(0x807394fc, soGroundShapeImpl, void, deactivate)
    STATIC_METHOD(0x80739554, soGroundShapeImpl, void, initShape)
    STATIC_METHOD_1ARG(0x807395d0, soGroundShapeImpl, void, reset, Vec2f*, newPos)
    STATIC_METHOD(0x807396d4, soGroundShapeImpl, void, resetStatus)
    STATIC_METHOD(0x80739724, soGroundShapeImpl, void, clearSetting)
    STATIC_METHOD(0x80739834, soGroundShapeImpl, void, updateDownMaterialInfo)
    STATIC_METHOD(0x80739888, soGroundShapeImpl, void, setEventToGround)
    STATIC_METHOD(0x807398b0, soGroundShapeImpl, void, setTestCollStop)
    STATIC_METHOD(0x807398d8, soGroundShapeImpl, void, setTestPreMap)
    STATIC_METHOD(0x80739920, soGroundShapeImpl, void, setTestDynamicCollision)
    STATIC_METHOD(0x80739948, soGroundShapeImpl, void, setCalcLineMove)
    STATIC_METHOD(0x8073995c, soGroundShapeImpl, void, setTargetLineCategory)
    STATIC_METHOD(0x80739968, soGroundShapeImpl, void, setExclusiveLineAttr)
    STATIC_METHOD(0x80739974, soGroundShapeImpl, void, setPassableCheck)
    STATIC_METHOD(0x8073997c, soGroundShapeImpl, void, isPassableCheck)
    STATIC_METHOD(0x80739990, soGroundShapeImpl, void, setCloudThrough)
    STATIC_METHOD(0x807399a4, soGroundShapeImpl, void, isCloudThrough)
    STATIC_METHOD(0x807399b4, soGroundShapeImpl, void, isCloudThroughOut)
    STATIC_METHOD(0x807399d4, soGroundShapeImpl, void, setCorrectIgnoreSlope)
    STATIC_METHOD(0x807399e8, soGroundShapeImpl, void, enable)
    STATIC_METHOD(0x80739a04, soGroundShapeImpl, void, unable)
    STATIC_METHOD(0x80739a20, soGroundShapeImpl, void, setFlag)
    STATIC_METHOD(0x80739a58, soGroundShapeImpl, void, setSafePos)
    STATIC_METHOD(0x80739a78, soGroundShapeImpl, void, setSpeed)
    STATIC_METHOD(0x80739a80, soGroundShapeImpl, void, calcRhombusModify)
    STATIC_METHOD(0x80739ba4, soGroundShapeImpl, void, updateRhombusModify)
    STATIC_METHOD(0x8073a3b8, soGroundShapeImpl, void, updateRhombus)
    STATIC_METHOD(0x8073a624, soGroundShapeImpl, void, updateRhombusFollowNode)
    STATIC_METHOD(0x8073a8c8, soGroundShapeImpl, void, update)
    STATIC_METHOD(0x8073a958, soGroundShapeImpl, void, getCorrectPos)
    STATIC_METHOD(0x8073ab00, soGroundShapeImpl, void, attach)
    STATIC_METHOD(0x8073ab28, soGroundShapeImpl, bool, isAttach)
    STATIC_METHOD(0x8073ab44, soGroundShapeImpl, void, detach)
    STATIC_METHOD(0x8073ab68, soGroundShapeImpl, void, setAttackPoint)
    STATIC_METHOD(0x8073abb4, soGroundShapeImpl, void, getAttackPointBreak)
    STATIC_METHOD(0x8073abec, soGroundShapeImpl, void, getCrushRateUD)
    STATIC_METHOD(0x8073ac54, soGroundShapeImpl, void, getCrushRateLR)
    STATIC_METHOD(0x8073acbc, soGroundShapeImpl, void, modify)
    // STATIC_METHOD(0x8073accc, soGroundShapeImpl, void, modify)
    STATIC_METHOD(0x8073ace0, soGroundShapeImpl, void, getRhombus)
    // STATIC_METHOD(0x8073acfc, soGroundShapeImpl, void, getRhombus)
    STATIC_METHOD(0x8073ad20, soGroundShapeImpl, void, setTracerType)

    char _spacer[0x4];

    // 0x4
    // used in custom AI function 0x3D
    grCollStatus* collStatus;
    Vec2f safePos;
    Vec2f offset;
    // 0x8
    char airGroundState;
};

struct soGroundModuleImpl {
    double getDistanceFromUnderGrCol(double maxDistance, Vec3f * unk, bool unkBool);
    unsigned int isPassableCheck(int unk);
    unsigned int isPassableGround(int unk);
    int getCorrect();
    void setCorrect(int mode);
    xyDouble getLeftPos();
    xyDouble getRightPos();
    xyDouble getUpPos();
    xyDouble getDownPos();
    bool attachGround();
    void getCorrectPos(Vec3f* result);
    xyDouble getCorrectPos();
    // xyDouble getDownCorrectPos();
    void setShapeSafePos(xyDouble xyPos, int unk);
    void apply();

    char _spacer2[0x28];

    // 0x28
    ArrayVector<soGroundShapeImpl> * groundShapeImplVec;

//    char _spacer2[0x24 - 0x1C - 4];

    // 0x24
    // used in custom AI function 0x4B / 0x4C
//    int * unk2;
};

#define _getDistanceFromUnderGrCol_soGroundModuleImpl ((double (*)(double maxDistance, soGroundModuleImpl * self, Vec3f * position, bool unkBool)) 0x80734f8c)
#define _setGrCollisisonMode ((void (*)(soGroundModuleImpl * self, int collisionMode)) 0x80730a04)
#define _getCorrect_soGroundModuleImpl ((int (*)(soGroundModuleImpl * self, int index)) 0x80731730)
#define _setCorrect_soGroundModuleImpl ((void (*)(soGroundModuleImpl * self, int mode, int unk)) 0x80731324)
#define _isPassableGround_soGroundModuleImpl ((unsigned int (*)(soGroundModuleImpl * self, int unk)) 0x80731e44)
#define _isPassableCheck_soGroundModuleImpl ((unsigned int (*)(soGroundModuleImpl * self, int unk)) 0x80731b1c)

#define _apply_soGroundModuleImpl ((xyDouble (*)(soGroundModuleImpl * self)) 0x8073111c)
#define _getLeftPos_soGroundModuleImpl ((xyDouble (*)(soGroundModuleImpl * self, int unk)) 0x80733f9c)
#define _getRightPos_soGroundModuleImpl ((xyDouble (*)(soGroundModuleImpl * self, int unk)) 0x80733fe0)
#define _getUpPos_soGroundModuleImpl ((xyDouble (*)(soGroundModuleImpl * self, int unk)) 0x80733f14)
#define _getCorrectPos_soGroundModuleImpl ((void (*)(Vec3f* correctPos, soGroundModuleImpl * self, int unk)) 0x80731eb4)
#define _getDownPos_soGroundModuleImpl ((xyDouble (*)(soGroundModuleImpl * self, int unk)) 0x80733f58)
#define _attachGround_soGroundModuleImpl ((bool (*)(soGroundModuleImpl * self, int index)) 0x80734500)
// #define _getDownCorrectPos_soGroundModuleImpl ((xyDouble (*)(soGroundModuleImpl * self, int unk)) 0x80734184)

#define _setShapeSafePos_soGroundModule ((void (*)(soGroundModuleImpl* self, xyDouble xyPos, int unk2)) 0x807344b8)

#endif //PROJECTMCODES_SOGROUNDMODULEIMPL_H
