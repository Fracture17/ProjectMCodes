#pragma once

#include "stddef.h"
#include "containers/Vec3f.h"

struct Position {
	float xPos;
	float yPos;
	float zPos;
};

struct soPostureModuleImpl_vtable {
	char*** objDescriptor;
	void* thisPtrFixup;

	void** fn__isNull;
	void** fn____dt;
	void** fn__init;
	void** fn__initPos;
	void** fn__getPos;
	void** fn__setPos;
	void** fn__getPrevPos;
	void** fn__getPrevRoughPos;
	void** fn__setPrevRoughPos;
	void** fn__getLr;
	void** fn__setLr;
	void** fn__reverseLr;
	void** fn__initRot;
	void** fn__initRot1;
	void** fn__getRot;
	void** fn__setRot;
	void** fn__initRotYLr;
	void** fn__isRotYLrDifferentInnerLr;
	void** fn__getRotYLr;
	void** fn__updateRotYLr;
	void** fn__reverseRotYLr;
	void** fn__getBaseScale;
	void** fn__getScale;
	void** fn__setScale;
	void** fn__initScale;
	void** fn__getOwnerScale;
	void** fn__setOwnerScale;
	void** fn__begin;
	void** fn__rewind;
	void** fn__turnBack;
	void** fn__interpolate;
	void** fn__setStickLR;
	void** fn__setSyncConstraintNode;
	void** fn__fixConstraintNodePosture;
	void** fn__fixRot;
};

class soPostureModuleImpl {
public:
	static void getPos(Position* destination, soPostureModuleImpl* source);

	VTABLE_METHOD(soPostureModuleImpl*, void, __dt)
	VTABLE_METHOD(soPostureModuleImpl*, void, init)
	VTABLE_METHOD(soPostureModuleImpl*, void, initPos)
	// VTABLE_METHOD(soPostureModuleImpl*, void, getPos)
	VTABLE_METHOD(soPostureModuleImpl*, void, setPos)
	VTABLE_METHOD(soPostureModuleImpl*, void, getPrevPos)
	VTABLE_METHOD(soPostureModuleImpl*, void, getPrevRoughPos)
	VTABLE_METHOD(soPostureModuleImpl*, void, setPrevRoughPos)
	VTABLE_METHOD(soPostureModuleImpl*, void, getLr)
	VTABLE_METHOD(soPostureModuleImpl*, void, setLr)
	VTABLE_METHOD(soPostureModuleImpl*, void, reverseLr)
	VTABLE_METHOD(soPostureModuleImpl*, void, initRot)
	VTABLE_METHOD(soPostureModuleImpl*, void, initRot1)
	VTABLE_METHOD(soPostureModuleImpl*, void, getRot)
	VTABLE_METHOD_2ARG(soPostureModuleImpl*, void, setRot, Vec3f*, rotation, int, index)
	VTABLE_METHOD(soPostureModuleImpl*, void, initRotYLr)
	VTABLE_METHOD(soPostureModuleImpl*, void, isRotYLrDifferentInnerLr)
	VTABLE_METHOD(soPostureModuleImpl*, void, getRotYLr)
	VTABLE_METHOD(soPostureModuleImpl*, void, updateRotYLr)
	VTABLE_METHOD(soPostureModuleImpl*, void, reverseRotYLr)
	VTABLE_METHOD(soPostureModuleImpl*, void, getBaseScale)
	VTABLE_METHOD(soPostureModuleImpl*, void, getScale)
	VTABLE_METHOD(soPostureModuleImpl*, void, setScale)
	VTABLE_METHOD(soPostureModuleImpl*, void, initScale)
	VTABLE_METHOD(soPostureModuleImpl*, void, getOwnerScale)
	VTABLE_METHOD(soPostureModuleImpl*, void, setOwnerScale)
	VTABLE_METHOD(soPostureModuleImpl*, void, begin)
	VTABLE_METHOD(soPostureModuleImpl*, void, rewind)
	VTABLE_METHOD(soPostureModuleImpl*, void, turnBack)
	VTABLE_METHOD(soPostureModuleImpl*, void, interpolate)
	VTABLE_METHOD(soPostureModuleImpl*, void, setStickLR)
	VTABLE_METHOD(soPostureModuleImpl*, void, setSyncConstraintNode)
	VTABLE_METHOD(soPostureModuleImpl*, void, fixConstraintNodePosture)
	VTABLE_METHOD(soPostureModuleImpl*, void, fixRot)

	soPostureModuleImpl_vtable* vtable;

	char _spacer[0xC - 0x4];
	//0xC
	float xPos;
	//0x10
	float yPos;
	//0x14
	float zPos;
	//0x18
	float prevXPos;
	//0x1C
	float prevYPos;
	//0x20
	float prevZPos;
	char _spacer2[0x3C - 0x20 - 4];

	//0x3C
	float scale;

	//0x40
	//right = 1.0, left = -1.0
	float direction;

    char _spacer3[0x5c - 0x44];

	//0x5C
	//a pointer to a vector with Rotations
    int* rotation;

	char _spacer4[0x68 - 0x58];

	//0x68
	float ownerScale;
};

#define _getPos_PostureModuleImpl ((void (*)(Position *destination, soPostureModuleImpl* source)) 0x8072fa9c)
