

#include "aiParam.h"

ATKDContent* AiParam::searchAtkData(short subactionID) {
  return _searchAtkData_ATKDHead(this->ATKDPtr, subactionID);
}