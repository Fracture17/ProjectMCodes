//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_SOWORKMANAGEMODULEIMPL_H
#define PROJECTMCODES_SOWORKMANAGEMODULEIMPL_H


#include "soGeneralWorkSimple.h"

class soWorkManageModuleImpl {
public:
    char _spacer[0x20];
    //0x20
    soGeneralWorkSimple* LAVariables;
    //0x24
    soGeneralWorkSimple* RAVariables;
    //0x28
    //seems to be something with IC variables
    //not sure what the structure is yet
};


#endif //PROJECTMCODES_SOWORKMANAGEMODULEIMPL_H
