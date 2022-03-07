//
// Created by dareb on 9/12/2020.
//

#ifndef PROJECTMCODES_FTSTATUSCOMMON_H
#define PROJECTMCODES_FTSTATUSCOMMON_H

#include "Brawl/FT/ftParamCustomizeModuleImpl.h"

struct ftCommonDataAccessor {
    unsigned int* getFtStatusData(int fighterID, int ftAction);
    ftParam* getParam(int fighterID);
    ftParamEtc* getParamEtc(int fighterID);
};

//static location of global ftCommonDataAccessor object
#define FIGHTER_COMMON_DATA_ACCESSOR ((ftCommonDataAccessor*) 0x80b87fcc)

#define _getFtStatusData_ftCommonDataAccessor ((unsigned int* (*)(ftCommonDataAccessor* self, int fighterID, int ftAction)) 0x8084ff60)
#define _getParam_ftCommonDataAccessor ((ftParam* (*)(ftCommonDataAccessor* self, int fighterID, int unk)) 0x8084ff28)
#define _getParamEtc_ftCommonDataAccessor ((ftParamEtc* (*)(ftCommonDataAccessor* self, int fighterID)) 0x8084ff48)


#endif //PROJECTMCODES_FTSTATUSCOMMON_H
