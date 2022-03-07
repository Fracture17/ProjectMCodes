//
// Created by dareb on 9/12/2020.
//

#include "ftCommonDataAccessor.h"

unsigned int* ftCommonDataAccessor::getFtStatusData(int fighterID, int ftAction) {
    return _getFtStatusData_ftCommonDataAccessor(FIGHTER_COMMON_DATA_ACCESSOR, fighterID, ftAction);
}

ftParam* ftCommonDataAccessor::getParam(int fighterID) {
    return _getParam_ftCommonDataAccessor(FIGHTER_COMMON_DATA_ACCESSOR, fighterID, 0);
}

ftParamEtc* ftCommonDataAccessor::getParamEtc(int fighterID) {
    return _getParamEtc_ftCommonDataAccessor(FIGHTER_COMMON_DATA_ACCESSOR, fighterID);
}
