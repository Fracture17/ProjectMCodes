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
	char _spacer[0x4];

	// 0x4
	int entryId;
	char _spacer2[0x18 - 0x4 - 4];

	// 0x18
	int playerNumber;
};

//first int is fighter id or somethign
//second int seems to not be used (immediatly overwritten)
//#define ct_Fighter ((Fighter* (*)(Fighter* This, entryID, int, int, soModuleAccesser*)) 0x80830200)
//#define getInput_Fighter ((IpHuman* (*)(Fighter* This)) 0x8083ae38)
//#define getOwner_Fighter ((ftOwner * const (*)(Fighter * This)) 0x8083ae24)