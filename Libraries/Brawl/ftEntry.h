//
// Created by johno on 7/23/2020.
//

#ifndef PROJECTMCODES_FTENTRY_H
#define PROJECTMCODES_FTENTRY_H


struct ftEntry {
    //0xD
    //0 seems to mean hasn't started loading, 1 seems to mean is ready, but won't load in replays
    //3 means completly ready
    char _loadStatus;
};


#endif //PROJECTMCODES_FTENTRY_H
