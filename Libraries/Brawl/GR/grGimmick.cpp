//
// Created by Ilir on 2022-02-11.
//

#include "grGimmick.h"

void grGimmick::getPos(Vec3f* pos) {
    getPos_grGimmick(pos, this);
}

void grGimmick::getRot(Vec3f* rot) {
    getRot_grGimmick(rot, this);
}

void grGimmick::getScale(Vec3f* scale) {
    getScale_grGimmick(scale, this);
}

void grGimmick::setPos(double x, double y, double z) {
    setPos3_grGimmick(this, x, y, z);
}

void grGimmick::setRot(double rot1, double rot2, double rot3) {
    setRot2_grGimmick(this, rot1, rot2, rot3);
}

void grGimmick::setScale(double x, double y, double z) {
    setScale2_grGimmick(this, x, y, z);
}