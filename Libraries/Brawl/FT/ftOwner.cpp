//
// Created by dareb on 7/25/2020.
//

#include "ftOwner.h"

double FtOwner::getDamage() {
    return _getDamage_ftOwner(this);
}

void FtOwner::setDamage(double newValue, int shouldLog) {
    _setDamage_ftOwner(newValue, this, shouldLog);
}

bool FtOwner::isCpu() {
    return _isOperationCpu_ftOwner(this);
}

bool FtOwner::hasCurry() {
    return _getCurry_ftOwner(this);
}

void FtOwner::setCurry(bool curryState) {
    _setCurry_ftOwner(this, curryState);
}