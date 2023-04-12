//
// Created by johno on 7/20/2020.
//

#include "gfApplication.h"

unsigned int getScene() {
    u32* ptr = (u32*)(0x805b4fd8 + 0xd4);
    ptr = (u32*) *ptr;
    if(ptr < (u32*)0xA0000000) {
        ptr = (u32*) *(ptr + (0x10 / 4));
        if(ptr != nullptr) {
            u32 scene = *(ptr + (8 / 4));
            return scene;
        }
    }
    return false;
}