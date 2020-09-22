//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_REPLAYFIX_H
#define PROJECTMCODES_REPLAYFIX_H


#include "ReplayEvent.h"
#include "Assembly.h"
#include "Memory.h"
//#include "AsyncFileIO.h"
#include "ReplayFrameEventManager.h"
#include "ReplayFrameStartEvent.h"
#include "ReplayGameStartEvent.h"
#include "ReplayGameEndEvent.h"
#include "ReplayPreFrameFighterEvent.h"
#include "Brawl/ftManager.h"
#include "ReplayFrameEndEvent.h"
#include "ReplayLoadEvent.h"

#define REPLAY_BUFFER_START ((u8*) 0x91301c00)



#include "Assembly.h"
#include "Brawl/soModuleAccessor.h"
#include "Brawl/ftControllerModuleImpl.h"
#include "Wii/SD.h"
#include "Brawl/gmGlobalModeMelee.h"
#include "Memory.h"
#include "Brawl/muReplayTask.h"
#include "CLibs/cstring.h"
#include "Brawl/gfFileIO.h"
#include "Brawl/ipSwitch.h"
#include "Wii/PADStatus.h"
#include "Brawl/gfPadSystem.h"
#include "Brawl/GameGlobal.h"
#include "Brawl/gfApplication.h"
#include "ReplayFrameData.h"
#include "ReplayGameStartInfo.h"
#include "Wii/OSError.h"
#include "Wii/OSThread.h"
#include "Brawl/gfFileIOHandle.h"

#endif //PROJECTMCODES_REPLAYFIX_H
