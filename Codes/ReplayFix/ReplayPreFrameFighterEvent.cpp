//
// Created by johno on 7/24/2020.
//

#include "ReplayPreFrameFighterEvent.h"

ReplayPreFrameFighterEvent::ReplayPreFrameFighterEvent() {
    size = sizeof(ReplayPreFrameFighterEvent);
    id = ReplayEventID::preFrame;
}
