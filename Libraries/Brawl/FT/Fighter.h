#ifndef PROJECTMCODES_FIGHTER_H
#define PROJECTMCODES_FIGHTER_H

#include "Brawl/SO/StageObject.h"
#include "Brawl/SO/soModuleAccessor.h"
#include "Brawl/FT/ftCancelModule.h"
#include "Brawl/FT/ftOwner.h"

//A unique id given when loading the character
typedef int entryID;

struct Fighter_vtable18 {
	char _spacer[0x150];
	void** cancelModuleFn;
};

struct ftOwner;
struct Fighter : public StageObject {
	
	ftCancelModule* getCancelModule();
	
	//can't use constructor because Brawl constructor already calls base class constructor
	//so it would be called multiple times
	//Fighter* ct(entryID id, int, int);
	//IpHuman* getInput();
	ftOwner* getOwner();

    int getFtKind();
    void setMetal(double health, bool setEffect, int unk3);
    void setCurry(bool setEffect, int unk2);
    void setHammer(bool setEffect, int unk2);
    void setSuperStar(bool setEffect, unsigned int unk2, int unk3);
    void setFlower(double rate, bool setEffect, int unk3, double size, int unk5);
    void setHeartSwap(entryID givingPlayerEntryId, int unk2, bool setEffect);
    void setSlow(bool setEffect, int slowStrength, int duration, int unk4);
    void startScaling(bool setEffect, bool isPoison);

    void endFinal(int unk1, int unk2, int unk3);

	char _spacer_vt18[0x100 - sizeof(StageObject)];
	Fighter_vtable18* vtable18;

	char _spacer_ft[0x110 - 0x100 - 4];
};

#define _getFtKind_Fighter ((int (*)(Fighter * This)) 0x8076b00c)

//first int is fighter id or somethign
//second int seems to not be used (immediatly overwritten)
//#define ct_Fighter ((Fighter* (*)(Fighter* This, entryID, int, int, soModuleAccesser*)) 0x80830200)
//#define getInput_Fighter ((IpHuman* (*)(Fighter* This)) 0x8083ae38)
#define getOwner_Fighter ((ftOwner * const (*)(Fighter * This)) 0x8083ae24)

#define setMetal_Fighter ((void (*)(Fighter* self, double health, bool setEffect, int unk3)) 0x80843110)
#define setCurry_Fighter ((void (*)(Fighter* self, bool setEffect, int unk2)) 0x80843314)
#define setHammer_Fighter ((void (*)(Fighter* self, bool setEffect, int unk2)) 0x808436d0)
#define setSuperStar_Fighter ((void (*)(Fighter* self, bool setEffect, unsigned int unk2, int unk3)) 0x80843978)
#define setFlower_Fighter ((void (*)(Fighter* self, double rate, bool setEffect, int unk3, double size, int unk5)) 0x80843ce8)
#define setHeartSwap_Fighter ((void (*)(Fighter* self, entryID givingPlayerEntryId, int unk2, bool setEffect)) 0x80844144)
#define setSlow_Fighter ((void (*)(Fighter* self, bool setEffect, int slowStrength, int duration, int unk4)) 0x80842e6c)
#define startScaling_Fighter ((void (*)(Fighter* self, bool setEffect, bool isPoison)) 0x80841ad4)
#define endFinal_Fighter ((void (*)(Fighter* self, int unk1, int unk2, int unk3)) 0x80838318)

#endif