//
// Created by dareb on 8/8/2020.
//

#ifndef PROJECTMCODES_DRAWABLE_H
#define PROJECTMCODES_DRAWABLE_H

#include <Wii/GX.h>

struct Point {
    Point(int color, float x1, float y1, int thickness);

    GXColor color;
    float x;
    float y;
    int thickness;
    void draw();
};

struct Line {
    Line(int color, float x1, float y1, float x2, float y2, int thickness);

    GXColor color;
    float x1;
    float y1;
    float x2;
    float y2;
    int thickness;
    void draw();
};

#endif //PROJECTMCODES_DRAWABLE_H
