//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_FTCONTROLLERMODULEIMPL_H
#define PROJECTMCODES_FTCONTROLLERMODULEIMPL_H


#include "soControllerModuleImpl.h"
#include "soModuleAccessor.h"
#include "soControllerImpl.h"


struct ftControllerModuleImpl: public soControllerModuleImpl {
    char _spacer_ft[0x2C - sizeof(soControllerModuleImpl)];
    //0x2C
    soModuleAccessor* modules;
    //0x30
    soControllerImpl controller;
    char _spacer_ft2[0x14C - 0x34];
    //0x14C
    //Seems to just be a ptr to controller attribute
    soControllerImpl* controllerPtr;
};


#endif //PROJECTMCODES_FTCONTROLLERMODULEIMPL_H
