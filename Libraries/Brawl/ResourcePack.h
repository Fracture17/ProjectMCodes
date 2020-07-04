//
// Created by johno on 7/2/2020.
//

#ifndef PROJECTMCODES_RESOURCEPACK_H
#define PROJECTMCODES_RESOURCEPACK_H


//this has a bunch of stuff for collections
//My name, couldn't find reference in function map
struct ResourcePack {
    char _spacer[0x1D10];

    //0x1D10
    //usually -1, set to 0 when loading replay or it will just hang
    //not sure if used for anything else
    u32 startReplayFlag;
};


#endif //PROJECTMCODES_RESOURCEPACK_H
