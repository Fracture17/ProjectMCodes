//
// Created by Ilir on 2022-02-11.
//

#ifndef PROJECTMCODES_GRGIMMICK_H
#define PROJECTMCODES_GRGIMMICK_H

#include "Ground.h"

struct grGimmick : public Ground {
    void getPos(Vec3f* pos);
    void getRot(Vec3f* rot);
    void getScale(Vec3f* scale);
    void setPos(double x, double y, double z);
    void setRot(double rot1, double rot2, double rot3);
    void setScale(double x, double y, double z);
};

#define getPos_grGimmick ((void (*)(Vec3f* pos, grGimmick* self)) 0x809785e4)
#define getRot_grGimmick ((void (*)(Vec3f* rot, grGimmick* self)) 0x80978690)
#define getScale_grGimmick ((void (*)(Vec3f* scale, grGimmick* self)) 0x80978714)

#define setPos3_grGimmick ((void (*)(grGimmick* self, double x, double y, double z)) 0x809785c8)
//#define setRot1_grGimmick ((void (*)(grGimmick* self, int unk2)) 0x8097864c ) // unk2 is just a struct with the rotations
#define setRot2_grGimmick ((void (*)(grGimmick* self, double rot1, double rot2, double rot3)) 0x80978674)
#define setScale2_grGimmick ((void (*)(grGimmick* self, double x, double y, double z)) 0x809786f8)




#endif //PROJECTMCODES_GRGIMMICK_H
