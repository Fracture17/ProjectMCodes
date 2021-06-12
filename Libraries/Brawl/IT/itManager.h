
#ifndef PROJECTMCODES_ITMANAGER_H
#define PROJECTMCODES_ITMANAGER_H

#include "Brawl/IT/BaseItem.h"

struct itManager {
    BaseItem* getItemFromIndex(u32 index);
};

#define _getItemFromIndex_itManager ((BaseItem * (*)(itManager * self, u32 index)) 0x809b33a0)
#define _getInstance_itManager ((itManager * (*)()) 0x809ac12c)

#define ITEM_MANAGER _getInstance_itManager()


#endif //PROJECTMCODES_ITMANAGER_H