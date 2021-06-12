//
// Created by dareb on 7/30/2020.
//

#include <Containers/Vec3f.h>
#include "soGroundModuleImpl.h"

double soGroundModuleImpl::getDistanceFromUnderGrCol(double maxDistance, Vec3f * position, bool unkBool) {
    return _getDistanceFromUnderGrCol_soGroundModuleImpl(maxDistance, this, position, unkBool);
}

void soGroundModuleImpl::setCorrect(int collisionMode) {
    _setCorrect_soGroundModuleImpl(this, collisionMode, 0);
}

unsigned int soGroundModuleImpl::isPassableGround(int unk) {
    return _isPassableGround_soGroundModuleImpl(this, unk);
}

unsigned int soGroundModuleImpl::isPassableCheck(int unk) {
    return _isPassableCheck_soGroundModuleImpl(this, unk);
}