//
// Created by johno on 7/24/2020.
//

#include "ReplayFrameEventManager.h"

ReplayFrameEventManager::ReplayFrameEventManager(ReplayEvent* buffer, u32 bufferSize)
                                                : fileIO(buffer, bufferSize) {
}

//Expects event to be dynamically alocated
void ReplayFrameEventManager::push(ReplayEvent* event) {
    events.push(event);
}

ReplayGameStartEvent* ReplayFrameEventManager::getGameStartEvent() {
    //for now, should always be first event if it exists, might change in future
    return (ReplayGameStartEvent*) events[0];
    //return (ReplayGameStartEvent*) mainBuffer;
}

ReplayFrameStartEvent* ReplayFrameEventManager::getFrameStartEvent() {
    for(int i = 0; i < events.size(); i++) {
        if(events[i]->id == ReplayEventID::frameStart) {
            return (ReplayFrameStartEvent*) events[i];
        }
    }
    return nullptr;
}

void ReplayFrameEventManager::saveFrameEvents() {
    for(int i = 0; i < events.size(); i++) {
        fileIO.write(events[i], events[i]->size);
    }

    clear();
}

void ReplayFrameEventManager::saveAndClose() {
    saveFrameEvents();
    fileIO.closeWhenBufferEmpty();
}

void ReplayFrameEventManager::clear() {
    for(int i = 0; i < events.size(); i++) {
        delete events[i];
    }
    events.clear();
}


void ReplayFrameEventManager::loadNextFrame() {
    clear();

    loadEventsToVector();
}

ReplayPreFrameFighterEvent* ReplayFrameEventManager::getFighterEventFromPort(int port, bool createIfNotFound) {
    for(int i = 0; i < events.size(); i++) {
        if(events[i]->id == ReplayEventID::preFrame) {
            if(((ReplayPreFrameFighterEvent*) events[i])->port == port)
                return (ReplayPreFrameFighterEvent*) events[i];
        }
    }
    return nullptr;
}


void ReplayFrameEventManager::loadEventsToVector() {
    ReplayEvent* event;
    do {
        event = (ReplayEvent*) fileIO.buffer.currentStart;
        event = (ReplayEvent*) fileIO.read(event->size);
        events.push(event);
    } while((event->id != ReplayEventID::frameEnd) && (event->id != ReplayEventID::gameStart));
}

void ReplayFrameEventManager::openReplayFileForWriting(FILE* file) {
    fileIO.openForWriting(file);
}

void ReplayFrameEventManager::openReplayFileForReading(FILE* file) {
    fileIO.openForReading(file);
}

