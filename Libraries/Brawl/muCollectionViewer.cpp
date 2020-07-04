//
// Created by johno on 7/2/2020.
//

#include "muCollectionViewer.h"

int muCollectionViewer::getCurFileID() {
    return _getCurFileID_muCollectionViewer(this);
}


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


void muCollectionViewer::getFileHandle(char* buffer, muCollectionViewer* base, int fileID) {
    _getFileHandle_muCollectionViewer(buffer, base, fileID, 0xBEEF);
}
