#ifndef PROJECTMCODES_AIPARAM_H
#define PROJECTMCODES_AIPARAM_H

#include "Containers/Vec3f.h"

struct ATKDContent {
  short sact;
  short unk;
  short start;
  short end;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
};

struct aiInput;
struct aiParam {
    ATKDContent* searchAtkData(short subactionID);

    // 0x0
    int* AIPDPtr;
    // 0x4
    int* ATKDPtr;
    // 0x8
    float levelValue;
    // 0xC
    char _spacer[0x4C - 0x8 - 4];
    // 0x4C
    aiInput* input;

    char _spacer2[0x5C - 0x4C - 4];

    // 0x5C
    Vec3f position;
    // 0x68
    Vec3f scale;
    // 0x74
    Vec3f topNPos;

};

#define _searchAtkData_ATKDHead ((ATKDContent* (*)(int * self, short subactionID)) 0x8092a744) 

#endif //PROJECTMCODES_AIPARAM_H
