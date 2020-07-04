//
// Created by johno on 7/2/2020.
//

#ifndef PROJECTMCODES_GMGLOBALMODEMELEE_H
#define PROJECTMCODES_GMGLOBALMODEMELEE_H


struct gmGlobalModeMelee {
    char _spacer[0x320];
}__attribute__((packed, aligned(2)));


#define GM_GLOBAL_MODE_MELEE ((gmGlobalModeMelee*) 0x90180f20)


#endif //PROJECTMCODES_GMGLOBALMODEMELEE_H
