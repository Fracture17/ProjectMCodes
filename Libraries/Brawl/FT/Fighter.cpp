#include "Fighter.h"


// Fighter* Fighter::ct(entryID id, int a, int b) {
// 	//TODO: make module accesser
// 	//ct_Fighter(this, id, a, b, )
// }

// IpHuman* Fighter::getInput() {
// 	return getInput_Fighter(this);
// }

ftOwner* Fighter::getOwner() {
	return getOwner_Fighter(this);
}

int Fighter::getFtKind() {
    return _getFtKind_Fighter(this);
}

void Fighter::setMetal(double health, bool setEffect, int unk3) {
    setMetal_Fighter(this, health, setEffect, unk3);
}

void Fighter::setCurry(bool setEffect, int unk2) {
    setCurry_Fighter(this, setEffect, unk2);
}

void Fighter::setHammer(bool setEffect, int unk2) {
    setHammer_Fighter(this, setEffect, unk2);
}

void Fighter::setSuperStar(bool setEffect, unsigned int unk2, int unk3) {
    setSuperStar_Fighter(this, setEffect, unk2, unk3);
}

void Fighter::setFlower(double rate, bool setEffect, int unk3, double size, int unk5) {
    setFlower_Fighter(this, rate, setEffect, unk3, size, unk5);
}

void Fighter::setHeartSwap(entryID givingPlayerEntryId, int unk2, bool setEffect) {
    setHeartSwap_Fighter(this, givingPlayerEntryId, unk2, setEffect);
}

void Fighter::setSlow(bool setEffect, int slowStrength, int duration, int unk4){
    setSlow_Fighter(this, setEffect, slowStrength, duration, unk4);
}

void Fighter::startScaling(bool setEffect, bool isPoison){
    startScaling_Fighter(this, setEffect, isPoison);
}

void Fighter::endFinal(int unk1, int unk2, int unk3) {
    endFinal_Fighter(this, unk1, unk2, unk3);
}

#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)
ftCancelModule* Fighter::getCancelModule() {
	return ((ftCancelModule* (*)(void* ft)) this->vtable18->cancelModuleFn)(this);
}