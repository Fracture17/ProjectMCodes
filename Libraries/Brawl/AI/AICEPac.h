#ifndef PROJECTMCODES_AICEPac_H
#define PROJECTMCODES_AICEPac_H

#include "stddef.h"

struct AICEStringEntryPair {
  int index;
  int offset;
};

struct AICEStringEntry {
  char* getStringEntry(int index);
  int count;
  int spacer[3];
};

struct AICEEntry {
  int ID;
  int EventsOffset;
  int part2Offset;
  int varClearAmount;
};

struct AICEPac {
    AICEEntry* getCEEntry(int entryNumber);
    int getStringCount(int entryNumber);
    char* getStringEntry(int entryNumber, int index);

    int unk1;
    // 0x4
    int numEntries;
    int unk2;
    int unk3;
};

#define _get_script_AISCRIPTPACK ((AICEEntry* (*)(void* AICharCE, void* AICommonCE, u32 targetScript)) 0x8091dedc)

#endif //PROJECTMCODES_AICEPac_H
