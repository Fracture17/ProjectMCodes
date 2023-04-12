//
// Created by dareb on 7/30/2020.
//

#include <Containers/Vec3f.h>
#include "soGroundModuleImpl.h"

double soGroundModuleImpl::getDistanceFromUnderGrCol(double maxDistance, Vec3f * position, bool unkBool) {
    return _getDistanceFromUnderGrCol_soGroundModuleImpl(maxDistance, this, position, unkBool);
}

int soGroundModuleImpl::getCorrect() {
    return _getCorrect_soGroundModuleImpl(this, 0);
}

void soGroundModuleImpl::setCorrect(int collisionMode) {
    _setCorrect_soGroundModuleImpl(this, collisionMode, 0);
}

bool soGroundModuleImpl::attachGround() {
    return _attachGround_soGroundModuleImpl(this, 0);
}

void soGroundModuleImpl::getCorrectPos(Vec3f* result) {
    _getCorrectPos_soGroundModuleImpl(result, this, 0);
}

xyDouble soGroundModuleImpl::getCorrectPos() {
    Vec3f correctPos = {};
    getCorrectPos(&correctPos);
    xyDouble res;
    res.xPos = correctPos.f1;
    res.yPos = correctPos.f2;
    return res;
}

unsigned int soGroundModuleImpl::isPassableGround(int unk) {
    return _isPassableGround_soGroundModuleImpl(this, unk);
}

unsigned int soGroundModuleImpl::isPassableCheck(int unk) {
    return _isPassableCheck_soGroundModuleImpl(this, unk);
}

xyDouble soGroundModuleImpl::getLeftPos() {
    return _getLeftPos_soGroundModuleImpl(this, 0);
}

xyDouble soGroundModuleImpl::getRightPos() {
    return _getRightPos_soGroundModuleImpl(this, 0);
}

xyDouble soGroundModuleImpl::getUpPos() {
    return _getUpPos_soGroundModuleImpl(this, 0);
}

xyDouble soGroundModuleImpl::getDownPos() {
    return _getDownPos_soGroundModuleImpl(this, 0);
}

// xyDouble soGroundModuleImpl::getDownCorrectPos() {
//     return _getDownCorrectPos_soGroundModuleImpl(this, 0);
// }

void soGroundModuleImpl::apply() {
    _apply_soGroundModuleImpl(this);
}

void soGroundModuleImpl::setShapeSafePos(xyDouble xyPos, int unk) {
    return _setShapeSafePos_soGroundModule(this, xyPos, unk);
}