
#include "./soWorkManageModuleImpl.h"

void soWorkManageModuleImpl::setFlag(VarType type, int offset, bool value) {
  _setFlag_soWorkManageModuleImpl(this, value, type | 0x02000000 | offset);
}