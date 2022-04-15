
#ifndef PROJECTMCODES_ITMANAGER_H
#define PROJECTMCODES_ITMANAGER_H

#include "Brawl/IT/BaseItem.h"
#include "Containers/ArrayList.h"

struct itArchive {};

// TODO: Use ArrayVector wrapper
struct itManager {
    BaseItem* getItemFromIndex(u32 index);
    BaseItem* createItem(itemIdName itemID, u32 variantID);
    void preloadAssist(itemIdName itemId);
    void preloadPokemon(itemIdName itemId);
    void preloadItemKindArchive(itemIdName itemId);

    char _spacer[0xC8];
    ArrayList<itArchive*> itArchiveArrayList;

    char _spacer2[0x6D8 - 0xC8 - sizeof(itArchiveArrayList)];
    ArrayList<BaseItem*> baseItemArrayList;

    char _spacer3[0x10BC - 0x6D8 - sizeof(baseItemArrayList)];
    itemIdName nextAssist;

    char _spacer4[0x10C8 - 0x10BC - 4];
    // Pokemon Queue
    ArrayList<u32> itKindArrayList;
};

#define _getItemFromIndex_itManager ((BaseItem * (*)(itManager * self, u32 index)) 0x809b33a0)
#define _getInstance_itManager ((itManager * (*)()) 0x809ac12c)
#define _createItem_itManager ((BaseItem * (*)(itManager * self, itemIdName itemID, u32 variantID, int unk1, int unk2, int unk3, int unk4, int unk5, int unk6)) 0x809b1a84)
#define _preloadAssist_itManager ((void (*)(itManager * self, itemIdName itemId)) 0x809afa0c)
#define _preloadPokemon_itManager ((void (*)(itManager * self, itemIdName itemId)) 0x809afcd8)
#define _preloadItemKindArchive_itManager ((void (*)(itManager * self, itemIdName itemId, int unk1, int unk2, int unk3)) 0x809ae960)

#define ITEM_MANAGER _getInstance_itManager()

#endif //PROJECTMCODES_ITMANAGER_H