//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_SOWORKMANAGEMODULEIMPL_H
#define PROJECTMCODES_SOWORKMANAGEMODULEIMPL_H


#include "soGeneralWorkSimple.h"

enum VarType {
    LA = 0x10000000,
    RA = 0x20000000
};

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

    void setFlag(VarType type, int offset, bool value);
};

#define _getInt_soWorkManageModuleImpl ((int (*)(void* self, unsigned int identifier)) 0x807ac9e0)
#define _isFlag_soWorkManageModuleImpl ((bool (*)(void* self, unsigned int identifier)) 0x807accdc)
#define _setFlag_soWorkManageModuleImpl ((void (*)(void* self, bool onOff, unsigned int identifier)) 0x807adea8)

#endif //PROJECTMCODES_SOWORKMANAGEMODULEIMPL_H
