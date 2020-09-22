//
// Created by johno on 7/1/2020.
//

#include "ReplayFix.h"


ReplayFrameEventManager eventManager(reinterpret_cast<ReplayEvent*>(REPLAY_BUFFER_START + 0x2000),
                                     0x10000);

//only applies at begining of game
//most codes use isRecording instead
bool isInReplay = false;

bool isRecording = false;
bool isPlaying = false;

bool shouldStartRecording = false;
bool shouldStartPlaying = false;

bool shouldStopRecording = false;
bool shouldStopPlaying = false;

char recordReplayPath[]  = "/Project+/rp/rp_200701_1500.bin";

FILE* replayFile = nullptr;

//these are the hashes of the load events that were already supposed to occur
vector<u16> playbackSchedualedLoadEvents;
//these are references to files that have finsihed loading, but were not loaded yet in the recorded game
vector<gfFileIORequest*> playbackEncounteredLoadEvents;


bool isGamePaused() {
    return (GF_APPLICATION->_gameFlags & 0x01000000) != 0;
}

BASIC_INJECT("gameStart", 0x806cf140, "stwu sp, -0x70(sp)");


extern "C" void gameStart() {
    if(isInReplay) {
        eventManager.loadNextFrame();
        auto gameStartEvent = eventManager.getGameStartEvent();
        DEFAULT_MT_RAND->seed = gameStartEvent->randSeed;
        *GM_GLOBAL_MODE_MELEE = gameStartEvent->meleeInfo;
    }
    else {
        replayFile = fopen(recordReplayPath, WRITE);
        eventManager.openReplayFileForWriting(replayFile);

        auto gameStartEvent = new ReplayGameStartEvent;
        gameStartEvent->randSeed = DEFAULT_MT_RAND->seed;
        eventManager.push(gameStartEvent);
    }
}


BASIC_INJECT("setBaseInfo", 0x8004b0f8, "blr");

extern "C" void setBaseInfo() {
    if(isInReplay == false) {
        eventManager.getGameStartEvent()->meleeInfo = *GM_GLOBAL_MODE_MELEE;
    }
}


//r28 is gfFileIORequest*
INJECTION("handleFileLoads", 0x80023210, R"(
    SAVE_REGS
    mr r3, r28
    bl handleFileLoads
    RESTORE_REGS
    lwz r3, 0x18(r3)
)");


extern "C" void handleFileLoads(gfFileIORequest* fileIoRequest) {
    if(isPlaying) {
        for(int i = 0; i < playbackSchedualedLoadEvents.size(); i++) {
            if(playbackSchedualedLoadEvents[i] == fileIoRequest->hash) {
                playbackSchedualedLoadEvents.erase(i);
                return;
            }
        }

        fileIoRequest->params &= ~IS_READY_PARAM_GF_FILE_IO_REQUEST;
        playbackEncounteredLoadEvents.push(fileIoRequest);
    }
    else if(isRecording) {
        auto loadEvent = new ReplayLoadEvent();
        loadEvent->loadHash = fileIoRequest->hash;
        eventManager.push(loadEvent);
    }
}



bool removeLoadEvent(ReplayLoadEvent* event) {
    for(int i = 0; i < playbackEncounteredLoadEvents.size(); i++) {
        if(playbackEncounteredLoadEvents[i]->hash == event->loadHash) {
            playbackEncounteredLoadEvents[i]->params |= IS_READY_PARAM_GF_FILE_IO_REQUEST;
            playbackEncounteredLoadEvents.erase(i);
            return true;
        }
    }
    return false;
}

BASIC_INJECT("frameStart", 0x800171b4, "li r25, 1");


extern "C" void frameStart() {
    if(shouldStopRecording) {
        shouldStopRecording = false;
        isRecording = false;

        //add frame end event
        auto frameEndEvent = new ReplayFrameEndEvent;
        frameEndEvent->frameNum = GAME_FRAME->frameCounter;
        frameEndEvent->randSeed = DEFAULT_MT_RAND->seed;
        eventManager.push(frameEndEvent);

        eventManager.saveFrameEvents();

        auto endEvent = new ReplayGameEndEvent;
        eventManager.push(endEvent);

        eventManager.saveAndClose();

        replayFile = nullptr;

        return;
    }
    if(shouldStopPlaying) {
        shouldStopPlaying = false;
        isPlaying = false;
        eventManager.fileIO.shouldClose = true;
        replayFile = nullptr;
        eventManager.clear();
        playbackEncounteredLoadEvents.clear();
        playbackSchedualedLoadEvents.clear();
    }



    if(isGamePaused() == false) {
        if(isRecording) {
            //add frame end event
            auto frameEndEvent = new ReplayFrameEndEvent;
            frameEndEvent->frameNum = GAME_FRAME->frameCounter;
            frameEndEvent->randSeed = DEFAULT_MT_RAND->seed;
            eventManager.push(frameEndEvent);

            eventManager.saveFrameEvents();

            //set up for next frame
            auto frameStartEvent = new ReplayFrameStartEvent;
            frameStartEvent->randomSeed = DEFAULT_MT_RAND->seed;
            frameStartEvent->frameNum = GAME_FRAME->frameCounter;
            eventManager.push(frameStartEvent);
        }
        else if(isPlaying) {
            if(playbackSchedualedLoadEvents.empty()) {
                eventManager.loadNextFrame();

                auto frameStartEvent = eventManager.getFrameStartEvent();
                DEFAULT_MT_RAND->seed = frameStartEvent->randomSeed;


                //TODO: Clean this
                //Checks if recorded load event has happend
                for(int i = 0; i < eventManager.events.size(); i++) {
                    if(eventManager.events[i]->id == ReplayEventID::load) {
                        auto event = (ReplayLoadEvent*) eventManager.events[i];
                        if(removeLoadEvent(event) == false) {
                            playbackSchedualedLoadEvents.push(event->loadHash);
                        }
                    }
                }


                auto frame = eventManager.getFrameStartEvent();
                if(frame->frameNum != (GAME_FRAME->frameCounter)) {
                    int x = 0;
                    x += 3;
                }
            }
            if(playbackSchedualedLoadEvents.empty() == false) {
                //pause until loaded
            }
        }
    }




    //The signal to start recording is sent in the middle of the frame
    //This means that info that occurred before would be lost, so just skip that frame and start now
    if(shouldStartRecording) {
        eventManager.saveFrameEvents();

        isRecording = true;
        shouldStartRecording = false;

        //set up for next frame
        auto frameStartEvent = new ReplayFrameStartEvent;
        frameStartEvent->randomSeed = DEFAULT_MT_RAND->seed;
        frameStartEvent->frameNum = GAME_FRAME->frameCounter;
        eventManager.push(frameStartEvent);
    }
    if(shouldStartPlaying) {
        isPlaying = true;
        shouldStartPlaying = false;

        eventManager.loadNextFrame();

        auto frameStartEvent = eventManager.getFrameStartEvent();
        DEFAULT_MT_RAND->seed = frameStartEvent->randomSeed;
    }
}


//sets should start recording flag, and stops Brawl start recording function
BASIC_INJECT("startRecording", 0x8004b328, "blr");

extern "C" void startRecording() {
    if(isInReplay) {
        shouldStartPlaying = true;
    }
    else {
        shouldStartRecording = true;
    }
}


BASIC_INJECT("endRecording", 0x8004b5e4, "blr");

extern "C" void endRecording() {
    if(isRecording) {
        shouldStopRecording = true;
    }
    if(isPlaying) {
        shouldStopPlaying = true;
    }
}




//r28 is padStatus array ptr
INJECTION("playOrRecord", 0x8004aa5c, R"(
    SAVE_REGS
    bl playOrRecord
    RESTORE_REGS
    addi r5, sp, 32
)");


//TODO: called once per character, either move or only record/play current port
extern "C" void playOrRecord() {
    if(isGamePaused() == false) {
        if(isPlaying) {
            auto frame = eventManager.getFrameStartEvent();
            if(frame->frameNum != (GAME_FRAME->frameCounter + 1)) {
                int x = 0;
                x += 2;
            }
            for(int i = 0; i < 4; i++) {
                auto fighterEvent = eventManager.getFighterEventFromPort(i);
                if(fighterEvent != nullptr) {
                    auto& pad = PAD_SYSTEM->pads[i];
                    pad.buttons = fighterEvent->buttons;
                    pad.stickX = fighterEvent->stickX;
                    pad.stickY = fighterEvent->stickY;
                    pad.cStickX = fighterEvent->cStickX;
                    pad.cStickY = fighterEvent->cStickY;
                    pad.LTrigger = fighterEvent->LTrigger;
                    pad.RTrigger = fighterEvent->RTrigger;
                }
            }
        }
        else if(isRecording) {
            for(int i = 0; i < 4; i++) {
                auto entry = FIGHTER_MANAGER->getEntryIdFromIndex(i);
                if(entry != -1) {
                    auto fighterEvent = new ReplayPreFrameFighterEvent;
                    auto& pad = PAD_SYSTEM->pads[i];
                    fighterEvent->buttons = pad.buttons;
                    fighterEvent->stickX = pad.stickX;
                    fighterEvent->stickY = pad.stickY;
                    fighterEvent->cStickX = pad.cStickX;
                    fighterEvent->cStickY = pad.cStickY;
                    fighterEvent->LTrigger = pad.LTrigger;
                    fighterEvent->RTrigger = pad.RTrigger;
                    fighterEvent->port = i;

                    eventManager.push(fighterEvent);
                }
            }
        }
    }
}




INJECTION("stopKeyRecorderClear", 0x8004b060, "blr");
INJECTION("stopSetInitialProcessCount", 0x8004b310, "blr");
INJECTION("stopMeleeGetInitialProcessCount", 0x8004b304, "blr");



















INJECTION("replaceReplayPathWithFakeFile", 0x81198350, R"(
    SAVE_REGS
    bl replaceReplayPathWithFakeFile
    cmpwi r3, 1
    RESTORE_REGS
    beq _SKIP_replaceReplayPathWithFakeFile
    blr
_SKIP_replaceReplayPathWithFakeFile:
    lwz r4, 0x1AC(r4)
)");


const char FAKE_REPLAY_NAME[] = "fake.bin";

//replaces calls to getFileHandle so they return a path to a dummy valid replay
//Since the function is still useful to me, if r6 is set to 0xBEEF, it runs as normal
extern "C" bool replaceReplayPathWithFakeFile(char* buffer, muCollectionViewer* base, int fileID, int flag) {
    if(flag == 0xBEEF) {
        return true;
    }
    strcpy(buffer, FAKE_REPLAY_NAME);
    return false;
}




//prevents replays from being considered corrupted when selected
INJECTION("alwaysAllowReplayToBeSelected", 0x8119811c, "cmpwi r0, 1");





void getCurrentReplayName(muReplayTask& replayTask, char* buffer) {
    int fileID = replayTask.getCurFileID();
    muReplayTask::getFileHandle(buffer, &replayTask, fileID);
}


const char REPLAY_DIRECTORY[] = "A:/Project+/rp/";

//r3 already has muReplayTask*
BASIC_INJECT("setGameInfoForReplay", 0x8119841c, "blr");

extern "C" void setGameInfoForReplay(muReplayTask& replayTask) {
    char replayName[100] = {};
    getCurrentReplayName(replayTask, replayName);
    char replayPath[100] = {};
    strcpy(replayPath, REPLAY_DIRECTORY);
    strcat(replayPath, replayName);

    replayFile = fopen(replayPath, READ);




    fread(REPLAY_BUFFER_START + 0x10000, sizeof(ReplayGameStartEvent), 1, replayFile);
    auto gameStartEvent = (ReplayGameStartEvent*) (REPLAY_BUFFER_START + 0x10000);
    DEFAULT_MT_RAND->seed = gameStartEvent->randSeed;
    *GM_GLOBAL_MODE_MELEE = gameStartEvent->meleeInfo;

    fseek(replayFile, 0);


    eventManager.openReplayFileForReading(replayFile);




    //fread(REPLAY_BUFFER_START + 0x5000, sizeof(ReplayGameStartEvent), 1, replayFile);
    //ReplayGameStartEvent* gameStartEvent = (ReplayGameStartEvent*) (REPLAY_BUFFER_START + 0x5000);
    //DEFAULT_MT_RAND->seed = gameStartEvent->randSeed;
    //*GM_GLOBAL_MODE_MELEE = gameStartEvent->meleeInfo;


    //eventManager.loadNextFrame();

    //readFromSDFile(replayPath, SAVED_GAME_INFO, sizeof(ReplayGameStartInfo), 0);

    //auto file = gfFileIO(replayPath, 0, 0, nullptr, SAVED_GAME_INFO);
    //file.readSDFile();

    //DEFAULT_MT_RAND->seed = SAVED_GAME_INFO->randSeed;
    //*GM_GLOBAL_MODE_MELEE = SAVED_GAME_INFO->meleeInfo;

    //replayIOThread = OSThread(_readReplayFrame, 0x2000, 0);

    //memcpy(GM_GLOBAL_MODE_MELEE, &SAVED_GAME_INFO->meleeInfo, sizeof(gmGlobalModeMelee));

    IP_SWITCH->controllerFLags ^= 0x80000000;
    replayTask.resources->startReplayFlag = 0;

    isInReplay = true;
    //dataIndex = 0;
}


INJECTION("stopReplayButtonConfig", 0x8011051c, "mr r28, r29");


BASIC_INJECT("startPlayback", 0x8004b608, "blr");

extern "C" void startPlayback() {
    shouldStartPlaying = true;
}



BASIC_INJECT("stopPlayback", 0x8004b738, "blr");


extern "C" void stopPlayback() {
    if(isPlaying) {
        shouldStopPlaying = true;
    }
}