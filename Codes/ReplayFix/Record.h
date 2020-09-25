//
// Created by johno on 9/24/2020.
//

#ifndef PROJECTMCODES_RECORD_H
#define PROJECTMCODES_RECORD_H

#include "ReplayEvent.h"
#include "Assembly.h"
#include "Memory.h"
//#include "AsyncFileIO.h"
#include "ReplayFrameEventManager.h"
#include "Brawl/ftManager.h"
#include "Wii/mtRand.h"
#include "Debug.h"
#include "Brawl/gfFileIORequest.h"

#include "Tests/test_ReplayFix.h"

#include "ReplayRecorder.h"



void recordGameStart(char* recordReplayPath);


void recordMeleeInfo();

void recordLoadEvent(gfFileIORequest* fileIoRequest);


void recordFrameEnd();

void recordFrameStart();

void startRecording();


void stopRecording();


void recordFighterInfo();

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
#include "Wii/OSError.h"
#include "Wii/OSThread.h"
#include "Brawl/gfFileIOHandle.h"

#endif //PROJECTMCODES_RECORD_H
