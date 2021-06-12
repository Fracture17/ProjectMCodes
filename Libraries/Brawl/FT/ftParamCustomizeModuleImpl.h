//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_FTPARAMCUSTOMIZEMODULEIMPL_H
#define PROJECTMCODES_FTPARAMCUSTOMIZEMODULEIMPL_H


struct ftParamCustomizeModuleImpl {
    char _spacer[0x70];
    // 0x70
    float gravity;
    // 0x74
    float maxFallSpeed;

    char _spacer2[0x90 - 0x74 - 4];

    // 0x90
    float fastFallSpeed;

    char _spacer3[0xB4 - 0x90 - 4];
    
    // 0xB4
    float weight;
};


#endif //PROJECTMCODES_FTPARAMCUSTOMIZEMODULEIMPL_H
