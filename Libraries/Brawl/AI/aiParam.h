#ifndef PROJECTMCODES_AIPARAM_H
#define PROJECTMCODES_AIPARAM_H

#include "stddef.h"

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

struct AiInput;
struct FtInput;
struct AiParam {
    STATIC_METHOD_1ARG(0x80929bd0, AiParam, float, get_width, char*, aiStatIdx)
    STATIC_METHOD_1ARG(0x80929c58, AiParam, float, get_height, char*, aiStatIdx)
    
    ATKDContent* searchAtkData(short subactionID);

    // 0x0
    int* AIPDPtr;
    // 0x4
    int* ATKDPtr;
    // 0x8
    char levelValue;

    char _spacer[0x48 - 0x8 - 1];
    // 0x48
    FtInput* ftInput;
    // 0x4C
    AiInput* aiInput;

};

#define _searchAtkData_ATKDHead ((ATKDContent* (*)(int * self, short subactionID)) 0x8092a744) 

#endif //PROJECTMCODES_AIPARAM_H
