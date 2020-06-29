//
// Created by johno on 6/25/2020.
//

#ifndef CSSCUSTOMCONTROLS_MUSELCHARPLAYERAREA_H
#define CSSCUSTOMCONTROLS_MUSELCHARPLAYERAREA_H


#include "MuSelctChrList.h"
#include "MuSelctChrNameEntry.h"

struct muSelCharPlayerArea {
    char spacer[0x1FC];

    //0x1FC
    MuSelctChrList muSelctChrList;

    //0x370
    MuSelctChrNameEntry muSelctChrNameEntry;
};


#endif //CSSCUSTOMCONTROLS_MUSELCHARPLAYERAREA_H
