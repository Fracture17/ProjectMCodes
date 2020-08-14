//
// Created by johno on 7/27/2020.
//

#include "ReplayLoadEvent.h"

ReplayLoadEvent::ReplayLoadEvent() {
    size = sizeof(ReplayLoadEvent);
    id = ReplayEventID::load;
}
