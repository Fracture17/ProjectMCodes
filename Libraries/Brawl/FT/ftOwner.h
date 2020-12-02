//
// Created by dareb on 7/25/2020.
//

#ifndef PROJECTMCODES_FTOWNER_H
#define PROJECTMCODES_FTOWNER_H

struct ftOwner {
    double getDamage();
    void setDamage(double newValue, int shouldLog);

    char spacer1[0x24];

    // 0x24
    float fDamage;
};

#define _getDamage_ftOwner ((double (*)(ftOwner * owner)) 0x8081c264)
#define _setDamage_ftOwner ((void (*)(double newValue, ftOwner * owner, int shouldLog)) 0x8081bdcc)

#endif //PROJECTMCODES_FTOWNER_H
