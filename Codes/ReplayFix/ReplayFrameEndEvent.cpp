//
// Created by johno on 7/25/2020.
//

#include "ReplayFrameEndEvent.h"

ReplayFrameEndEvent::ReplayFrameEndEvent() {
    size = sizeof(ReplayFrameEndEvent);
    id = ReplayEventID::frameEnd;
}
