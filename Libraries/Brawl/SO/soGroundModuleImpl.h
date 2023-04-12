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
    VTABLE_METHOD(void, init)
    VTABLE_METHOD(void, getType)
    VTABLE_METHOD(void, getAABBox)
    VTABLE_METHOD_1ARG(void, setDownPos, Vec2f*, newPos)
    VTABLE_METHOD(void, setLeftPos)
    VTABLE_METHOD(void, setRightPos)
    VTABLE_METHOD(void, setCenterPos)
    VTABLE_METHOD(void, setTopPos)
    VTABLE_METHOD(void, getCenterPos)
    VTABLE_METHOD(void, getDownPos)
    VTABLE_METHOD(void, getLeftPos)
    VTABLE_METHOD(void, getRightPos)
    VTABLE_METHOD(void, getTopPos)
    VTABLE_METHOD(void, move)
    VTABLE_METHOD(void, set)
    VTABLE_METHOD(void, getHeight)
    VTABLE_METHOD(void, getWidth)
    VTABLE_METHOD(void, test)
    VTABLE_METHOD(void, updateAABBox)
    VTABLE_METHOD(void, setLR)
    VTABLE_METHOD(void, setUD)
    VTABLE_METHOD(void, setLR1)
    VTABLE_METHOD(void, modifyDownY)
    
    grCollShape_vtable* vtable;
};

struct grCollStatus {
    char _spacer[0x60];
    // 0x60
    grCollShape* currCollShape;

    char _spacer2[0x6E - 0x60 - 4];

    // 0x14
    char airGroundState;
};

struct soGroundShapeImpl {
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
