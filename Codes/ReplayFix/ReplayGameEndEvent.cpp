//
// Created by johno on 7/25/2020.
//

#include "ReplayGameEndEvent.h"

ReplayGameEndEvent::ReplayGameEndEvent() {
    size = sizeof(ReplayGameEndEvent);
    id = ReplayEventID::gameEnd;
}
