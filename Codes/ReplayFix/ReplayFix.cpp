//
// Created by johno on 7/1/2020.
//

#include "ReplayFix.h"



//only applies at begining of game
//most codes use isRecording instead
bool isInReplay = false;


enum class ReplayState {
    none,
    recording,
    playing,
    shouldStartRecording,
    shouldStartPlaying,
    shouldStopRecording,
    shouldStopPlaying
};

ReplayState replayState = ReplayState::none;

char recordReplayPath[]  = "/Project+/rp/rp_200701_1500.bin";


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
        playGameStart();
    }
    else {
        recordGameStart(recordReplayPath);
    }
}


BASIC_INJECT("setBaseInfo", 0x8004b0f8, "blr");

extern "C" void setBaseInfo() {
    if(isInReplay == false) {
        recordMeleeInfo();
    }
}


//r28 is gfFileIORequest*
INJECTION("handleRecordFileLoads", 0x80023210, R"(
    SAVE_REGS
    mr r3, r28
    bl handleRecordFileLoads
    RESTORE_REGS
    lwz r3, 0x18(r3)
)");


extern "C" void handleRecordFileLoads(gfFileIORequest* fileIoRequest) {
    if(replayState == ReplayState::recording) {
        recordLoadEvent(fileIoRequest);
    }
}


//r28 is gfFileIORequest*
INJECTION("handlePlaybackFileLoads", 0x800231f0, R"(
    SAVE_REGS
    mr r3, r28
    bl handlePlaybackFileLoads
    RESTORE_REGS
    cmpwi r0, 1
)");


extern "C" void handlePlaybackFileLoads(gfFileIORequest* fileIoRequest) {
    if(replayState == ReplayState::playing) {
        playFileLoad(fileIoRequest);
    }
}


BASIC_INJECT("frameStart", 0x800171b4, "li r25, 1");


extern "C" void frameStart() {
    if(replayState == ReplayState::playing) {
        ASSERT(isInReplay);

        if(isGamePaused() == false) {
            playFrameStart();
        }
    }
    else if(replayState == ReplayState::recording) {
        ASSERT(isInReplay == false);

        if(isGamePaused() == false) {
            recordFrameEnd();
            recordFrameStart();
        }
    }
    else if(replayState == ReplayState::shouldStartPlaying) {
        ASSERT(isInReplay);

        replayState = ReplayState::playing;

        startPlayback();
    }
    else if(replayState == ReplayState::shouldStartRecording) {
        ASSERT(isInReplay == false);

        startRecording();
        replayState = ReplayState::recording;
    }
    else if(replayState == ReplayState::shouldStopPlaying) {
        ASSERT(isInReplay);

        replayState = ReplayState::none;

        stopPlayback();
        isInReplay = false;
    }
    else if(replayState == ReplayState::shouldStopRecording) {
        ASSERT(isInReplay == false);

        stopRecording();
        replayState = ReplayState::none;
    }
}


//TODO: Check if the playing branches actually get run

//sets should start recording flag, and stops Brawl start recording function
BASIC_INJECT("startReplay", 0x8004b328, "blr");

extern "C" void startReplay() {
    if(isInReplay) {
        replayState = ReplayState::shouldStartPlaying;
    }
    else {
        replayState = ReplayState::shouldStartRecording;
    }
}


BASIC_INJECT("stopReplay", 0x8004b5e4, "blr");

extern "C" void stopReplay() {
    if(replayState == ReplayState::playing) {
        replayState = ReplayState::shouldStopPlaying;
    }
    else if(replayState == ReplayState::recording) {
        replayState = ReplayState::shouldStopRecording;
    }
}





INJECTION("playOrRecord", 0x8004aa28, R"(
    SAVE_REGS
    bl playOrRecord
    RESTORE_REGS
    mr r28, r31
)");


//TODO: called once per character, either move or only record/play current port
extern "C" void playOrRecord() {
    if(isGamePaused() == false) {
        if(replayState == ReplayState::playing) {
            playFighters();
        }
        else if(replayState == ReplayState::recording) {
            recordFighterInfo();
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







//r3 already has muReplayTask*
BASIC_INJECT("setupReplay", 0x8119841c, "blr");

extern "C" void setupReplay(muReplayTask& replayTask) {
    setGameInfoForReplay(replayTask);
    isInReplay = true;
}


INJECTION("stopReplayButtonConfig", 0x8011051c, "mr r28, r29");



//These might be duplicates
BASIC_INJECT("setStartPlaybackFlag", 0x8004b608, "blr");

extern "C" void setStartPlaybackFlag() {
    ASSERT(replayState == ReplayState::none);
    ASSERT(isInReplay);

    replayState = ReplayState::shouldStartPlaying;
}



BASIC_INJECT("setStopPlaybackFlag", 0x8004b738, "blr");


extern "C" void setStopPlaybackFlag() {
    if(replayState == ReplayState::playing) {
        replayState = ReplayState::shouldStopPlaying;
    }
}


