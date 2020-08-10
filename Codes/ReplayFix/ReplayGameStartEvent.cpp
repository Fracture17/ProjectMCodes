//
// Created by johno on 7/24/2020.
//

#include "ReplayGameStartEvent.h"

ReplayGameStartEvent::ReplayGameStartEvent() {
    size = sizeof(ReplayGameStartEvent);
    id = ReplayEventID::gameStart;
}
