//
// Created by dareb on 7/29/2020.
//

#ifndef PROJECTMCODES_SOGROUNDMODULEIMPL_H
#define PROJECTMCODES_SOGROUNDMODULEIMPL_H

struct unk4_soGroundModule {
    char _spacer[0x18];

    // 0x18
    // used in custom AI function 0x3D
    float landingCollisionBottomYPos;
};

struct unk3_soGroundModule {
    char _spacer[0x8];

    // 0x8
    int airGroundState;

    char _spacer2[0x60 - 0x8 - 4];

    // 0x60
    // used in custom AI function 0x3D
    unk4_soGroundModule * unk1;
};

struct unk2_soGroundModule {
    char _spacer[0x10];

    // 0x10
    // used in custom AI function 0x3D
    unk3_soGroundModule * unk1;
};

struct unk1_soGroundModule {
    char _spacer[0x28];

    // 0x28
    // used in custom AI function 0x3D
    unk2_soGroundModule * unk1;
};

struct soGroundModuleImpl {
    double getDistanceFromUnderGrCol(double min_unk, float * unk);

    char _spacer[0x1C];

    // 0x1C
    // used in custom AI function 0x3D
    unk1_soGroundModule * unk1;

    char _spacer2[0x24 - 0x1C - 4];

    // 0x24
    // used in custom AI function 0x4B / 0x4C
    int * unk2;
};

#define _getDistanceFromUnderGrCol_soGroundModuleImpl ((double (*)(double min_unk, soGroundModuleImpl * self, float * unk)) 0x80734f8c)

#endif //PROJECTMCODES_SOGROUNDMODULEIMPL_H
