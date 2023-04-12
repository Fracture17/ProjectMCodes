//
// Created by dareb on 7/29/2020.
//

#include "soStatusModuleImpl.h"

void soStatusModuleImpl::changeStatusForce(u32 action, soModuleAccessor* accesser) {
  _changeStatusForce_soStatusModuleImpl(this, action, accesser);
}

void soStatusModuleImpl::changeStatusRequest(u32 action, soModuleAccessor* accesser) {
  _changeStatusRequest_soStatusModuleImpl(this, action, accesser);
}

void soStatusModuleImpl::enableTransitionTermAll() {
  _enableTransitionTermAll_soStatusModuleImpl(this);
}