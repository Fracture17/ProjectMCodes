//
// Created by johno on 7/1/2020.
//

#ifndef PROJECTMCODES_SOGENERALWORKSIMPLE_H
#define PROJECTMCODES_SOGENERALWORKSIMPLE_H


struct soGeneralWorkSimple {
    bool isFlag(int flagIdx);

    char _spacer[0xC];
    //0xC
    int (*basics)[];
    //0x10
    int basicsSize;
    //0x14
    float (*floats)[];
    //0x18
    int floatsSize;
    //0x1C
    void* bitVariables;
    //0x20
    int bitsSize;
};

#define _isFlag_soGeneralWorkSimple ((int (*)(void* self, unsigned int identifier, unsigned int anotherThing)) 0x807990ac)

#endif //PROJECTMCODES_SOGENERALWORKSIMPLE_H
