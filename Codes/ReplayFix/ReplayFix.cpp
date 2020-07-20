//
// Created by johno on 7/1/2020.
//

#include "ReplayFix.h"



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



bool isInGame = false;
int dataIndex = 0;

INJECTION("stopKeyRecorderClear", 0x8004b060, "blr");
INJECTION("stopSetInitialProcessCount", 0x8004b310, "blr");
INJECTION("stopStartRec", 0x8004b328, R"(
    li r3, 1
    lis r4, isInGame@ha
    stb r3, isInGame@l(r4)
    blr
)");


BASIC_INJECT("setBaseInfo", 0x8004b0f8, "blr");

extern "C" void setBaseInfo() {
    memcpy(SAVED_GAME_INFO, GM_GLOBAL_MODE_MELEE, sizeof(gmGlobalModeMelee));
}

BASIC_INJECT("saveReplay", 0x8004b5e4, "lwz r0, 0(r3)");

char path[]  = "/Project+/rp/rp_200701_1500.bin";
bool hasSaved = false;
extern "C" void saveReplay() {
    if(hasSaved == false) {
        auto file = gfFileIO(path, 0, dataIndex * sizeof(FighterInputDataForReplayFrame) + sizeof(gmGlobalModeMelee),
                             SAVED_GAME_INFO,
                             nullptr);
        //file.writeSDFile();
        writeToSDFile(path, SAVED_GAME_INFO,
                      dataIndex * sizeof(FighterInputDataForReplayFrame) + sizeof(gmGlobalModeMelee));
        dataIndex = 0;
        isInGame = false;
        hasSaved = true;
    }
}


/*INJECTION("record", 0x808391c0, R"(
    SAVE_REGS
    mr r3, r4
    bl record
    RESTORE_REGS
    addi r11, sp, 32
)");


extern "C" void record(soModuleAccessor& accessor) {
    if(isInGame) {



        auto& controller = ((ftControllerModuleImpl*)accessor.controllerModule)->controller;
        SAVED_INPUT[dataIndex].currentInputs = controller.inputs;
        SAVED_INPUT[dataIndex].prevInputs = controller.prevInputs;
        SAVED_INPUT[dataIndex].stickY = controller.stickY;
        SAVED_INPUT[dataIndex].stickX = controller.stickX;

        auto& LAVariables = accessor.workModule->LAVariables;
        SAVED_INPUT[dataIndex].cStickY = (*LAVariables->floats)[35];
        SAVED_INPUT[dataIndex].cStickX = (*LAVariables->floats)[34];
        SAVED_INPUT[dataIndex].lightShield = (*LAVariables->basics)[77];

        dataIndex++;
    }
}*/



INJECTION("record", 0x80029570, R"(
    #This is the replacement.  Doing it now so I have room on the stack
    addi sp, sp, 1328

    SAVE_REGS
    mr r3, r4
    bl record
    RESTORE_REGS
)");

bool isInReplay = false;


extern "C" void record(soModuleAccessor& accessor) {
    if(isInGame && isInReplay == false) {
        SAVED_INPUT[dataIndex] = PAD_SYSTEM->pads[0];

        dataIndex++;
    }
}


//prevents replays from being considered corrupted when selected
INJECTION("alwaysAllowReplayToBeSelected", 0x8119811c, "cmpwi r0, 1");



void getCurrentReplayName(muReplayTask& replayTask, char* buffer) {
    int fileID = replayTask.getCurFileID();
    muReplayTask::getFileHandle(buffer, &replayTask, fileID);
}



//r3 already has muReplayTask*
BASIC_INJECT("setGameInfoForReplay", 0x8119841c, "blr");




const char REPLAY_DIRECTORY[] = "A:/Project+/rp/";

extern "C" void setGameInfoForReplay(muReplayTask& replayTask) {
    char replayName[100] = {};
    getCurrentReplayName(replayTask, replayName);
    char replayPath[100] = {};
    strcpy(replayPath, REPLAY_DIRECTORY);
    strcat(replayPath, replayName);

    readFromSDFile(replayPath, SAVED_GAME_INFO, 0x8000, 0);

    //auto file = gfFileIO(replayPath, 0, 0, nullptr, SAVED_GAME_INFO);
    //file.readSDFile();

    memcpy(GM_GLOBAL_MODE_MELEE, SAVED_GAME_INFO, sizeof(gmGlobalModeMelee));

    //IP_SWITCH->controllerFLags ^= 0x80000000;
    replayTask.resources->startReplayFlag = 0;

    isInReplay = true;
    dataIndex = 0;
}





INJECTION("stopMeleeGetInitialProcessCount", 0x8004b304, "blr");
INJECTION("stopStartPlay", 0x8004b608, R"(
    li r3, 0
    blr
)");
INJECTION("stopEndPlay", 0x8004b738, R"(
    li r3, 0
    lis r4, isInReplay@ha
    stb r3, isInReplay@l(r4)
    blr
)");

BASIC_INJECT("play", 0x8002956c, "mtlr r0");


extern "C" void play() {
    if(isInReplay) {
        PAD_SYSTEM->pads[0] = SAVED_INPUT[dataIndex];
        dataIndex++;
    }
}



