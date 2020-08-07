//
// Created by johno on 7/15/2020.
//

#ifndef PROJECTMCODES_SOMOTIONMODULE_H
#define PROJECTMCODES_SOMOTIONMODULE_H


struct soMotionModule {
    float getFrame();

    char _spacer[0x40];
    float animFrame;

    char _spacer2[0x58 - 0x40 - 4];
    // 0x58
    int subAction;
};


#define _getFrame_soMotionModule ((float (*)(soMotionModule* self)) 0x8071f474)

#endif //PROJECTMCODES_SOMOTIONMODULE_H
