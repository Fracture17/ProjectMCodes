//
// Created by Ilir on 2022-01-04.
//

#ifndef PROJECTMCODES_SOITEMMANAGEMODULEIMPL_H
#define PROJECTMCODES_SOITEMMANAGEMODULEIMPL_H

#include "../../../Globals/stddef.h"

struct soItemManageModuleImpl {
    void attachItem(unsigned int itemId, unsigned int unk3, int unk4);
};

#define attachItem_soItemManageModuleImpl ((void (*)(soItemManageModuleImpl* self, unsigned int itemId, unsigned int unk2, int unk3)) 0x807c3b1c)


#endif //PROJECTMCODES_SOITEMMANAGERMODULEIMPL_H
