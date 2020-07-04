//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_REPLAYFIX_H
#define PROJECTMCODES_REPLAYFIX_H


#include "Assembly.h"
#include "Brawl/soModuleAccessor.h"
#include "Brawl/ftControllerModuleImpl.h"
#include "FighterInputDataForReplayFrame.h"
#include "Wii/SD.h"
#include "Brawl/gmGlobalModeMelee.h"
#include "Memory.h"
#include "Brawl/muReplayTask.h"
#include "CLibs/cstring.h"
#include "Brawl/gfFileIO.h"
#include "Brawl/ipSwitch.h"
#include "Wii/PADStatus.h"
#include "Brawl/gfPadSystem.h"


#define REPLAY_BUFFER_START ((u8*) 0x91301c00)
#define SAVED_GAME_INFO ((gmGlobalModeMelee*) (REPLAY_BUFFER_START + 0x2000))
//#define SAVED_INPUT ((FighterInputDataForReplayFrame*) (SAVED_GAME_INFO + 1))
#define SAVED_INPUT ((gfPadGamecube*) (SAVED_GAME_INFO + 1))

#endif //PROJECTMCODES_REPLAYFIX_H
