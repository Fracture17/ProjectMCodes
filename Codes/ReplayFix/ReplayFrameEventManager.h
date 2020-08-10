//
// Created by johno on 7/24/2020.
//

#ifndef PROJECTMCODES_REPLAYFRAMEEVENTMANAGER_H
#define PROJECTMCODES_REPLAYFRAMEEVENTMANAGER_H


#include "ReplayEvent.h"
#include "ReplayGameStartEvent.h"
#include "ReplayFrameStartEvent.h"
#include "ReplayPreFrameFighterEvent.h"
#include "Memory.h"
#include "AsyncFileIO.h"
#include "Wii/FILE.h"
#include "Containers/vector.h"


class ReplayFrameEventManager {
public:
    ReplayFrameEventManager(ReplayEvent* buffer, u32 bufferSize);

    //ReplayEvent* next(ReplayEvent* current);

    //copies event to buffer
    void push(ReplayEvent* event);

    //only works at begining of game of course
    ReplayGameStartEvent* getGameStartEvent();
    ReplayFrameStartEvent* getFrameStartEvent();
    ReplayPreFrameFighterEvent* getFighterEventFromPort(int port, bool createIfNotFound=false);

    void clear();
    void loadEventsToVector();

    void openReplayFileForWriting(FILE* file);
    void openReplayFileForReading(FILE* file);

    void saveFrameEvents();
    void saveAndClose();
    void loadNextFrame();

    vector<ReplayEvent*> events;

    AsyncFileIO fileIO;
};


#endif //PROJECTMCODES_REPLAYFRAMEEVENTMANAGER_H
