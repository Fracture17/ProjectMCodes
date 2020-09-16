#pragma once

#include "StageObject.h"
#include "soModuleAccessor.h"

//A unique id given when loading the character
typedef int entryID;

struct Fighter : public StageObject {
	//can't use constructor because Brawl constructor already calls base class constructor
	//so it would be called multiple times
	//Fighter* ct(entryID id, int, int);
	//IpHuman* getInput();
	//ftOwner* getOwner();

	char _spacer_ft[0x110 - sizeof(StageObject)];
};

#define _getFtKind_Fighter (( (*)()) 0x8076b00c)

//first int is fighter id or somethign
//second int seems to not be used (immediatly overwritten)
//#define ct_Fighter ((Fighter* (*)(Fighter* This, entryID, int, int, soModuleAccesser*)) 0x80830200)
//#define getInput_Fighter ((IpHuman* (*)(Fighter* This)) 0x8083ae38)
//#define getOwner_Fighter ((ftOwner * const (*)(Fighter * This)) 0x8083ae24)