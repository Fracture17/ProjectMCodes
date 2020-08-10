//
// Created by johno on 7/24/2020.
//

#include "ReplayFrameStartEvent.h"

ReplayFrameStartEvent::ReplayFrameStartEvent() {
    size = sizeof(ReplayFrameStartEvent);
    id = ReplayEventID::frameStart;
}
