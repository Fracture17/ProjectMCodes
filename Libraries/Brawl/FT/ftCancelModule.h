//
// Created by dareb on 9/19/2020.
//

#ifndef PROJECTMCODES_FTCANCELMODULE_H
#define PROJECTMCODES_FTCANCELMODULE_H

struct ftCancelModule {
    int isEnableCancel();
    int unableCancelStatus();
    void enableCancelTransitionGroup(int groupID);

    char _spacer[0x38];
    bool canCancelAttack;
};

#define _unableCancelStatus_ftCancelModule ((int (*)(ftCancelModule* self)) 0x8084bd74)
#define _isEnableCancel_ftCancelModule ((int (*)(ftCancelModule* self)) 0x8084bd6c)
#define _enableCancelTransitionGroup_ftCancelModule ((void (*)(ftCancelModule* self, int groupID)) 0x8084B6D8)

#endif //PROJECTMCODES_FTCANCELMODULE_H
